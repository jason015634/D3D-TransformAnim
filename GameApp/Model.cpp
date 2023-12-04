#include "pch.h"
#include "Model.h"
#include "Mesh.h"
#include "Node.h"
#include "Animation.h"
void Model::Update(float deltaTime)
{
	// DX는 열우선
	XMMATRIX mScale = XMMatrixScalingFromVector(m_Scale);
	XMMATRIX mRotation = XMMatrixRotationX(m_Rotate.x)
		* XMMatrixRotationY(m_Rotate.y)
		* XMMatrixRotationZ(m_Rotate.z);

	XMMATRIX mTrans = XMMatrixTranslationFromVector(m_Position);
	
	XMMATRIX mBasis = XMMatrixIdentity();
	if (m_pParentObject) mBasis = m_pParentObject->GetMatrix();

	for (auto node : m_Nodes)
	{
		node->Update(deltaTime);
	}

	m_matrix = mScale * mRotation * mTrans * mBasis;
}

void Model::Render(ID3D11DeviceContext* _dc, ID3D11BlendState* m_pAlphaBlendState, ID3D11Buffer* m_pMaterialCB, ID3D11Buffer* m_pTransformCB, ConstantBuffer* test)
{
	m_RootNode->Render(_dc, m_pAlphaBlendState, m_pMaterialCB, m_pTransformCB, test);
}
