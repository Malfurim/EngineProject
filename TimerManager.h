#pragma once

// --- INCLUDES ---
#include <cstdint>

// --- CONSTANTS ---


// --- MACROS & DEFINES ---
#define TIMER TimerManager::Instance()
#define CALCDELTATIME() TimerManager::Instance()->CalculateDeltaTime()
#define DELTA TimerManager::Instance()->GetTime()

// --- ENUMS & STRUCTS ---


// --- FORWARD DECLARATIONS ---


class TimerManager
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	~TimerManager();

	// --- CORE FUNCTIONS ---
	// bool Initialize();
	// void Update();
	// void Render();

	// --- VIRTUAL FUNCTIONS ---


	// --- CLASS API ---
	void CalculateDeltaTime();

	// --- GETTERS & SETTERS ---
	double GetTime() const { return m_deltaTime; }

	// --- STATIC CLASS API ---
	static TimerManager* Instance();

protected:
	// --- CONSTRUCTORS ---


	// --- VIRTUAL FUNCTIONS ---


	// --- PROTECTED FUNCTIONS ---


private:
	// --- CONSTRUCTORS ---
	TimerManager();

	// --- PRIVATE FUNCTIONS ---


// - PROPERTIES -
public:
	// --- PUBLIC COMPONENT STATES ---


	// --- PUBLIC COMPONENT DATA ---


protected:
	// --- INTERNAL CONSTANTS ---


	// --- INTERNAL STRUCTS & ENUMS ---


	// --- INTERNAL COMPONENT STATES ---


	// --- INTERNAL COMPONENT DATA ---


private:
	// --- PRIVATE CONSTANTS ---


	// --- PRIVATE STRUCTS & ENUMS ---


	// --- PRIVATE COMPONENT STATES ---


	// --- PRIVATE COMPONENT DATA ---
	double m_frequency{ 0.0 };
	int64_t m_previousTime{ 0 };
	double m_deltaTime{ 0.0 };

	// --- STATIC GLOBAL STATES ---
	static TimerManager* ms_instance;
};

