#include "pch.h"
#include "ResourceLoader.h"
#include <assimp\Importer.hpp>
#include <assimp\cimport.h>
#include <DirectXMath.h>
#include <assimp\postprocess.h>
#include <assimp\scene.h>
#include <directxtk\DDSTextureLoader.h>
#include <directxtk\WICTextureLoader.h>
#include <filesystem>
#include "Mesh.h"
#include "Material.h"
#include "Node.h"
#include "Model.h"
#include "Animation.h"
HRESULT ResourceLoader::CreateTextureFromFile(ID3D11Device* d3dDevice, std::wstring szFileName, ID3D11ShaderResourceView** textureView)
{
	HRESULT hr = S_OK;
	//std::wstring _temp = L"..//res//textures//";

	//szFileName = _temp + szFileName;
	// Load the Texture
	hr = DirectX::CreateDDSTextureFromFile(d3dDevice, szFileName.c_str(), nullptr, textureView);
	if (FAILED(hr))
	{
		hr = DirectX::CreateWICTextureFromFile(d3dDevice, szFileName.c_str(), nullptr, textureView);
		if (FAILED(hr))
		{
			MessageBoxW(NULL, GetComErrorString(hr), szFileName.c_str(), MB_OK);
			return hr;
		}
	}
}

Model* ResourceLoader::LoadModel(ID3D11Device* device, const string& _filePath)
{
	Model* pGameObject = new Model();
	// 8. FBX Loading
	Assimp::Importer importer;
	unsigned int importFlags = aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace |
		aiProcess_ConvertToLeftHanded;

	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);

	// FBX 파일 경로를 scene에 바인딩
	const aiScene* scene = importer.ReadFile(_filePath.c_str(), importFlags);

	assert(scene);

	m_Materials.resize(scene->mNumMaterials);
	for (int i = 0; i < scene->mNumMaterials; ++i)
	{
		m_Materials[i] = ProcessMaterial(device, scene->mMaterials[i], pGameObject);
	}
	pGameObject->m_Materials = m_Materials;
	m_Meshes.resize(scene->mNumMeshes);
	for (int i = 0; i < scene->mNumMeshes; ++i)
	{
		m_Meshes[i] = ProcessMesh(device, scene->mMeshes[i], pGameObject);
	}
	pGameObject->m_Meshes = m_Meshes;

	pGameObject->m_RootNode = ProcessNode(scene->mRootNode, nullptr, pGameObject);

	m_Animations.resize(scene->mNumAnimations);
	for (int i = 0; i < scene->mNumAnimations; ++i)
	{
		Animation* animation = new Animation(pGameObject);
		animation->AnimationName = (scene->mAnimations[i]->mName).C_Str();
		animation->m_tickPerSecond = tickPerSecond = static_cast<float>(scene->mAnimations[i]->mTicksPerSecond);
		animation->AnimationDuration = (scene->mAnimations[i]->mDuration) / tickPerSecond;
		animation->NodeAnimations = ProcessAnimation(scene->mAnimations[i], pGameObject, animation);

		m_Animations.push_back(animation);
	}

	AssignAnimation(pGameObject->m_RootNode);

	importer.FreeScene();
	return pGameObject;
}

Node* ResourceLoader::ProcessNode(aiNode* _node, Node* _parentNode, Model* _model)
{
	Node* newNode = new Node{_model};

	newNode->m_name = _node->mName.C_Str();
	newNode->m_pParentNode = _parentNode;
	newNode->m_LocalTM = XMMatrixTranspose(XMMATRIX(_node->mTransformation[0]));
	_model->m_Nodes.push_back(newNode);

	//newNode->m_pMeshes.resize(_node->mNumMeshes);
	for (UINT i = 0; i < _node->mNumMeshes; i++) {
		_model->m_Meshes[_node->mMeshes[i]]->m_pParentNode = newNode;
		newNode->m_pMeshes.push_back(_model->m_Meshes[_node->mMeshes[i]]);
	}

	//if (_node == _scene->mRootNode)
	//{
	//	m_pRootNode = newNode;
	//}
	//else
	//{
	//}

	for (UINT i = 0; i < _node->mNumChildren; i++) {
		newNode->m_pChildren.push_back(ProcessNode(_node->mChildren[i], newNode, _model));
	}

	return newNode;
}

Mesh* ResourceLoader::ProcessMesh(ID3D11Device* _device, aiMesh* _mesh, Model* _model)
{
	Mesh* mesh = new Mesh(_model);
	mesh->Create(_device, _mesh);

	return mesh;
}

Material* ResourceLoader::ProcessMaterial(ID3D11Device* device, aiMaterial* _material, Model* _model)
{
	Material* material = new Material(_model);
	material->Create(device, _material);

	return material;
}

vector<NodeAnimation> ResourceLoader::ProcessAnimation(aiAnimation* _animation, Model* _model, Animation* _anim)
{
	vector<NodeAnimation> nodeAnims;
	for (int i = 0; i < _animation->mNumChannels; ++i)
	{
		NodeAnimation* nodeAnim = new NodeAnimation;
		nodeAnim->NodeName = (_animation->mChannels[i]->mNodeName).C_Str();
		nodeAnim->AnimationKeys = _anim->Create(_animation->mChannels[i]);

		nodeAnims.push_back(*nodeAnim);
	}

	return nodeAnims;
}

void ResourceLoader::AssignAnimation(Node* node)
{
	for (auto& anim : m_Animations[1]->NodeAnimations)
	{
		if (node->m_name == anim.NodeName)
		{
			node->m_pNodeAnimation = &anim;
			break;
		}
	}

	for (const auto& child : node->m_pChildren)
	{
		AssignAnimation(child);
	}
}

std::vector<Texture> ResourceLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene)
{
	std::vector<Texture> textures;
	for (UINT i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (UINT j = 0; j < textures_loaded_.size(); j++) {
			if (std::strcmp(textures_loaded_[j].Path.c_str(), str.C_Str()) == 0) {
				textures.push_back(textures_loaded_[j]);
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip) {   // If texture hasn't been loaded already, load it
			HRESULT hr;
			Texture texture;

			const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
			if (embeddedTexture != nullptr) {
				texture.textureResource = loadEmbeddedTexture(embeddedTexture);
			}
			else {
				std::string filename = std::string(str.C_Str());
				filename = directory_ + '/' + filename;
				std::wstring filenamews = std::wstring(filename.begin(), filename.end());
				HR_T(CreateTextureFromFile(dev_, filenamews.c_str(), &texture.textureResource));
			}
			texture.Type = typeName;
			texture.Path = str.C_Str();
			textures.push_back(texture);
			this->textures_loaded_.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

ID3D11ShaderResourceView* ResourceLoader::loadEmbeddedTexture(const aiTexture* embeddedTexture)
{
	HRESULT hr;
	ID3D11ShaderResourceView* texture = nullptr;

	if (embeddedTexture->mHeight != 0) {
		// Load an uncompressed ARGB8888 embedded texture
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = embeddedTexture->mWidth;
		desc.Height = embeddedTexture->mHeight;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA subresourceData;
		subresourceData.pSysMem = embeddedTexture->pcData;
		subresourceData.SysMemPitch = embeddedTexture->mWidth * 4;
		subresourceData.SysMemSlicePitch = embeddedTexture->mWidth * embeddedTexture->mHeight * 4;

		ID3D11Texture2D* texture2D = nullptr;
		HR_T(dev_->CreateTexture2D(&desc, &subresourceData, &texture2D));
		
		HR_T(dev_->CreateShaderResourceView(texture2D, nullptr, &texture));
		
		return texture;
	}

	// mHeight is 0, so try to load a compressed texture of mWidth bytes
	const size_t size = embeddedTexture->mWidth;

	HR_T(CreateWICTextureFromMemory(dev_, devcon_, reinterpret_cast<const unsigned char*>(embeddedTexture->pcData), size, nullptr, &texture));

	return texture;
}
