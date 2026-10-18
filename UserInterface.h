#pragma once

// --- INCLUDES ---
#include <vector>

// --- CONSTANTS ---


// --- MACROS & DEFINES ---
static constexpr int STATIC_WINDOW_OFFSET = 0;
static constexpr int INTERACTIVE_WINDOW_OFFSET = 1000;
static constexpr int FRONT_WINDOW_OFFSET = 1000000;

#define UI UserInterface::Instance()
#define UIADD(window) UserInterface::Instance()->AddWindow(window)
#define UIADD2(window, type) UserInterface::Instance()->AddWindow(window, type)
#define UISETFRONT(window) UserInterface::Instance()->SetToFront(window)

// --- ENUMS & STRUCTS ---
enum class WindowType
{
	WINDOW_STATIC,
	WINDOW_INTERACTIVE,
	WINDOW_FORCEDFRONT,
};

// --- FORWARD DECLARATIONS ---
class UIElement;
class UIWindow;
enum class MouseButton : unsigned int;
enum class InputEventType;

class UserInterface
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	~UserInterface();

	// --- CORE FUNCTIONS ---
	// bool Initialize();
	void Update();
	void Render();

	// --- PUBLIC VIRTUAL FUNCTIONS ---


	// --- CLASS API ---
	void AddWindow(UIWindow* window, WindowType windowType = WindowType::WINDOW_INTERACTIVE);
	bool ContainsWindow(UIWindow* window);
	bool RemoveWindow(UIWindow* window);
	bool RemoveWindow(int index);
	void SortWindows();
	bool MouseEvent(MouseButton mouseButton, InputEventType eventType);

	// --- GETTERS & SETTERS ---
	UIWindow* GetWindow(int index);
	void SetToFront(UIWindow* window);

	// --- STATIC CLASS API ---
	static UserInterface* Instance();

protected:
	// --- CONSTRUCTORS ---


	// --- PROTECTED VIRTUAL FUNCTIONS ---


	// --- PROTECTED FUNCTIONS ---


private:
	// --- CONSTRUCTORS ---
	UserInterface();

	// --- PRIVATE FUNCTIONS ---
	void ResetZIndexes(int removedIndex);
	void DecreaseWindowCount(int index);
	void DispatchMouseEvent(UIElement* element, MouseButton mouseButton, InputEventType eventType, float x, float y);

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


	// --- PRIVATE COMPONENT STATES ---


	// --- PRIVATE COMPONENT DATA ---
	std::vector<UIWindow*> m_windows;
	int m_staticWindows{ 0 };
	int m_interactiveWindows{ 0 };
	int m_forcedFrontWindows{ 0 };

	// --- STATIC GLOBAL STATES ---
	static UserInterface* ms_instance;
};