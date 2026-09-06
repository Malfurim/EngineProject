#include "InputManager.h"

// --- ADDITIONAL INCLUDES ---
#include <string>

// --- MACROS & DEFINES ---


// --- FORWARD DECLARATIONS ---


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC GLOBAL STATES & DATA								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
InputManager* InputManager::ms_instance = nullptr;

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CONSTRUCTORS & DESTRUCTOR								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
InputManager::InputManager()
{
	for (unsigned int i = 0; i < MAX_KEYS; i++)
	{
		m_keys[i] = false;
		m_prevKeys[i] = false;
	}

	for (unsigned int i = 0; i < MAX_BUTTONS; i++)
	{
		m_mouse[i] = false;
		m_prevMouse[i] = false;
	}
}

InputManager::~InputManager()
{
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CORE FUNCTIONS											//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//bool InputManager::Initialize()
//{
//	return true;
//}

void InputManager::Update()
{
	for (unsigned int i = 0; i < MAX_KEYS; i++)
		m_prevKeys[i] = m_keys[i];
	for (unsigned int i = 0; i < MAX_BUTTONS; i++)
		m_prevMouse[i] = m_mouse[i];
}

//void InputManager::Render()
//{
//}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	VIRTUAL FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CLASS API												//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void InputManager::KeyDown(unsigned int key)
{
	if (key < MAX_KEYS)
		m_keys[key] = true;
}

void InputManager::KeyUp(unsigned int key)
{
	if (key < MAX_KEYS)
		m_keys[key] = false;
}

void InputManager::MouseButtonDown(MouseButton button)
{
	unsigned int index = (unsigned int)button;
	if (index < MAX_BUTTONS)
	{
		m_mouse[index] = true;
	}
}

void InputManager::MouseButtonUp(MouseButton button)
{
	unsigned int index = (unsigned int)button;
	if (index < MAX_BUTTONS)
	{
		m_mouse[index] = false;
	}
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	GETTERS & SETTERS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
bool InputManager::IsKeyDown(unsigned int key)
{
	if (key >= MAX_KEYS)
		return false;
	return m_keys[key];
}

bool InputManager::IsKeyPressed(unsigned int key)
{
	if (key >= MAX_KEYS)
		return false;
	return m_keys[key] && !m_prevKeys[key];
}

bool InputManager::IsKeyReleased(unsigned int key)
{
	if (key >= MAX_KEYS)
		return false;
	return !m_keys[key] && m_prevKeys[key];
}

bool InputManager::IsMouseButtonDown(MouseButton button)
{
	unsigned int index = (unsigned int)button;
	if (index >= MAX_BUTTONS)
		return false;
	return m_mouse[index];
}

bool InputManager::IsMouseButtonPressed(MouseButton button)
{
	unsigned int index = (unsigned int)button;
	if (index >= MAX_BUTTONS)
		return false;
	return m_mouse[index] && !m_prevMouse[index];
}

bool InputManager::IsMouseButtonReleased(MouseButton button)
{
	unsigned int index = (unsigned int)button;
	if (index >= MAX_BUTTONS)
		return false;
	return !m_mouse[index] && m_prevMouse[index];
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC CLASS API										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
InputManager* InputManager::Instance()
{
	if (ms_instance == nullptr)
		ms_instance = new InputManager();
	return ms_instance;
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

