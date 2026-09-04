#pragma once

#include "Window.h"
#include "DirectXManager.h"

class Graphics
{
public:
	Graphics();
	~Graphics();

	bool Initialize(int screenWidth, int screenHeight, LPCWSTR title, bool isFullscreen, bool vsyncEnabled, float screenDepth, float screenNear);
	/*
	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();

	HWND GetHwnd();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void EnableZBuffer(bool enable);
	*/
private:
	//Window* m_window{ nullptr };
	//DirectXManager* m_directXManager{ nullptr };
};

