#include "pch.h"
#include "TimeSystem.h"

void TimeSystem::Update()
{
	m_previousTime = m_currentTime;
	QueryPerformanceCounter(&m_currentTime);
	m_deltaTime = (float)(m_currentTime.QuadPart - m_previousTime.QuadPart) / (float)(m_frequency.QuadPart);
}

TimeSystem::TimeSystem()
{
	QueryPerformanceFrequency(&m_frequency);
	QueryPerformanceCounter(&m_previousTime);
	QueryPerformanceCounter(&m_currentTime);
}
