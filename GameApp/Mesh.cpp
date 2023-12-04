#include "pch.h"
#include "Mesh.h"

#include <memory>
#include "ConstantBuffers.h"
#include "Helper.h"
#include <assimp/scene.h>
#include "Material.h"
#include "Model.h"
#include "ResourceLoader.h"
Mesh::Mesh(Model* _pOwner)
	:m_pOwner(_pOwner)
{
}

Mesh::~Mesh()
{
	SAFE_RELEASE(m_VertexBuffer);
	SAFE_RELEASE(m_IndexBuffer);
}

void Mesh::CreateVertexBuffer(ID3D11Device* device, Vertex* vertices, UINT vertexCount)
{
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(Vertex) * vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;

	// ���ؽ� ���� ����
	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = vertices;
	HR_T(device->CreateBuffer(&bd, &vbData, &m_VertexBuffer));

	// ���ؽ� ���� ����
	m_VertexCount = vertexCount;
	m_VertexBufferStride = sizeof(Vertex);
	m_VertexBufferOffset = 0;
}

void Mesh::CreateIndexBuffer(ID3D11Device* device, WORD* indices, UINT indexCount)
{
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(WORD) * indexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;

	// �ε��� ���� ����
	D3D11_SUBRESOURCE_DATA ibData = {};
	ibData.pSysMem = indices;
	HR_T(device->CreateBuffer(&bd, &ibData, &m_IndexBuffer));

	// �ε��� ���� ����
	m_IndexCount = indexCount;
}

void Mesh::Create(ID3D11Device* device, aiMesh* mesh)
{
	m_MaterialIndex = mesh->mMaterialIndex;
	m_pMaterial = m_pOwner->m_Materials[m_MaterialIndex];
	// ���ؽ� ���� ����
	// ���ۿ� ���ε��Ǹ� �ٷ� ������ �� �ֵ��� unique_ptr ���
	unique_ptr<Vertex[]> vertices(new Vertex[mesh->mNumVertices]);
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		vertices[i].Position = Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		// ���� ���� ���׸����� Material Ŭ�������� �����ϹǷ� TexCoords ��Ʈ�� 1���� ������
		vertices[i].Texcoord = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		vertices[i].Normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		vertices[i].Tangent = Vector3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
	}
	CreateVertexBuffer(device, vertices.get(), mesh->mNumVertices);

	// �ε��� ���� ����
	// ���ۿ� ���ε��Ǹ� �ٷ� ������ �� �ֵ��� unique_ptr ���
	// ��(�ﰢ��)�� ������ 3���̱� ������ ��(Face)�� ������ 3�� ������
	unique_ptr<WORD[]> indices(new WORD[mesh->mNumFaces * 3]);
	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		indices[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
		indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
		indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
	}
	CreateIndexBuffer(device, indices.get(), mesh->mNumFaces * 3);
}

void Mesh::Render(ID3D11DeviceContext* _dc, ID3D11BlendState* m_pAlphaBlendState, ID3D11Buffer* m_pMaterialCB, ID3D11Buffer* m_pTransformCB, ConstantBuffer* test)
{
	m_pMaterial->m_materialCB.Diffuse = m_pMaterial->m_baseColor;
	m_pMaterial->m_materialCB.Ambient = m_pOwner->m_materialCB.Ambient;
	//m_pMaterial->m_materialCB.Diffuse = m_pOwner->m_materialCB.Diffuse;
	m_pMaterial->m_materialCB.Emissive = m_pOwner->m_materialCB.Emissive;
	m_pMaterial->m_materialCB.Specular = m_pOwner->m_materialCB.Specular;
	m_pMaterial->m_materialCB.SpecularPower = m_pOwner->m_materialCB.SpecularPower;


	auto buifferData = *test;

	m_pMaterial->Render(_dc, m_pAlphaBlendState, m_pMaterialCB);
	_dc->UpdateSubresource(m_pTransformCB, 0, nullptr, test, 0, 0);
	//_dc->UpdateSubresource(m_pBoneTransformBuffer, 0, nullptr, &m_matrixPalleteCB, 0, 0);
	_dc->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	_dc->IASetVertexBuffers(0, 1, &m_VertexBuffer, &m_VertexBufferStride, &m_VertexBufferOffset);
	_dc->DrawIndexed(m_IndexCount, 0, 0);
}