#include "UserInterface.h"

// --- ADDITIONAL INCLUDES ---
#include <algorithm>
#include <iterator>
#include "InputManager.h"
#include "UIElement.h"
#include "UIButton.h"
#include "UIWindow.h"
#include "Deleters.h"

// --- MACROS & DEFINES ---


// --- FORWARD DECLARATIONS ---


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC GLOBAL STATES & DATA								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
UserInterface* UserInterface::ms_instance = nullptr;

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CONSTRUCTORS & DESTRUCTOR								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
UserInterface::UserInterface()
{
}

UserInterface::~UserInterface()
{
	SAFE_DELETE_VECTOR(m_windows);
	ms_instance = nullptr;
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CORE FUNCTIONS											//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//bool UserInterface::Initialize()
//{
//	return true;
//}

void UserInterface::Update()
{
	for (auto* window : m_windows)
		window->Update();

	if (!m_windowsToDestroy.empty())
	{
		for (auto* window : m_windowsToDestroy)
		{
			delete window;
		}
		m_windowsToDestroy.clear();
	}
}

void UserInterface::Render()
{
	for (auto* window : m_windows)
		window->Render();
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PUBLIC VIRTUAL FUNCTIONS								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CLASS API												//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void UserInterface::AddWindow(UIWindow* window, WindowType windowType)
{
	if (window == nullptr)
		return;

	if (ContainsWindow(window))
		return;

	int zIndex = 0;
	std::vector<UIWindow*>::iterator insertPosition;
	switch (windowType)
	{
	case WindowType::WINDOW_FORCEDBACK:
		zIndex = BACK_WINDOW_OFFSET + m_forcedBackWindows;
		insertPosition = m_windows.begin() + m_forcedBackWindows;
		window->m_dragType = WindowDrag::DRAG_NONE;
		m_forcedBackWindows++;
		break;

	case WindowType::WINDOW_FORCEDFRONT:
		zIndex = FRONT_WINDOW_OFFSET + m_forcedFrontWindows;
		insertPosition = m_windows.end();
		window->m_dragType = WindowDrag::DRAG_NONE;
		m_forcedFrontWindows++;
		break;

	default:	// WindowType::WINDOW_INTERACTIVE
		zIndex = INTERACTIVE_WINDOW_OFFSET + m_interactiveWindows;
		insertPosition = m_windows.end() - m_forcedFrontWindows;
		m_interactiveWindows++;
		break;
	}

	window->SetZIndex(zIndex);
	m_windows.insert(insertPosition, window);
}

bool UserInterface::ContainsWindow(UIWindow* window)
{
	for (auto* element : m_windows)
	{
		if (element == window)
			return true;
	}
	return false;
}

bool UserInterface::RemoveWindow(UIWindow* window)
{
	auto index = std::find(m_windows.begin(), m_windows.end(), window);
	if (index != m_windows.end())
	{
		int i = static_cast<int>(std::distance(m_windows.begin(), index));
		DecreaseWindowCount(i);
		m_windows.erase(index);
		ResetZIndexes(i);
		m_windowsToDestroy.push_back(window);
		return true;
	}
	return false;
}

bool UserInterface::RemoveWindow(int index)
{
	if (index >= m_windows.size() || index < 0)
		return false;

	DecreaseWindowCount(index);
	UIWindow* window = m_windows[index];
	m_windows.erase(m_windows.begin() + index);
	delete window;
	ResetZIndexes(index);
	return true;
}

void UserInterface::SortWindows()
{
	std::sort(m_windows.begin(), m_windows.end(), [](UIWindow* element1, UIWindow* element2) {
		return element1->m_zIndex < element2->m_zIndex;
		});
}

bool UserInterface::MouseEvent(MouseButton mouseButton, InputEventType eventType)
{
	float x = MOUSEX;
	float y = MOUSEY;

	if (eventType == InputEventType::INPUT_EVENT_RELEASE)
	{
		if (UIButton::IsCaptured() || UIWindow::IsCaptured())
		{
			UIButton::ReleaseCapture();
			UIWindow::ReleaseCapture();
			return true;
		}
	}

	for (auto it = m_windows.rbegin(); it != m_windows.rend(); ++it)
	{
		UIWindow* window = *it;
		if (window == nullptr || !window->Visibility || !window->IsMouseOver() || !window->IsInteractive())
			continue;

		if (eventType == InputEventType::INPUT_EVENT_CLICK && mouseButton == MouseButton::MOUSE_BUTTON_LEFT && window->m_dragType != WindowDrag::DRAG_NONE)
		{
			SetToFront(window);
		}

		UIElement* element = window->GetElementAtPosition(x, y);
		if (element != nullptr && element->IsInteractive())
		{
			DispatchMouseEvent(element, mouseButton, eventType, x, y);
			return true;
		}

		DispatchMouseEvent(window, mouseButton, eventType, x, y);
		return true;
	}

	Interactive::RemoveFocus(FocusType::FOCUS_TYPE_UI);

	if (mouseButton == MouseButton::MOUSE_BUTTON_RIGHT && eventType == InputEventType::INPUT_EVENT_CLICK)
	{
		Position mouseDelta = MOUSEDELTA;
		if (mouseDelta.X != 0.0f || mouseDelta.Y != 0.0f)
		{
			// active camera set rotation based on mouse move and move mouse back to origin
			//SetPosition(GetPosition() + mouseDelta);
		}
	}

	return false;
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	GETTERS & SETTERS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
UIWindow* UserInterface::GetWindow(int index)
{
	if (index >= m_windows.size() || index < 0)
		return nullptr;
	return m_windows[index];
}

void UserInterface::SetToFront(UIWindow* window)
{
	if (window == nullptr || m_windows.empty())
		return;

	int zIndex = window->m_zIndex;
	int startIndex = m_forcedBackWindows + m_interactiveWindows - 1;
	int endIndex = m_forcedBackWindows + zIndex - INTERACTIVE_WINDOW_OFFSET;

	if (endIndex == startIndex)
		return;

	for (int i = startIndex; i > endIndex; i--)
	{
		m_windows[i]->SetZIndex(INTERACTIVE_WINDOW_OFFSET + i - m_forcedBackWindows - 1);
	}
	window->SetZIndex(INTERACTIVE_WINDOW_OFFSET + startIndex - m_forcedBackWindows);
	SortWindows();
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC CLASS API										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
UserInterface* UserInterface::Instance()
{
	if (ms_instance == nullptr)
		ms_instance = new UserInterface();
	return ms_instance;
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED VIRTUAL FUNCTIONS								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PRIVATE FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void UserInterface::ResetZIndexes(int removedIndex)
{
	if (removedIndex >= m_windows.size())
		return;

	int startIndex = removedIndex;
	int endIndex = 0;
	int zIndex = 0;

	if (removedIndex == 0 && m_forcedBackWindows > 0) zIndex = BACK_WINDOW_OFFSET;
	else if (removedIndex == m_forcedBackWindows) zIndex = INTERACTIVE_WINDOW_OFFSET;
	else if (removedIndex == m_forcedBackWindows + m_interactiveWindows) zIndex = FRONT_WINDOW_OFFSET;
	else zIndex = m_windows[removedIndex - 1]->m_zIndex + 1;

	if (removedIndex < m_forcedBackWindows)
	{
		endIndex = m_forcedBackWindows;
	}
	else if  (removedIndex >= m_forcedBackWindows && removedIndex < (m_forcedBackWindows + m_interactiveWindows))
	{
		endIndex = m_forcedBackWindows + m_interactiveWindows;
	}
	else
	{
		endIndex = static_cast<int>(m_windows.size());
	}

	for (int i = startIndex; i < endIndex; i++)
	{
		m_windows[i]->SetZIndex(zIndex);
		zIndex++;
	}
}

void UserInterface::DecreaseWindowCount(int index)
{
	if (index >= 0 && index < m_forcedBackWindows)
	{
		m_forcedBackWindows--;
	}
	else if (index >= m_forcedBackWindows && index < m_interactiveWindows + m_forcedBackWindows)
	{
		m_interactiveWindows--;
	}
	else if (index >= m_interactiveWindows + m_forcedBackWindows && index < m_forcedFrontWindows + m_interactiveWindows + m_forcedBackWindows)
	{
		m_forcedFrontWindows--;
	}
}

void UserInterface::DispatchMouseEvent(UIElement* element, MouseButton mouseButton, InputEventType eventType, float x, float y)
{
	Interactive::SetFocus(element, FocusType::FOCUS_TYPE_UI);
	switch (mouseButton)
	{
	case MouseButton::MOUSE_BUTTON_LEFT:
		(eventType == InputEventType::INPUT_EVENT_CLICK) ? element->LeftClick(x, y) : element->LeftRelease(x, y);
		break;
	case MouseButton::MOUSE_BUTTON_MIDDLE:
		(eventType == InputEventType::INPUT_EVENT_CLICK) ? element->MiddleClick(x, y) : element->MiddleRelease(x, y);
		break;
	case MouseButton::MOUSE_BUTTON_RIGHT:
		(eventType == InputEventType::INPUT_EVENT_CLICK) ? element->RightClick(x, y) : element->RightRelease(x, y);
		break;
	default:
		break;
	}
}
