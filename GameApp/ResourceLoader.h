#pragma once
#include <string>
#include <d3d11_1.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "Mesh.h"
struct aiMaterial;
class Model;
class Material;
class Node;
class Animation;
class NodeAnimation;
class ResourceLoader
{
private:
	Node* m_pRootNode;

public:
	// 인스턴스를 만들지 않고도 클래스에서 호출 할 수 있음 -> 정적 멤버 함수
	static HRESULT CreateTextureFromFile(ID3D11Device* d3dDevice, std::wstring szFileName, ID3D11ShaderResourceView** textureView);
	Model* LoadModel(ID3D11Device* device, const string& _filePath);

	vector<Mesh*> m_Meshes;
	vector<Material*> m_Materials;
	vector<Animation*> m_Animations;

	int m_curIdx = 0;
	float tickPerSecond;
private:
	HWND hwnd_;
	ID3D11Device* dev_;
	ID3D11DeviceContext* devcon_;
	std::string directory_;
	std::vector<Texture> textures_loaded_;

	Node* ProcessNode(aiNode* _node, Node* _parentNode, Model* _model);
	Mesh* ProcessMesh(ID3D11Device* _device, aiMesh* _mesh, Model* _model);
	Material* ProcessMaterial(ID3D11Device* device, aiMaterial* _material, Model* _model);
	vector<NodeAnimation> ProcessAnimation(aiAnimation* _animation, Model* _model, Animation* _anim);
	void AssignAnimation(Node* node);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
	ID3D11ShaderResourceView* loadEmbeddedTexture(const aiTexture* embeddedTexture);
};
