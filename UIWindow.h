#pragma once

// --- INCLUDES ---
#include <string>
#include "Position.h"
#include "UIPanel.h"
#include "WindowStyle.h"

// --- CONSTANTS ---


// --- MACROS & DEFINES ---


// --- ENUMS & STRUCTS ---
enum class WindowCloseButton
{
	CLOSEBUTTON_NONE,
	CLOSEBUTTON_CLOSE,
	CLOSEBUTTON_DESTROY,
};

enum class WindowDrag
{
	DRAG_NONE,
	DRAG_TITLE,
	DRAG_FULL,
};

// --- FORWARD DECLARATIONS ---
class UIText;
class UIImage;
class UIButton;

class UIWindow : public UIPanel
{
	friend class UserInterface;
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	UIWindow(Position position, Size size, const std::wstring& titleText, bool openAtInitialPosition, WindowCloseButton closeButtonType, const WindowStyle& windowStyle, WindowDrag draggable = WindowDrag::DRAG_TITLE);
	~UIWindow();

	// --- CORE FUNCTIONS ---
	// bool Initialize();
	void Update() override;
	void Render() override;

	// --- PUBLIC VIRTUAL FUNCTIONS ---
	void AddChild(UIElement* element) override;
	bool ContainsChild(UIElement* element) override;
	bool RemoveChild(UIElement* element) override;
	bool RemoveChild(int index) override;

	// --- CLASS API ---
	void Open();
	void Close();
	void Destroy();

	// --- GETTERS & SETTERS ---
	Size GetContentSize() const { return m_content->GetSize(); }

	// --- STATIC CLASS API ---
	static bool IsCaptured() { return ms_captured != nullptr; }
	static bool IsCaptured(const UIWindow* element) { return ms_captured == element; }
	static void ReleaseCapture()
	{
		if (IsCaptured())
		{
			ms_captured->LeftReleaseInternal(0.0f, 0.0f);
		}
	}

protected:
	// --- CONSTRUCTORS ---


	// --- PROTECTED VIRTUAL FUNCTIONS ---
	void LeftClickInternal(float mouseX, float mouseY) override;
	void LeftReleaseInternal(float mouseX, float mouseY) override;

	// --- PROTECTED FUNCTIONS ---


private:
	// --- CONSTRUCTORS ---


	// --- PRIVATE FUNCTIONS ---
	void AddStructureChild(UIElement* element);
	void EnsureTitleObject(Size size, const WindowStyle& windowStyle, const std::wstring& text, bool hasCloseButton);
	void EnsureCloseButtonObject(Size size, const WindowStyle& windowStyle, WindowCloseButton closeButtonType);
	void EnsureGraphicObject(Size size, const WindowStyle& windowStyle);
	void EnsureContentObject(Size size, const WindowStyle& windowStyle);

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
	bool m_initialized{ false };
	WindowDrag m_dragType{ WindowDrag::DRAG_NONE };
	bool m_isDragging{ false };
	bool m_openAtInitialPosition{ false };

	// --- PRIVATE COMPONENT DATA ---
	UIPanel* m_titleBar{ nullptr };
	UIButton* m_closeButton{ nullptr };
	UIImage* m_graphicTexture{ nullptr };
	UIPanel* m_content{ nullptr };

	Position m_initialPosition{ 0.0f, 0.0f };

	// --- STATIC GLOBAL STATES ---
	static UIWindow* ms_captured;
	static Position ms_previousMouseDragPosition;
};