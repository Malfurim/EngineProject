#include "Application.h"

// --- ADDITIONAL INCLUDES ---
#include "Settings.h"
#include "Window.h"
#include "DirectXManager.h"
#include "ResourceManager.h"
#include "TimerManager.h"
#include "InputManager.h"
#include "Camera.h"
#include "UserInterface.h"
#include "DebugWindow.h"

#include "UIPanel.h"
#include "UIImage.h"
#include "UIText.h"
#include "UIButton.h"
#include "UIWindow.h"

#include "Deleters.h"
#include "Utils.h"

#include <iomanip>

// --- MACROS & DEFINES ---
#define STR(str) std::to_wstring(str)
#define STRC(str) std::to_wstring(str).c_str()

// --- FORWARD DECLARATIONS ---


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC GLOBAL STATES & DATA								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
Application* Application::m_instance = nullptr;

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CONSTRUCTORS & DESTRUCTOR								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
Application::Application()
{
}

Application::~Application()
{
	SAFE_DELETE(m_camera);
	
	m_renderMatrix = {};
}

Application::Application(const Application& app)
{
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CORE FUNCTIONS											//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
bool Application::Initialize()
{
	m_camera = new Camera();
	m_camera->SetPosition(CAMERA_START_POSITION_X, CAMERA_START_POSITION_Y, CAMERA_START_POSITION_Z);
	m_camera->SetRotation(CAMERA_START_ROTATION_X, CAMERA_START_ROTATION_Y, CAMERA_START_ROTATION_Z);

	float fieldOfView = 3.141592654f / 4.0f;
	float screenAspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	m_renderMatrix = {
		XMMatrixIdentity(),																				// World Matrix
		XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH),					// Projection Matrix
		XMMatrixOrthographicLH((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH)	// Ortho Matrix
	};
	
	UIWindow* w1 = new UIWindow({ 400, 255 }, { 800, 450 }, L"Main Test Panel", false, true, WINDOWSTYLE(L"default"), WindowDrag::DRAG_FULL);
	UIADD(w1);
	
	UIText* t1 = new UIText({ 0, 0 }, { 800, 450 }, FONT(L"default"));
	t1->SetFontSize(20.0f);
	//t1->SetMaxLength(128);
	t1->SetText(L"My first test\nof TEXT rendering\non multiple lines\nand centered in both\nhorizontal and vertical\npositions...");
	t1->SetFontColor({ 0.0, 0.8f, 0.0f, 1.0f });
	t1->SetCursorColor({ 1.0, 1.0f, 1.0f, 1.0f });
	t1->SetTextAlignment(HORIZONTAL_TEXT_ALIGNMENT_CENTER, VERTICAL_TEXT_ALIGNMENT_MIDDLE);
	t1->SetTextProperties(TEXT_PROPERTY_EDITABLE | TEXT_PROPERTY_MULTILINE);
	w1->AddChild(t1);
	
	//UIImage* i1 = new UIImage({ 800, 100 }, { 128, 128 });
	UIImage* i1 = new UIImage({ 16, 175 }, { 64, 64 });
	i1->SetImage(TEXTURE(L"ui/dirtAnimated"));
	i1->EnableAnimation(4, 1, 2);
	//i1->SetBorderSize(8.0f);
	//m_userInterface->AddChild(i1);
	w1->AddChild(i1);
	
	UIButton* b1 = new UIButton({ 96, 16 }, { 192, 64 });
	b1->SetBackgroundColor({ 0.5f, 0.0f, 0.0f, 1.0f });
	b1->SetBorderColor({ 0.75f, 0.0f, 0.0f, 1.0f });
	b1->SetBorderColor({ 0.0f, 0.75f, 0.0f, 1.0f }, ButtonState::BUTTON_STATE_HOVER);
	b1->SetBorderColor({ 0.0f, 0.0f, 0.75f, 1.0f }, ButtonState::BUTTON_STATE_PRESSED);
	b1->SetBorderSize(2.0f);
	b1->SetBackgroundColor({ 0.0f, 0.5f, 0.0f, 1.0f }, ButtonState::BUTTON_STATE_HOVER);
	b1->SetBackgroundColor({ 0.0f, 0.0f, 0.5f, 1.0f }, ButtonState::BUTTON_STATE_PRESSED);
	b1->SetFont(FONT(L"default"));
	b1->SetFontSize(20.0f);
	b1->SetFontColor({ 0.8f, 0.8f, 0.8f, 1.0f });
	b1->EnableRepeatable();
	w1->AddChild(b1);

	m_testIMG = new UIImage({ 16, 16 }, { 64, 64 });
	m_testIMG->SetImage(TEXTURE2(L"ui/loam", AssetPool::ASSET_DYNAMIC));
	w1->AddChild(m_testIMG);

	static int Number{ 0 };
	//b1->OnClick += [](UIButton* sender) { Number++; sender->SetText(STR(Number)); };
	b1->OnClick += [&](UIButton* sender) {
		switch (Number)
		{
		case 0:
			m_testIMG->SetImage(TEXTURE2(L"ui/sandy_loam", AssetPool::ASSET_DYNAMIC));
			sender->SetText(STR(Number));
			Number++;
			return;
		case 1:
			m_testIMG->SetImage(TEXTURE2(L"ui/silt", AssetPool::ASSET_DYNAMIC));
			sender->SetText(STR(Number));
			Number++;
			return;
		case 2:
			m_testIMG->SetImage(TEXTURE2(L"ui/silty_loam", AssetPool::ASSET_DYNAMIC));
			sender->SetText(STR(Number));
			Number++;
			return;
		case 3:
			m_testIMG->SetImage(TEXTURE2(L"ui/loam", AssetPool::ASSET_DYNAMIC));
			sender->SetText(STR(Number));
			Number = 0;
			return;
		}
		};
	b1->SetText(STR(Number));

	UIWindow* w2 = new UIWindow({ 700, 30 }, {200, 100}, L"Test Window", true, true, WINDOWSTYLE(L"default"), WindowDrag::DRAG_TITLE);
	UIADD(w2);
	UIWindow* w3 = new UIWindow({ 950, 30 }, { 200, 100 }, L"Test Window 2", true, true, WINDOWSTYLE(L"default"), WindowDrag::DRAG_TITLE);
	UIADD(w3);
	UIWindow* w4 = new UIWindow({ 700, 150 }, { 200, 100 }, L"Test Window 3 static", true, true, WINDOWSTYLE(L"default"), WindowDrag::DRAG_TITLE);
	UIADD2(w4, WindowType::WINDOW_STATIC);
	UIWindow* w5 = new UIWindow({ 950, 150 }, { 200, 100 }, L"Test Window 4 front", true, true, WINDOWSTYLE(L"default"), WindowDrag::DRAG_TITLE);
	UIADD2(w5, WindowType::WINDOW_FORCEDFRONT);

	t1 = nullptr;
	i1 = nullptr;
	b1 = nullptr;
	w1 = nullptr;
	w2 = nullptr;
	w3 = nullptr;
	w4 = nullptr;
	w5 = nullptr;

	UIADD2(DEBUG, WindowType::WINDOW_FORCEDFRONT);
	DEBUG->SetGridSize(3, 1);

	return true;
}

void Application::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	ShowWindow(WINDOWHWND, SW_SHOW);
	SetForegroundWindow(WINDOWHWND);
	SetFocus(WINDOWHWND);

	// Update loop
	while (msg.message != WM_QUIT)
	{
		INPUTUPDATE();
		CALCDELTATIME();

		while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message != WM_QUIT)
		{
			// To quit application (in future replaced by menu button ingame)
			if (ISKEYPRESSED(VK_ESCAPE))
				PostQuitMessage(0);

			Update();
			Render();
		}
	}
}

void Application::Shutdown()
{
	if (m_instance)
	{
		delete m_instance;
		m_instance = nullptr;
	}
	
	UIText::Shutdown();
	UIImage::Shutdown();
	GraphicBase::Shutdown();
	//delete ENTITY;
	delete INP;
	delete TIMER;
	delete UI;
	delete RESOURCE;
	delete DXMANAGER;
	delete WINDOW;
}

void Application::Update()
{
	float delta = (float)DELTA;

	if (ISKEYPRESSED(VK_F4))
	{
		DEBUG->Visibility = !DEBUG->Visibility;
	}

	if (ISMOUSEPRESSED(MouseButton::MOUSE_BUTTON_LEFT))
	{
		UI->MouseEvent(MouseButton::MOUSE_BUTTON_LEFT, InputEventType::INPUT_EVENT_CLICK);
	}
	
	if (ISMOUSERELEASED(MouseButton::MOUSE_BUTTON_LEFT))
	{
		UI->MouseEvent(MouseButton::MOUSE_BUTTON_LEFT, InputEventType::INPUT_EVENT_RELEASE);
	}


	XMFLOAT3 camera = m_camera->GetPosition();
	XMFLOAT3 camRot = m_camera->GetRotation();
	bool change = false;
	bool change2 = false;

	// Speed per second
	float speed = 9.0f * delta;
	float speed2 = 60.0f * delta;
	float speed3 = 120.0f * delta;
	
	XMFLOAT3 position = m_camera->GetPosition();
	XMFLOAT3 rotation = m_camera->GetRotation();
	XMFLOAT3 lookAt = { 0.0f, 0.0f, 1.0f };
	XMFLOAT3 lookRight = { 1.0f, 0.0f, 0.0f };
	XMFLOAT3 lookUp = { 0.0f, 1.0f, 0.0f };
	XMFLOAT3 cameraFront{};
	XMFLOAT3 cameraRight{};
	XMFLOAT3 cameraUp{};

	XMVECTOR positionVector = XMLoadFloat3(&position);
	XMVECTOR lookAtVector = XMLoadFloat3(&lookAt);
	XMVECTOR lookRightVector = XMLoadFloat3(&lookRight);
	XMVECTOR lookUpVector = XMLoadFloat3(&lookUp);

	// PI / 180.0f
	float oneDegree = 0.0174532925f;
	float yaw = rotation.x * oneDegree;
	float pitch = rotation.y * oneDegree;
	float roll = rotation.z * oneDegree;

	// Create the rotation matrix from the yaw, pitch and roll values
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	lookRightVector = XMVector3TransformCoord(lookRightVector, rotationMatrix);
	lookUpVector = XMVector3TransformCoord(lookUpVector, rotationMatrix);

	XMStoreFloat3(&cameraFront, lookAtVector);
	XMStoreFloat3(&cameraRight, lookRightVector);
	XMStoreFloat3(&cameraUp, lookUpVector);

	if (INP->IsStateGameplay())
	{
		if (ISKEYDOWN(VK_SHIFT))
		{
			// Speed per second
			speed = 24.0f * delta;
			speed2 = 150.0f * delta;
			speed3 = 240.0f * delta;
		}

		if (ISKEYDOWN('W'))
		{
			camera = { camera.x + cameraFront.x * speed, camera.y + cameraFront.y * speed, camera.z + cameraFront.z * speed };
			change = true;
		}

		if (ISKEYDOWN('A'))
		{
			camera = { camera.x - cameraRight.x * speed, camera.y - cameraRight.y * speed, camera.z - cameraRight.z * speed };
			change = true;
		}
		if (ISKEYDOWN('S'))
		{
			camera = { camera.x - cameraFront.x * speed, camera.y - cameraFront.y * speed, camera.z - cameraFront.z * speed };
			change = true;
		}
		if (ISKEYDOWN('D'))
		{
			camera = { camera.x + cameraRight.x * speed, camera.y + cameraRight.y * speed, camera.z + cameraRight.z * speed };
			change = true;
		}
		if (ISKEYDOWN(VK_SPACE))
		{
			camera = { camera.x + cameraUp.x * speed, camera.y + cameraUp.y * speed, camera.z + cameraUp.z * speed };
			change = true;
		}
		if (ISKEYDOWN('C'))
		{
			camera = { camera.x - cameraUp.x * speed, camera.y - cameraUp.y * speed, camera.z - cameraUp.z * speed };
			change = true;
		}
		if (ISKEYDOWN('J'))
		{
			camRot.x -= speed2;
			if (camRot.x < -360.0f)
				camRot.x += 360.0f;
			change2 = true;
		}
		if (ISKEYDOWN('L'))
		{
			camRot.x += speed2;
			if (camRot.x > 360.0f)
				camRot.x -= 360.0f;
			change2 = true;
		}
		if (ISKEYDOWN('I'))
		{
			camRot.y -= speed2;
			if (camRot.y < -90.0f)
				camRot.y = -90.0f;
			change2 = true;
		}
		if (ISKEYDOWN('K'))
		{
			camRot.y += speed2;
			if (camRot.y > 90.0f)
				camRot.y = 90.0f;
			change2 = true;
		}

		if (ISKEYDOWN('Y'))
		{

		}
		if (ISKEYDOWN('X'))
		{

		}

		/*
		if (ISKEYDOWN('U'))
		{
			camRot.z += speed2;
			change2 = true;
		}
		if (ISKEYDOWN('O'))
		{
			camRot.z -= speed2;
			change2 = true;
		}
		*/

		if (change)
			m_camera->SetPosition(camera.x, camera.y, camera.z);
		if (change2)
			m_camera->SetRotation(camRot.x, camRot.y, camRot.z);
	}

	std::wstring text = L"delta: " + ToWStringWithPrecision(delta, 5) + L"s\n" +
		L"Mouse: X " + ToWStringWithPrecision(MOUSEPOSITION.X, 0) + L", Y " + ToWStringWithPrecision(MOUSEPOSITION.Y, 0) + L"\n" +
		L"Camera Position:\n" +
		L"X " + ToWStringWithPrecision(m_camera->GetPosition().x, 3) + L", Y " + ToWStringWithPrecision(m_camera->GetPosition().y, 3) + L", Z " + ToWStringWithPrecision(m_camera->GetPosition().z, 3) + L"\n" +
		L"Camera Rotation:\n" +
		L"X " + ToWStringWithPrecision(m_camera->GetRotation().x, 3) + L", Y " + ToWStringWithPrecision(m_camera->GetRotation().y, 3) + L", Z " + ToWStringWithPrecision(m_camera->GetRotation().z, 3) + L"\n" +
		L"InputState: " + std::to_wstring(static_cast<unsigned int>(INP->GetInputState())).c_str();

	DEBUG->SetText(0, 0, text);

	UI->Update();
	RESOURCEUPDATE;

	std::wstring textureText = L"--- STATIC TEXTURES ---\n";
	for (auto& texture : RESOURCE->GetStaticTextureMap())
	{
		textureText += L"- " + texture.first + L"\n";
	}
	textureText += L"\n--- DYNAMIC TEXTURES ---\n";
	for (auto& texture : RESOURCE->GetDynamicTextureMap())
	{
		textureText += L"- " + texture.first + L" (Ref: " + std::to_wstring(texture.second.referenceCount) +
			L", Time: " + ToWStringWithPrecision(texture.second.timeSinceUnused, 1) + L"s)\n";
	}
	textureText += L"\n--- UNUSED TEXTURES ---\n";
	for (auto& texture : RESOURCE->GetUnusedTexturesVector())
	{
		textureText += L"- " + texture + L"\n";
	}
	textureText += L"\n--- LOADED FONTS ---\n";
	for (auto& font : RESOURCE->GetFontMap())
	{
		textureText += L"- " + font.first + L"\n";
	}
	DEBUG->SetText(2, 0, textureText);
}

void Application::Render()
{
	DXMANAGER->BeginScene(0.6875f, 0.765625f, 0.8671875f, 1.0f);

	XMMATRIX viewMatrix;
	XMMATRIX worldMatrix;
	XMMATRIX projectionMatrix;
	XMMATRIX orthoMatrix;
	XMMATRIX translateMatrix;

	m_camera->GetViewMatrix(viewMatrix);
	worldMatrix = m_renderMatrix.worldMatrix;
	projectionMatrix = m_renderMatrix.projectionMatrix;
	orthoMatrix = m_renderMatrix.orthoMatrix;

	/*
	for (int i = 0; i < 256; i++)
	{
		translateMatrix = XMMatrixTranslation((i % 16), 0, ((i / 16) % 16));
		worldMatrix = XMMatrixMultiply(XMMatrixIdentity(), translateMatrix);
		ENTITY->GetEntity(0)->Render(DXDEVICECONTEXT, worldMatrix, viewMatrix, projectionMatrix);
	}
	*/

	DXMANAGER->EnableZBuffer(false);
	DXMANAGER->EnableBlendState(true);

	translateMatrix = XMMatrixTranslation(0, 0, 1);
	viewMatrix = XMMatrixMultiply(XMMatrixIdentity(), XMMatrixTranslation(0.0f, 0.0f, 1.0f));
	worldMatrix = XMMatrixIdentity();

	UI->Render();

	DXMANAGER->EnableBlendState(false);
	DXMANAGER->EnableZBuffer(true);



	DXMANAGER->EndScene();
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	VIRTUAL FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CLASS API												//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	GETTERS & SETTERS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC CLASS API										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
Application* Application::Instance()
{
	if (m_instance == nullptr)
		m_instance = new Application();
	return m_instance;
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	VIRTUAL FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PRIVATE FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //

