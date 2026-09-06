#pragma once

// --- INCLUDES ---
#include <Windows.h>

// --- CONSTANTS ---


// --- MACROS & DEFINES ---
#define WINDOW Window::Instance()
#define WINDOWHWND Window::Instance()->GetHwnd()

// --- ENUMS & STRUCTS ---


// --- FORWARD DECLARATIONS ---

class Window
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	~Window();

	// --- CORE FUNCTIONS ---
	bool Initialize(int screenWidth, int screenHeight, LPCWSTR title, bool isFullscreen);
	// void Update();
	// void Render();
	
	// --- VIRTUAL FUNCTIONS ---


	// --- CLASS API ---


	// --- GETTERS & SETTERS ---
	HWND GetHwnd() const { return m_hwnd; }

	// --- STATIC CLASS API ---
	static Window* Instance();

protected:
	// --- CONSTRUCTORS ---


	// --- VIRTUAL FUNCTIONS ---


	// --- PROTECTED FUNCTIONS ---


private:
	// --- CONSTRUCTORS ---
	Window();

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
	bool m_fullscreen{ false };

	// --- PRIVATE COMPONENT DATA ---
	LPCWSTR m_applicationName{ L"" };
	HINSTANCE m_hinstance{ 0 };
	HWND m_hwnd{ 0 };

	// --- STATIC GLOBAL STATES ---
	static Window* ms_instance;
};
