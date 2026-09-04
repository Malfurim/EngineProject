#include "Graphics.h"

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

bool Graphics::Initialize(int screenWidth, int screenHeight, LPCWSTR title, bool isFullscreen, bool vsyncEnabled, float screenDepth, float screenNear)
{
	if (!GAMEWINDOW->Initialize(screenWidth, screenHeight, title, isFullscreen))
	{
		MessageBox(NULL, L"Failed to initialize Win32 window", L"ERROR", MB_OK);
		return false;
	}
	
	if (!DXMANAGER->Initialize(screenWidth, screenHeight, vsyncEnabled, WINDOWHWND, isFullscreen, screenDepth, screenNear))
	{
		MessageBox(NULL, L"Failed to initialize DirectX", L"ERROR", MB_OK);
		return false;
	}
	/*
	m_directXManager = new DirectXManager();
	if (!m_directXManager->Initialize(screenWidth, screenHeight, vsyncEnabled, GetHwnd(), isFullscreen, screenDepth, screenNear))
	{
		MessageBox(NULL, L"Failed to initialize DirectX", L"ERROR", MB_OK);
		return false;
	}
	*/
	return true;
}
/*
void Graphics::BeginScene(float red, float green, float blue, float alpha)
{
	m_directXManager->BeginScene(red, green, blue, alpha);
}

void Graphics::EndScene()
{
	m_directXManager->EndScene();
}

HWND Graphics::GetHwnd()
{
	return m_window->GetHwnd();
}

ID3D11Device* Graphics::GetDevice()
{
	return m_directXManager->GetDevice();
}

ID3D11DeviceContext* Graphics::GetDeviceContext()
{
	return m_directXManager->GetDeviceContext();
}

void Graphics::EnableZBuffer(bool enable)
{
	m_directXManager->EnableZBuffer(enable);
}
*/