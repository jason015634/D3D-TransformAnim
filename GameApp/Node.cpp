#include "pch.h"
#include "Node.h"
#include "Model.h"
#include "Mesh.h"
#include "ConstantBuffers.h"
#include "Animation.h"
Node::Node(Model* pOwner)
	:m_pOwner(pOwner)
{
}

Node::~Node()
{
}

void Node::Update(float deltaTime)
{
	Matrix mBasis = DirectX::XMMatrixIdentity();

	if (m_pNodeAnimation)
	{
		m_animationSpeed = 0.01f;
		m_elapsedTime += (deltaTime * m_animationSpeed);

		int nextIdx = (m_curIdx + 1) % (m_pNodeAnimation->AnimationKeys.size());
		if (m_elapsedTime > m_pNodeAnimation->AnimationKeys[nextIdx].FrameTime)
		{
			m_curIdx = nextIdx;
			if (m_curIdx == 0)
			{
				m_elapsedTime = 0.f;
			}
		}

		nextIdx = (m_curIdx + 1) % (m_pNodeAnimation->AnimationKeys.size());
		Vector3 prePosition = m_pNodeAnimation->AnimationKeys[m_curIdx].FramePos;
		Vector3 preScale = m_pNodeAnimation->AnimationKeys[m_curIdx].FrameScale;
		Quaternion preQuaternion = m_pNodeAnimation->AnimationKeys[m_curIdx].FrameRot;
		Vector3 nextPosition = m_pNodeAnimation->AnimationKeys[nextIdx].FramePos;
		Vector3 nextScale = m_pNodeAnimation->AnimationKeys[nextIdx].FrameScale;
		Quaternion nextQuaternion = m_pNodeAnimation->AnimationKeys[nextIdx].FrameRot;

		float t = (m_elapsedTime - m_pNodeAnimation->AnimationKeys[m_curIdx].FrameTime) /
			((m_pNodeAnimation->AnimationKeys[nextIdx].FrameTime) - (m_pNodeAnimation->AnimationKeys[m_curIdx].FrameTime));

		Vector3 position = Vector3::Lerp(prePosition, nextPosition, t);
		Vector3 scale = Vector3::Lerp(preScale, nextScale, t);
		Quaternion quaternion = Quaternion::Slerp(preQuaternion, nextQuaternion, t);

		m_LocalTM = Matrix::CreateScale(scale) * Matrix::CreateFromQuaternion(quaternion) * Matrix::CreateTranslation(position);

	}
	
	if (m_pParentNode)
		m_WorldTM = m_LocalTM * m_pParentNode->m_WorldTM;
	else
		m_WorldTM = m_pOwner->GetMatrix();

	//for (auto child : m_pChildren)
	//{
	//	child->Update();
	//}if (m_pNodeAnimation)
	
}

void Node::Render(ID3D11DeviceContext* _dc, ID3D11BlendState* m_pAlphaBlendState, ID3D11Buffer* m_pMaterialCB, ID3D11Buffer* m_pTransformCB, ConstantBuffer* test)
{
	test->mWorld = XMMatrixTranspose(m_WorldTM);

	for (auto mesh : m_pMeshes)
	{
		mesh->Render(_dc, m_pAlphaBlendState, m_pMaterialCB, m_pTransformCB, test);
	}
	for (auto node : m_pChildren)
		node->Render(_dc, m_pAlphaBlendState, m_pMaterialCB, m_pTransformCB, test);
}

