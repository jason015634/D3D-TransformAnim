#include "pch.h"
#include "Animation.h"
#include "Model.h"
#include "Node.h"
#include "TimeSystem.h"


void Animation::Update()
{

}

vector<AnimationKey> Animation::Create(aiNodeAnim* nodeAnimation)
{
	assert(nodeAnimation != nullptr);
	assert(nodeAnimation->mNumPositionKeys == nodeAnimation->mNumRotationKeys);
	assert(nodeAnimation->mNumRotationKeys == nodeAnimation->mNumScalingKeys);

	vector<AnimationKey> Keys;
	NodeAnimation nodeAnim;
	for (UINT i = 0; i < nodeAnimation->mNumPositionKeys; ++i)
	{
		AnimationKey Key;
		Key.FramePos.x = nodeAnimation->mPositionKeys[i].mValue.x;
		Key.FramePos.y = nodeAnimation->mPositionKeys[i].mValue.y;
		Key.FramePos.z = nodeAnimation->mPositionKeys[i].mValue.z;

		Key.FrameScale.x = nodeAnimation->mScalingKeys[i].mValue.x;
		Key.FrameScale.y = nodeAnimation->mScalingKeys[i].mValue.y;
		Key.FrameScale.z = nodeAnimation->mScalingKeys[i].mValue.z;

		Key.FrameRot.x = nodeAnimation->mRotationKeys[i].mValue.x;
		Key.FrameRot.y = nodeAnimation->mRotationKeys[i].mValue.y;
		Key.FrameRot.z = nodeAnimation->mRotationKeys[i].mValue.z;
		Key.FrameRot.w = nodeAnimation->mRotationKeys[i].mValue.w;

		Key.FrameTime = nodeAnimation->mPositionKeys[i].mTime / m_tickPerSecond;

		Keys.push_back(Key);
	}

	nodeAnim.NodeName = nodeAnimation->mNodeName.C_Str();

	return Keys;
}
