#pragma once

// --- INCLUDES ---
#include <DirectXMath.h>

// --- CONSTANTS ---


// --- MACROS & DEFINES ---
#define APPLICATION Application::Instance()

// --- ENUMS & STRUCTS ---


// --- FORWARD DECLARATIONS ---
class Camera;
class UIPanel;
class UIText;
using namespace DirectX;

class Application
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	~Application();
	Application(const Application& app);

	// --- CORE FUNCTIONS ---
	bool Initialize();
	void Run();
	void Shutdown();
	void Update();
	void Render();

	// --- VIRTUAL FUNCTIONS ---


	// --- CLASS API ---


	// --- GETTERS & SETTERS ---


	// --- STATIC CLASS API ---
	static Application* Instance();

protected:
	// --- CONSTRUCTORS ---


	// --- VIRTUAL FUNCTIONS ---


	// --- PROTECTED FUNCTIONS ---


private:
	// --- CONSTRUCTORS ---


	// --- PRIVATE FUNCTIONS ---
	Application();

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
	struct RenderMatrix
	{
		XMMATRIX worldMatrix;
		XMMATRIX projectionMatrix;
		XMMATRIX orthoMatrix;
	};

	// --- PRIVATE COMPONENT STATES ---


	// --- PRIVATE COMPONENT DATA ---
	RenderMatrix m_renderMatrix{};
	Camera* m_camera{ nullptr };

	UIPanel* m_userInterface{ nullptr };
	UIPanel* m_debug{ nullptr };
	UIText* m_debugText{ nullptr };

	float xx{ 0.0f };

	// --- STATIC GLOBAL STATES ---
	static Application* m_instance;
};