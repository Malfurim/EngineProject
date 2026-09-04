#pragma once

#include <Windows.h>
#include <Windowsx.h>

#define WINDOW Window::Instance()
#define WINDOWHWND Window::Instance()->GetHwnd()

class Window
{
public:
	~Window();

	bool Initialize(int screenWidth, int screenHeight, LPCWSTR title, bool isFullscreen);
	HWND GetHwnd();

	static Window* Instance();

private:
	Window();

	bool m_fullscreen{ false };
	LPCWSTR m_applicationName{ L"" };
	HINSTANCE m_hinstance{ 0 };
	HWND m_hwnd{ 0 };

	static Window* ms_instance;
};
