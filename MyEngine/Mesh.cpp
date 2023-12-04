#include "Mesh.h"

#include <memory>

#include "../GameApp/Helper.h"
#include <assimp/scene.h>
Mesh::Mesh()
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