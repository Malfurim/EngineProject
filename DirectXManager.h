#pragma once

// --- INCLUDES ---
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>

// --- CONSTANTS ---


// --- MACROS & DEFINES ---
#define DXMANAGER DirectXManager::Instance()
#define DXDEVICE DirectXManager::Instance()->GetDevice()
#define DXDEVICECONTEXT DirectXManager::Instance()->GetDeviceContext()

// --- ENUMS & FLAGS ---


// --- FORWARD DECLARATIONS ---


class DirectXManager
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	~DirectXManager();

	// --- VIRTUAL FUNCTIONS ---
	virtual bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool isFullscreen, float screenDepth, float screenNear);
	// virtual void Update();
	// virtual void Render();

	// --- CLASS API ---
	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();

	void ResetViewPort();

	void EnableZBuffer(bool enable);
	void EnableBlendState(bool enable);

	// --- GETTERS & SETTERS ---
	ID3D11Device* GetDevice() const { return m_device; }
	ID3D11DeviceContext* GetDeviceContext() const { return m_deviceContext; }

	void GetVideoCardInfo(char* cardName, int& memory);

	void SetBackBufferRenderTarget();

	// --- STATIC CLASS API ---
	static DirectXManager* Instance();

protected:
	// --- CONSTRUCTORS ---


	// --- VIRTUAL FUNCTIONS ---


	// --- PROTECTED FUNCTIONS ---


private:
	// --- CONSTRUCTORS ---
	DirectXManager();

	// --- PRIVATE FUNCTIONS ---


// - PROPERTIES -
public:
	// --- PUBLIC COMPONENT STATES ---


	// --- PUBLIC COMPONENT DATA ---


protected:
	// --- INTERNAL COMPONENT STATES ---


	// --- INTERNAL COMPONENT DATA ---


private:
	// --- PRIVATE COMPONENT STATES ---
	bool m_vsync_enabled{ false };

	// --- PRIVATE COMPONENT DATA ---
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

	// --- STATIC GLOBAL STATES ---
	static DirectXManager* ms_instance;
};

