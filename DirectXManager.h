#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <DirectXMath.h>
#include "Settings.h"

#include "Singleton.h"

using namespace DirectX;

#define DXMANAGER DirectXManager::Instance()
#define DXDEVICE DirectXManager::Instance()->GetDevice()
#define DXDEVICECONTEXT DirectXManager::Instance()->GetDeviceContext()

class DirectXManager
{
public:
	~DirectXManager();

	bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool isFullscreen, float screenDepth, float screenNear);
	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetVideoCardInfo(char* cardName, int& memory);

	void SetBackBufferRenderTarget();
	void ResetViewPort();

	void EnableZBuffer(bool enable);
	void EnableBlendState(bool enable);

	static DirectXManager* Instance();

private:
	DirectXManager();

	bool m_vsync_enabled{ false };
	int m_videoCardMemory{ 0 };
	char m_videoCardDescription[128]{};
	IDXGISwapChain* m_swapChain{ nullptr };
	ID3D11Device* m_device{ nullptr };
	ID3D11DeviceContext* m_deviceContext{ nullptr };
	ID3D11RenderTargetView* m_renderTargetView{ nullptr };
	ID3D11Texture2D* m_depthStencilBuffer{ nullptr };
	ID3D11DepthStencilState* m_depthStencilState{ nullptr };
	ID3D11DepthStencilState* m_depthDisabledStencilState{ nullptr };
	ID3D11DepthStencilView* m_depthStencilView{ nullptr };
	ID3D11RasterizerState* m_rasterState{ nullptr };
	ID3D11BlendState* m_alphaEnabledBlendingState{ nullptr };
	ID3D11BlendState* m_alphaDisabledBlendingState{ nullptr };
	//XMMATRIX m_projectionMatrix;
	//XMMATRIX m_worldMatrix;
	//XMMATRIX m_orthoMatrix;
	D3D11_VIEWPORT m_viewport{ NULL };

	static DirectXManager* ms_instance;
};

