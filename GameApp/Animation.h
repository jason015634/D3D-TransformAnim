#pragma once

struct AnimationKey
{
	float FrameTime;
	Vector3 FramePos;
	Vector3 FrameScale;
	Quaternion FrameRot;
};

struct NodeAnimation
{
	string NodeName;
	vector<AnimationKey> AnimationKeys;
	int CurrentKey = 0;
	int NumKey = 0;
	float CurrentTime = 0;
	float Ratio = 0;
};
struct aiNodeAnim;
struct aiAnimation;
class Model;
class Node;
class Animation
{
public:
	
	Node* m_pConnectNode = nullptr;

	Animation(Model* _pOwner)
		:m_pOwner(_pOwner) { }
	Model* m_pOwner;

	void Update();
	vector<AnimationKey> Create(aiNodeAnim* nodeAnimation);
	vector<NodeAnimation> NodeAnimations;
	string AnimationName;
	float AnimationDuration;
	float AnimationSpeed;
	float m_tickPerSecond;
private:
	int m_curKey = 0;
	float m_timer = 0.f;

};