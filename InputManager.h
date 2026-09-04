#pragma once

#include "Position.h"

#define INP					InputManager::Instance()
#define INPUTUPDATE()		InputManager::Instance()->Update()
#define ISKEYDOWN(k)        InputManager::Instance()->IsKeyDown(k)
#define ISKEYPRESSED(k)     InputManager::Instance()->IsKeyPressed(k)
#define ISKEYRELEASED(k)    InputManager::Instance()->IsKeyReleased(k)
#define ISMOUSEDOWN(b)      InputManager::Instance()->IsMouseButtonDown(b)
#define ISMOUSEPRESSED(b)   InputManager::Instance()->IsMouseButtonPressed(b)
#define MOUSEPOSITION		InputManager::Instance()->GetMousePosition()
#define MOUSEX				InputManager::Instance()->GetMouseX()
#define MOUSEY				InputManager::Instance()->GetMouseY()

enum class MouseButton : unsigned int
{
	MOUSE_BUTTON_LEFT		= 0,
	MOUSE_BUTTON_RIGHT		= 1,
	MOUSE_BUTTON_MIDDLE		= 2,
	MOUSE_BUTTON_X1			= 3,
	MOUSE_BUTTON_X2			= 4,
};

enum class InputState
{
	INPUT_STATE_GAMEPLAY,			// All controls allowed, basic gameplay
	INPUT_STATE_MENU,				// Controls limited for keyboard navigation through menus
	INPUT_STATE_TEXTEDITING,		// Controls disabled for text input until ENTER or ECS pressed
	INPUT_STATE_CUTSCENE,			// Controls disabled except controls for skipping and such
};

class InputManager
{
public:
	~InputManager();

	void Update();

	// Keyboard
	void KeyDown(unsigned int key);
	void KeyUp(unsigned int key);

	bool IsKeyDown(unsigned int key);
	bool IsKeyPressed(unsigned int key);
	bool IsKeyReleased(unsigned int key);

	// Mouse
	void MouseButtonDown(MouseButton button);
	void MouseButtonUp(MouseButton button);
	void SetMousePosition(Position position);
	void SetMousePosition(int x, int y);

	bool IsMouseButtonDown(MouseButton button);
	bool IsMouseButtonPressed(MouseButton button);
	bool IsMouseButtonReleased(MouseButton button);
	Position GetMousePosition() const { return m_mousePosition; }
	float GetMouseX() const { return m_mousePosition.X; }
	float GetMouseY() const { return m_mousePosition.Y; }


	void SetInputState(InputState state) { m_inputState = state; }
	InputState GetInputState() { return m_inputState; }
	bool IsStateGameplay() { return m_inputState == InputState::INPUT_STATE_GAMEPLAY; }
	bool IsStateMenu() { return m_inputState == InputState::INPUT_STATE_MENU; }
	bool IsStateTextEdit() { return m_inputState == InputState::INPUT_STATE_TEXTEDITING; }


	static InputManager* Instance();

private:
	InputManager();

	static constexpr unsigned int MAX_KEYS = 256;
	static constexpr unsigned int MAX_BUTTONS = 5;

	bool m_keys[MAX_KEYS]{ false };
	bool m_prevKeys[MAX_KEYS]{ false };

	bool m_mouse[MAX_BUTTONS]{ false };
	bool m_prevMouse[MAX_BUTTONS]{ false };

	Position m_mousePosition{ 0, 0 };

	InputState m_inputState{ InputState::INPUT_STATE_GAMEPLAY };


	static InputManager* ms_instance;
};

