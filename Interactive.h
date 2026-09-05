#pragma once

// --- INCLUDES ---
#include "Event.h"
#include "InputManager.h"

// --- CONSTANTS ---
static constexpr unsigned int MAX_FOCUS_TYPES = 2;

// --- ENUMS & FLAGS ---
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

	// --- EVENT API ---
	void HandleLeftClick();
	void HandleRightClick();
	void HandleMiddleClick();

	void HandleLeftRelease();
	void HandleRightRelease();
	void HandleMiddleRelease();

	void HandleMouseEnter();
	void HandleMouseLeave();
	void HandleMouseMove(int x, int y);

	void HandleFocus();
	void HandleFocusLost();

	// --- SETTERS & GETTERS ---
	InputState GetRequiredInputState() const { return m_requiredInputState; }
	FocusType GetFocusType() const { return m_focusType; }
	bool IsInteractive() const { return m_interactive; }
	bool IsHover() const { return m_isHover; }

	// --- VIRTUAL EVENT API ---
	virtual void OnTextInput(wchar_t character) {}

	// --- STATIC EVENT API ---
	static void SetFocus(Interactive* object, FocusType type);
	static void RemoveFocus(FocusType type);
	static Interactive* GetFocus(int type) { return ms_focus[type]; }

protected:
	// --- CONSTRUCTORS & DESTRUCTORS ---
	Interactive();

	// --- INTERNAL INPUT LOGIC ---
	void UpdateMouseEvents();

	// --- INTERNAL VIRTUALS ---
	virtual void OnLeftClick();
	virtual void OnRightClick() {}
	virtual void OnMiddleClick() {}

	virtual void OnLeftRelease() {}
	virtual void OnRightRelease() {}
	virtual void OnMiddleRelease() {}

	virtual void OnMouseEnter() {}
	virtual void OnMouseLeave() {}
	virtual void OnMouseMove(int x, int y) {}

	virtual void OnFocus() {}
	virtual void OnFocusLost() {}

	virtual bool CheckMouseOver() const = 0;
	virtual bool CapturesKeyboard() const { return false; }

private:


// - PROPERTIES -
public:
	// --- PUBLIC EVENT PROPERTIES ---
	Event<Interactive*> LeftClick;
	Event<Interactive*> RightClick;
	Event<Interactive*> MiddleClick;

	Event<Interactive*> LeftRelease;
	Event<Interactive*> RightRelease;
	Event<Interactive*> MiddleRelease;

	Event<Interactive*> MouseEnter;
	Event<Interactive*> MouseLeave;
	Event<Interactive*, int, int> MouseMove;

	Event<Interactive*> Focus;
	Event<Interactive*> FocusLost;

protected:
	// --- INTERNAL INTERACTION STATES ---
	InputState m_requiredInputState{ InputState::INPUT_STATE_GAMEPLAY };
	FocusType m_focusType{ FocusType::FOCUS_TYPE_UI };
	bool m_interactive{ false };
	bool m_isHover{ false };

private:

	// --- STATIC GLOBAL INTERACTION STATES ---
	static Interactive* ms_focus[MAX_FOCUS_TYPES];
};

