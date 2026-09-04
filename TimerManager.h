#pragma once

#include <Windows.h>

#define TIMER TimerManager::Instance()
#define CALCDELTATIME() TimerManager::Instance()->CalculateDeltaTime()
#define DELTA TimerManager::Instance()->GetTime()

class TimerManager
{
public:
	~TimerManager();

	void CalculateDeltaTime();
	double GetTime() const { return m_deltaTime; }

	static TimerManager* Instance();

private:
	TimerManager();

	double m_frequency{ 0.0 };
	INT64 m_previousTime{ 0 };
	double m_deltaTime{ 0.0 };

	static TimerManager* ms_instance;
};

