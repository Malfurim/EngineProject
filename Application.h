#pragma once

// --- INCLUDES ---
#include "DXMath.h"

// --- CONSTANTS ---


// --- MACROS & DEFINES ---
#define APPLICATION Application::Instance()

// --- ENUMS & STRUCTS ---


// --- FORWARD DECLARATIONS ---
class Camera;
class UIPanel;
class UIText;
class UIImage;

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
	void UpdateInput();
	void UpdateUI();
	void Update3D();
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
		DXMATRIX worldMatrix;
		DXMATRIX projectionMatrix;
		DXMATRIX orthoMatrix;
	};

	// --- PRIVATE COMPONENT STATES ---
	int m_fps{ 0 };
	int m_ups{ 0 };

	// --- PRIVATE COMPONENT DATA ---
	RenderMatrix m_renderMatrix{};
	Camera* m_camera{ nullptr };

	UIImage* m_testIMG{ nullptr };

	// --- STATIC GLOBAL STATES ---
	static Application* m_instance;
};