#pragma once

// --- INCLUDES ---
#include "Event.h"
#include "InputManager.h"

// --- CONSTANTS ---
static constexpr unsigned int MAX_FOCUS_TYPES = 2;

// --- MACROS & DEFINES ---


// --- ENUMS & STRUCTS ---
enum FocusType
{
	FOCUS_TYPE_UI,
	FOCUS_TYPE_WORLD,
};

// --- FORWARD DECLARATIONS ---


class Interactive
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	~Interactive();

	// --- CORE FUNCTIONS ---
	// bool Initialize();
	// void Update();
	// void Render();
	
	// --- VIRTUAL FUNCTIONS ---
	virtual void OnCharInput(wchar_t character) {}
	virtual void OnKeyDown(wchar_t key) {}
	virtual void OnKeyUp(wchar_t key) {}

	// --- CLASS API ---
	void LeftClick(float mouseX, float mouseY);
	void RightClick(float mouseX, float mouseY);
	void MiddleClick(float mouseX, float mouseY);

	void LeftRelease(float mouseX, float mouseY);
	void RightRelease(float mouseX, float mouseY);
	void MiddleRelease(float mouseX, float mouseY);

	void MouseEnter();
	void MouseLeave();
	void MouseMove(float mouseX, float mouseY);

	void Focus();
	void FocusLost();

	// --- GETTERS & SETTERS ---
	InputState GetRequiredInputState() const { return m_requiredInputState; }
	FocusType GetFocusType() const { return m_focusType; }
	bool IsInteractive() const { return m_interactive; }
	bool IsHover() const { return m_isHover; }
	bool IsFocused() const { return (ms_focus[m_focusType] == this); }

	// --- STATIC CLASS API ---
	static void SetFocus(Interactive* object, FocusType type);
	static void RemoveFocus(FocusType type);
	static Interactive* GetFocus(int type) { return ms_focus[type]; }

protected:
	// --- CONSTRUCTORS ---
	Interactive();

	// --- VIRTUAL FUNCTIONS ---
	virtual void LeftClickInternal(float mouseX, float mouseY) {}
	virtual void RightClickInternal(float mouseX, float mouseY) {}
	virtual void MiddleClickInternal(float mouseX, float mouseY) {}

	virtual void LeftReleaseInternal(float mouseX, float mouseY) {}
	virtual void RightReleaseInternal(float mouseX, float mouseY) {}
	virtual void MiddleReleaseInternal(float mouseX, float mouseY) {}

	virtual void MouseEnterInternal() {}
	virtual void MouseLeaveInternal() {}
	virtual void MouseMoveInternal(float mouseX, float mouseY) {}

	virtual void FocusInternal() {}
	virtual void FocusLostInternal() {}

	virtual bool CheckMouseOver() const = 0;
	virtual bool CapturesKeyboard() const { return false; }

	// --- PROTECTED FUNCTIONS ---
	void UpdateMouseEvents();

private:
	// --- CONSTRUCTORS ---


	// --- PRIVATE FUNCTIONS ---


// - PROPERTIES -
public:
	// --- PUBLIC COMPONENT STATES ---


	// --- PUBLIC COMPONENT DATA ---
	Event<Interactive*, float, float> OnLeftClick;
	Event<Interactive*, float, float> OnRightClick;
	Event<Interactive*, float, float> OnMiddleClick;

	Event<Interactive*, float, float> OnLeftRelease;
	Event<Interactive*, float, float> OnRightRelease;
	Event<Interactive*, float, float> OnMiddleRelease;

	Event<Interactive*> OnMouseEnter;
	Event<Interactive*> OnMouseLeave;
	Event<Interactive*, float, float> OnMouseMove;

	Event<Interactive*> OnFocus;
	Event<Interactive*> OnFocusLost;

protected:
	// --- INTERNAL CONSTANTS ---


	// --- INTERNAL STRUCTS & ENUMS ---
	
	
	// --- INTERNAL COMPONENT STATES ---
	InputState m_requiredInputState{ InputState::INPUT_STATE_GAMEPLAY };
	FocusType m_focusType{ FocusType::FOCUS_TYPE_UI };
	bool m_interactive{ false };
	bool m_isHover{ false };

	// --- INTERNAL DATA VARIABLES ---

private:
	// --- PRIVATE CONSTANTS ---


	// --- PRIVATE STRUCTS & ENUMS ---


	// --- PRIVATE COMPONENT STATES ---


	// --- COMPONENT DATA VARIABLES ---


	// --- STATIC GLOBAL STATES ---
	static Interactive* ms_focus[MAX_FOCUS_TYPES];
};

