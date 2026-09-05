#include "Application.h"
#include "Settings.h"
#include "Window.h"
#include "DirectXManager.h"
#include "ResourceManager.h"
#include "TimerManager.h"
#include "InputManager.h"
#include "Camera.h"

#include "UIPanel.h"
#include "UIImage.h"
#include "UIText.h"

#include "Deleters.h"
#include "Utils.h"

#include <iomanip>

#define STR(str) std::to_wstring(str)
#define STRC(str) std::to_wstring(str).c_str()

Application* Application::m_instance = nullptr;

Application::Application()
{
}

Application::~Application()
{
	SAFE_DELETE(m_camera);
	SAFE_DELETE(m_userInterface);
	
	m_renderMatrix = {};
}

Application::Application(const Application& app)
{
}

bool Application::Initialize()
{
	RESOURCE->LoadAssets();
	
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

	m_userInterface = new UIPanel({0, 0}, {SCREEN_WIDTH, SCREEN_HEIGHT});
	//m_userInterface->SetBackgroundColor({0.0f, 0.0f, 0.0f, 0.25f});
	
	UIPanel* p1 = new UIPanel({ 560, 315 }, { 480, 270 }, { 0.5f, 0.0f, 0.0f, 1.0f });
	m_userInterface->AddChild(p1);

	UIText* t1 = new UIText({ 0, 0 }, { 480, 270 }, RESOURCE->GetFont(L"font01"));
	t1->SetFontSize(20.0f);
	t1->SetText(L"My first test\nof TEXT rendering\non multiple lines\nand centered in both\nhorizontal and vertical\npositions...");
	t1->SetFontColor({ 0.0, 0.8f, 0.0f, 1.0f });
	t1->SetTextAlignment(HORIZONTAL_TEXT_ALIGNMENT_CENTER, VERTICAL_TEXT_ALIGNMENT_MIDDLE);
	t1->SetTextProperties(TEXT_PROPERTY_EDITABLE);
	p1->AddChild(t1);

	p1 = nullptr;
	t1 = nullptr;

	m_debug = new UIPanel({ 8, 8 }, { 256, 384 }, { 0.0f, 0.0f, 0.0f, 0.35f });
	m_userInterface->AddChild(m_debug);
	m_debugText = new UIText({ 4, 4 }, { m_debug->GetWidth() - 8, m_debug->GetHeight() - 8 }, RESOURCE->GetFont(L"font01"));
	m_debugText->SetFontColor({ 1.0, 1.0f, 1.0f, 1.0f });
	m_debugText->SetFontSize(16.0f);
	m_debug->AddChild(m_debugText);

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
	delete RESOURCE;
	delete DXMANAGER;
	delete WINDOW;
}

void Application::Update()
{
	float delta = (float)DELTA;

	if (ISMOUSEPRESSED(MouseButton::MOUSE_BUTTON_LEFT))
	{
		UIElement* element = m_userInterface->GetElementAtPosition(MOUSEX, MOUSEY);
		if (element != nullptr)
		{
			element->HandleLeftClick();
			xx = element->GetPositionLeft();
		}
		else
		{
			Interactive::RemoveFocus(FocusType::FOCUS_TYPE_UI);
			/*
			if (GraphicBase::GetFocus() != nullptr)
			{
				GraphicBase::GetFocus()->Unfocus();
			}
			*/
		}
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

		if (ISKEYDOWN('T'))
		{
			m_userInterface->GetChild(0)->SetPosition(m_userInterface->GetChild(0)->GetPositionLeft(), m_userInterface->GetChild(0)->GetPositionTop() - speed3);
		}
		if (ISKEYDOWN('F'))
		{
			m_userInterface->GetChild(0)->SetPosition(m_userInterface->GetChild(0)->GetPositionLeft() - speed3, m_userInterface->GetChild(0)->GetPositionTop());
		}
		if (ISKEYDOWN('G'))
		{
			m_userInterface->GetChild(0)->SetPosition(m_userInterface->GetChild(0)->GetPositionLeft(), m_userInterface->GetChild(0)->GetPositionTop() + speed3);
		}
		if (ISKEYDOWN('H'))
		{
			m_userInterface->GetChild(0)->SetPosition(m_userInterface->GetChild(0)->GetPositionLeft() + speed3, m_userInterface->GetChild(0)->GetPositionTop());
		}

		if (ISKEYDOWN('V'))
		{
			m_userInterface->GetChild(0)->SetSize(m_userInterface->GetChild(0)->GetSize().Width - speed3, m_userInterface->GetChild(0)->GetSize().Height);
		}
		if (ISKEYDOWN('B'))
		{
			m_userInterface->GetChild(0)->SetSize(m_userInterface->GetChild(0)->GetSize().Width + speed3, m_userInterface->GetChild(0)->GetSize().Height);
		}
		if (ISKEYDOWN('N'))
		{
			m_userInterface->GetChild(0)->SetSize(m_userInterface->GetChild(0)->GetSize().Width, m_userInterface->GetChild(0)->GetSize().Height - speed3);
		}
		if (ISKEYDOWN('M'))
		{
			m_userInterface->GetChild(0)->SetSize(m_userInterface->GetChild(0)->GetSize().Width, m_userInterface->GetChild(0)->GetSize().Height + speed3);
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

	std::wstring text = L"delta: " + ToWStringWithPrecision(delta, 5) + L"\n" +
		L"Mouse: X " + ToWStringWithPrecision(MOUSEPOSITION.X, 0) + L", Y " + ToWStringWithPrecision(MOUSEPOSITION.Y, 0) + L"\n" +
		L"Camera Position:\n" +
		L"X " + ToWStringWithPrecision(m_camera->GetPosition().x, 3) + L", Y " + ToWStringWithPrecision(m_camera->GetPosition().y, 3) + L", Z " + ToWStringWithPrecision(m_camera->GetPosition().z, 3) + L"\n" +
		L"Camera Rotation:\n" +
		L"X " + ToWStringWithPrecision(m_camera->GetRotation().x, 3) + L", Y " + ToWStringWithPrecision(m_camera->GetRotation().y, 3) + L", Z " + ToWStringWithPrecision(m_camera->GetRotation().z, 3) + L"\n" +
		L"" + ToWStringWithPrecision(xx, 3) + L"\n" +
		L"InputState: " + std::to_wstring(static_cast<unsigned int>(INP->GetInputState())).c_str();

	m_debugText->SetText(text);

	m_userInterface->Update();
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

	m_userInterface->Render();

	DXMANAGER->EnableBlendState(false);
	DXMANAGER->EnableZBuffer(true);



	DXMANAGER->EndScene();
}

Application* Application::Instance()
{
	if (m_instance == nullptr)
		m_instance = new Application();
	return m_instance;
}