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
// 3d 모델로부터 불러온 버텍스 정보로 버텍스 버퍼를 생성한다.
// 3d 모델로부터 불러온 버텍스 정보로 인덱스 버퍼를 생성한다.
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
    UINT m_VertexBufferStride = 0;    // 버텍스 하나의 크기
    UINT m_VertexBufferOffset = 0;    // 버텍스 버퍼의 오프셋
    UINT m_VertexCount = 0;           // 버텍스 개수 
    UINT m_IndexCount = 0;            // 인덱스 개수
    UINT m_MaterialIndex = 0;         // 머테리얼 인덱스  

    Model* m_pOwner;
    Material* m_pMaterial;
    Node* m_pParentNode = nullptr;
private:
    // aiMesh로부터 불러온 정보로 Create 함수를 진행하기 위한 내부함수
    // 버텍스 버퍼 생성
    void CreateVertexBuffer(ID3D11Device* device, Vertex* vertices, UINT vertexCount);
    // 인덱스 버퍼 생성
    void CreateIndexBuffer(ID3D11Device* device, WORD* indices, UINT indexCount);

public:
    // aiMesh로부터 불러온 정보로 버텍스 버퍼 & 인덱스 버퍼 생성
    void Create(ID3D11Device* device, aiMesh* mesh);

    void Render(ID3D11DeviceContext* _dc, ID3D11BlendState* m_pAlphaBlendState, ID3D11Buffer* m_pMaterialCB, ID3D11Buffer* m_pTransformCB, ConstantBuffer* test);
};

