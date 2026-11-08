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

#include "WorldManager.h"
#include "WorldObject.h"

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

	RESOURCE->LoadUIShaders();
	RESOURCE->LoadWorldShaders();

	WorldObject::SetProjectionMatrix(m_renderMatrix.projectionMatrix);
	//m_testObject = WorldObject::Create(MESH(L"cubeNew"), MATERIAL(L"test"));
	WorldObjectDesc desc;
	desc.mesh = MESH(L"cubeNew");
	desc.material = MATERIAL(L"test");
	WORLD->CreateObject(desc);
	desc.position = { 1.0f, 0.0f, 0.0f };
	WORLD->CreateObject(desc);
	desc.position = { 2.0f, 0.0f, 0.0f };
	WORLD->CreateObject(desc);
	desc.position = { 1.0f, 1.0f, 0.0f };
	WORLD->CreateObject(desc);

	desc.position = { 0.0f, 2.0f, 2.0f };
	m_testObject = WORLD->CreateObject(desc);

	UIWindow* w1 = new UIWindow({ 1192, 8 }, { 400, 225 }, L"Main Test Panel", true, WindowCloseButton::CLOSEBUTTON_NONE, WINDOWSTYLE(L"default"), WindowDrag::DRAG_FULL);
	UIADD(w1);

	w1 = nullptr;

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

	double updateUIDelta = 0.0;
	double update3DDelta = 0.0;
	double renderDelta = 0.0;
	double secondDelta = 0.0;
	int fps = 0;
	int ups = 0;
	int upsUI = 0;
	int ups3D = 0;

	// Update loop
	while (msg.message != WM_QUIT)
	{
		INPUTUPDATE();
		CALCDELTATIME();

		updateUIDelta += DELTA;
		update3DDelta += DELTA;
		renderDelta += DELTA;
		secondDelta += DELTA;

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

			UpdateInput();
			ups++;

			if (updateUIDelta >= (REFRESHRATE / 4.0))
			{
				UpdateUI();
				INP->ResetMouseDelta();
				TIMER->ResetTimeUI();
				upsUI++;
				updateUIDelta -= (REFRESHRATE / 4.0);
			}

			if (update3DDelta >= REFRESHRATE)
			{
				Update3D();
				TIMER->ResetTimeWorld();
				ups3D++;
				update3DDelta -= REFRESHRATE;
			}

			if (renderDelta >= REFRESHRATE)
			{
				Render();
				fps++;
				renderDelta -= REFRESHRATE;
			}

			if (secondDelta >= 1.0)
			{
				m_fps = fps;
				m_ups = ups;

				DEBUG->SetText(1, 0, L"");
				DEBUG->AddText(1, 0, L"--- Debug ---");
				DEBUG->AddText(1, 0, L"\nFPS: " + STR(m_fps));
				DEBUG->AddText(1, 0, L"\nUI: " + STR(upsUI));
				DEBUG->AddText(1, 0, L"\n3D: " + STR(ups3D));
				DEBUG->AddText(1, 0, L"\nUPS: " + STR(m_ups));

				fps = 0;
				ups = 0;
				upsUI = 0;
				ups3D = 0;
				secondDelta -= 1.0;
			}
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

	//delete ENTITY;
	delete INP;
	delete TIMER;
	delete WORLD;
	delete UI;
	delete RESOURCE;
	delete DXMANAGER;
	delete WINDOW;
}

void Application::UpdateInput()
{
	if (ISKEYPRESSED(VK_F3))
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

	if (ISMOUSEPRESSED(MouseButton::MOUSE_BUTTON_RIGHT))
	{
		UI->MouseEvent(MouseButton::MOUSE_BUTTON_RIGHT, InputEventType::INPUT_EVENT_CLICK);
	}

	if (ISMOUSEPRESSED(MouseButton::MOUSE_BUTTON_RIGHT))
	{
		UI->MouseEvent(MouseButton::MOUSE_BUTTON_RIGHT, InputEventType::INPUT_EVENT_RELEASE);
	}

	RESOURCEUPDATE;
}

void Application::UpdateUI()
{
	float delta = (float)DELTAUI;

	DXFLOAT3 camera = m_camera->GetPosition();
	DXFLOAT3 camRot = m_camera->GetRotation();
	bool change = false;
	bool change2 = false;

	// Speed per second
	float speed = 9.0f * delta;
	float speed2 = 60.0f * delta;
	float speed3 = 120.0f * delta;
	
	DXFLOAT3 position = m_camera->GetPosition();
	DXFLOAT3 rotation = m_camera->GetRotation();
	DXFLOAT3 lookAt = { 0.0f, 0.0f, 1.0f };
	DXFLOAT3 lookRight = { 1.0f, 0.0f, 0.0f };
	DXFLOAT3 lookUp = { 0.0f, 1.0f, 0.0f };
	DXFLOAT3 cameraFront{};
	DXFLOAT3 cameraRight{};
	DXFLOAT3 cameraUp{};

	DXVECTOR positionVector = XMLoadFloat3(&position);
	DXVECTOR lookAtVector = XMLoadFloat3(&lookAt);
	DXVECTOR lookRightVector = XMLoadFloat3(&lookRight);
	DXVECTOR lookUpVector = XMLoadFloat3(&lookUp);

	// PI / 180.0f
	float oneDegree = 0.0174532925f;
	float yaw = rotation.x * oneDegree;
	float pitch = rotation.y * oneDegree;
	float roll = rotation.z * oneDegree;

	// Create the rotation matrix from the yaw, pitch and roll values
	DXMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

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

	std::wstring text = L"delta: " + ToWStringWithPrecision(DELTA, 8) + L"s\n" +
		L"Mouse: X " + ToWStringWithPrecision(MOUSEPOSITION.X, 0) + L", Y " + ToWStringWithPrecision(MOUSEPOSITION.Y, 0) + L"\n" +
		L"Camera Position:\n" +
		L"X " + ToWStringWithPrecision(m_camera->GetPosition().x, 3) + L", Y " + ToWStringWithPrecision(m_camera->GetPosition().y, 3) + L", Z " + ToWStringWithPrecision(m_camera->GetPosition().z, 3) + L"\n" +
		L"Camera Rotation:\n" +
		L"X " + ToWStringWithPrecision(m_camera->GetRotation().x, 3) + L", Y " + ToWStringWithPrecision(m_camera->GetRotation().y, 3) + L", Z " + ToWStringWithPrecision(m_camera->GetRotation().z, 3) + L"\n" +
		L"InputState: " + std::to_wstring(static_cast<unsigned int>(INP->GetInputState())).c_str();

	DEBUG->SetText(0, 0, text);

	UI->Update();

	std::wstring resourceText = L"--- STATIC RESOURCES ---\n";
	for (auto& resource : RESOURCE->GetStaticTextureMap())
	{
		resourceText += L"Tex - " + resource.first + L"\n";
	}
	for (auto& resource : RESOURCE->GetStaticMeshMap())
	{
		resourceText += L"Mesh - " + resource.first + L"\n";
	}
	for (auto& resource : RESOURCE->GetStaticAnimatedMeshMap())
	{
		resourceText += L"AMesh - " + resource.first + L"\n";
	}
	for (auto& resource : RESOURCE->GetStaticMaterialMap())
	{
		resourceText += L"Mat - " + resource.first + L"\n";
	}
	resourceText += L"\n--- DYNAMIC RESOURCES ---\n";
	for (auto& resource : RESOURCE->GetDynamicTextureMap())
	{
		resourceText += L"Tex - " + resource.first + L" (Ref: " + std::to_wstring(resource.second.referenceCount) +
			L", Time: " + ToWStringWithPrecision(resource.second.timeSinceUnused, 1) + L"s)\n";
	}
	for (auto& resource : RESOURCE->GetDynamicMeshMap())
	{
		resourceText += L"Mesh - " + resource.first + L" (Ref: " + std::to_wstring(resource.second.referenceCount) +
			L", Time: " + ToWStringWithPrecision(resource.second.timeSinceUnused, 1) + L"s)\n";
	}
	for (auto& resource : RESOURCE->GetDynamicAnimatedMeshMap())
	{
		resourceText += L"AMesh - " + resource.first + L" (Ref: " + std::to_wstring(resource.second.referenceCount) +
			L", Time: " + ToWStringWithPrecision(resource.second.timeSinceUnused, 1) + L"s)\n";
	}
	for (auto& resource : RESOURCE->GetDynamicMaterialMap())
	{
		resourceText += L"Mat - " + resource.first + L" (Ref: " + std::to_wstring(resource.second.referenceCount) +
			L", Time: " + ToWStringWithPrecision(resource.second.timeSinceUnused, 1) + L"s)\n";
	}
	resourceText += L"\n--- UNUSED RESOURCES ---\n";
	for (auto& resource : RESOURCE->GetUnusedTexturesVector())
	{
		resourceText += L"Tex - " + resource + L"\n";
	}
	for (auto& resource : RESOURCE->GetUnusedMeshVector())
	{
		resourceText += L"Mesh - " + resource + L"\n";
	}
	for (auto& resource : RESOURCE->GetUnusedAnimatedMeshVector())
	{
		resourceText += L"AMesh - " + resource + L"\n";
	}
	for (auto& resource : RESOURCE->GetUnusedMaterialVector())
	{
		resourceText += L"Mat - " + resource + L"\n";
	}
	resourceText += L"\n--- LOADED FONTS ---\n";
	for (auto& font : RESOURCE->GetFontMap())
	{
		resourceText += L"- " + font.first + L"\n";
	}
	resourceText += L"\n--- LOADED SHADERS ---\n";
	for (auto& font : RESOURCE->GetShaderMap())
	{
		resourceText += L"- " + font.first + L"\n";
	}
	DEBUG->SetText(2, 0, resourceText);
}

void Application::Update3D()
{
	m_camera->Update();
	WorldObject::SetViewMatrix(m_camera->GetViewMatrix());
	
	static float speed = 2.5f;
	static float rotSpeed = 45.0f;
	static float direction = 1;
	DXFLOAT3 pos = m_testObject->GetPosition();
	DXFLOAT3 rot = m_testObject->GetRotation();
	if (pos.x > 5.0f)
		direction = -1;
	else if (pos.x < -5.0f)
		direction = 1;
	
	m_testObject->SetPosition(pos.x + (DELTAWORLD * speed * direction), pos.y, pos.z);
	m_testObject->SetRotation(rot.x + (DELTAWORLD * rotSpeed), rot.y + (DELTAWORLD * (rotSpeed * 0.5f)), rot.z);

	WORLDUPDATE;
}

void Application::Render()
{
	DXMANAGER->BeginScene(0.6875f, 0.765625f, 0.8671875f, 1.0f);

	/*
	DXMATRIX viewMatrix = m_camera->GetViewMatrix();
	DXMATRIX worldMatrix;
	DXMATRIX projectionMatrix;
	DXMATRIX orthoMatrix;
	DXMATRIX translateMatrix;

	worldMatrix = m_renderMatrix.worldMatrix;
	projectionMatrix = m_renderMatrix.projectionMatrix;
	orthoMatrix = m_renderMatrix.orthoMatrix;
	*/
	/*
	for (int i = 0; i < 256; i++)
	{
		translateMatrix = DXMATRIXTranslation((i % 16), 0, ((i / 16) % 16));
		worldMatrix = DXMATRIXMultiply(DXMATRIXIdentity(), translateMatrix);
		ENTITY->GetEntity(0)->Render(DXDEVICECONTEXT, worldMatrix, viewMatrix, projectionMatrix);
	}
	*/

	WORLDRENDER;

	DXMANAGER->EnableZBuffer(false);
	DXMANAGER->EnableBlendState(true);
	/*
	translateMatrix = XMMatrixTranslation(0, 0, 1);
	viewMatrix = XMMatrixMultiply(XMMatrixIdentity(), XMMatrixTranslation(0.0f, 0.0f, 1.0f));
	worldMatrix = XMMatrixIdentity();
	*/

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

