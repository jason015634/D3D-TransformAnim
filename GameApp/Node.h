#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <directxtk/SimpleMath.h>
#include "ConstantBuffers.h"
using namespace Microsoft::WRL;
using namespace DirectX::SimpleMath;
using namespace std;
class Model;
class Mesh;
class NodeAnimation;
class Node
{
public:
	Node(Model* pOwner);
	~Node();
public:
	string m_name;
	Node* m_pParentNode;
	Model* m_pOwner;
	Matrix m_WorldTM;
	Matrix m_LocalTM;

	vector<Node*> m_pChildren = {};
	vector<Mesh*>m_pMeshes = {};
	NodeAnimation* m_pNodeAnimation;
	int m_curIdx = 0;
	float m_elapsedTime;
	float m_animationSpeed = 1.f;
	void Update(float deltaTime);
	void Render(ID3D11DeviceContext* _dc, ID3D11BlendState* m_pAlphaBlendState, ID3D11Buffer* m_pMaterialCB, ID3D11Buffer* m_pTransformCB, ConstantBuffer* test);
};

