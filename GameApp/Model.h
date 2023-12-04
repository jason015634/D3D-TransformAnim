#pragma once

#include "ConstantBuffers.h"
#include <directxtk/SimpleMath.h>
class Node;
class Mesh;
class Material;
class Animation;
using namespace DirectX;
using namespace DirectX::SimpleMath;
class Model
{
private:
	Matrix m_matrix = {};
	Model* m_pParentObject = nullptr;
public:
	CB_Lighting m_materialCB;
	Vector3 m_Position = { 0,0,0 };
	Vector3 m_Scale = { 1,1,1 };
	Vector3 m_Rotate = { 0,0,0 };

	Node* m_RootNode;
	vector<Mesh*> m_Meshes = {};
	vector<Material*> m_Materials = {};
	vector<Animation*> m_Animation = {};
	vector<Node*> m_Nodes = {};
	float ProgressAnimTime;

public:
	XMMATRIX GetMatrix() { return m_matrix; }
	void Update(float deltaTime);
	void Render(ID3D11DeviceContext* _dc, ID3D11BlendState* m_pAlphaBlendState, ID3D11Buffer* m_pMaterialCB, ID3D11Buffer* m_pTransformCB, ConstantBuffer* test);
};

