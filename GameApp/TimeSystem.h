#pragma once
#include <memory>
#include <Windows.h>
class TimeSystem
{
public:
	static TimeSystem* GetInst()
	{
		static std::unique_ptr<TimeSystem> timer = std::make_unique<TimeSystem>();
		return timer.get();
	}
	float GetDeltaTime()
	{
		return m_deltaTime;
	}
	void Update();

	TimeSystem();
private:


	LARGE_INTEGER m_currentTime;
	LARGE_INTEGER m_previousTime;
	LARGE_INTEGER m_frequency;
	float m_deltaTime = 0.f;
	float m_curTime = 0.f;
};

