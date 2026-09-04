#pragma once

class Camera;
class UIPanel;
class UIText;


#define APPLICATION Application::Instance()

class Application
{
private:
	struct RenderMatrix
	{
		XMMATRIX worldMatrix;
		XMMATRIX projectionMatrix;
		XMMATRIX orthoMatrix;
	};

public:
	~Application();
	Application(const Application& app);

	bool Initialize();

	void Run();
	void Shutdown();

	void Update();
	void Render();

	static Application* Instance();

private:
	Application();

	RenderMatrix m_renderMatrix{};
	Camera* m_camera{ nullptr };

	UIPanel* m_userInterface{ nullptr };
	UIPanel* m_debug{ nullptr };
	UIText* m_debugText{ nullptr };

	float xx{ 0.0f };

	static Application* m_instance;
};