#include "TimerManager.h"

TimerManager* TimerManager::ms_instance = nullptr;

TimerManager::TimerManager()
{
	INT64 frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	if (frequency == 0)
	{
		return;
	}
	m_frequency = frequency;
	QueryPerformanceCounter((LARGE_INTEGER*)&m_previousTime);
}

TimerManager::~TimerManager()
{
}

void TimerManager::CalculateDeltaTime()
{
	INT64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	INT64 elapsedTicks = currentTime - m_previousTime;
	m_deltaTime = (double)elapsedTicks / m_frequency;
	m_previousTime = currentTime;
	if (m_deltaTime > 0.1)
		m_deltaTime = 0.1;
}

TimerManager* TimerManager::Instance()
{
	if (ms_instance == nullptr)
		ms_instance = new TimerManager();
	return ms_instance;
}
