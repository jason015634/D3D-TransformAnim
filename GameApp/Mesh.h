#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <directxtk/SimpleMath.h>

#include "ConstantBuffers.h"

#include "Helper.h"
using namespace Microsoft::WRL;
using namespace DirectX::SimpleMath;
using namespace std;

struct Vertex
{
    Vector3 Position;
    Vector2 Texcoord;
    Vector3 Normal;
    Vector3 Tangent;
};

struct Texture
{
    string Type;
    string Path;
    ID3D11ShaderResourceView* textureResource;

    void Release()
    {
        SAFE_RELEASE(textureResource);
    }
};

struct aiMesh;
class Node;
class Model;
class Material;
// Mesh Class
// 3d �𵨷κ��� �ҷ��� ���ؽ� ������ ���ؽ� ���۸� �����Ѵ�.
// 3d �𵨷κ��� �ҷ��� ���ؽ� ������ �ε��� ���۸� �����Ѵ�.
struct ConstantBuffer;
class Mesh
{
public:
    Mesh(Model* _pOwner);
    ~Mesh();

public:
    ID3D11Buffer* m_VertexBuffer;
    ID3D11Buffer* m_IndexBuffer;

    vector<Texture> m_textures;
    UINT m_VertexBufferStride = 0;    // ���ؽ� �ϳ��� ũ��
    UINT m_VertexBufferOffset = 0;    // ���ؽ� ������ ������
    UINT m_VertexCount = 0;           // ���ؽ� ���� 
    UINT m_IndexCount = 0;            // �ε��� ����
    UINT m_MaterialIndex = 0;         // ���׸��� �ε���  

    Model* m_pOwner;
    Material* m_pMaterial;
    Node* m_pParentNode = nullptr;
private:
    // aiMesh�κ��� �ҷ��� ������ Create �Լ��� �����ϱ� ���� �����Լ�
    // ���ؽ� ���� ����
    void CreateVertexBuffer(ID3D11Device* device, Vertex* vertices, UINT vertexCount);
    // �ε��� ���� ����
    void CreateIndexBuffer(ID3D11Device* device, WORD* indices, UINT indexCount);

public:
    // aiMesh�κ��� �ҷ��� ������ ���ؽ� ���� & �ε��� ���� ����
    void Create(ID3D11Device* device, aiMesh* mesh);

    void Render(ID3D11DeviceContext* _dc, ID3D11BlendState* m_pAlphaBlendState, ID3D11Buffer* m_pMaterialCB, ID3D11Buffer* m_pTransformCB, ConstantBuffer* test);
};

