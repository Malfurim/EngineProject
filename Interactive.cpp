#include "Interactive.h"

// --- ADDITIONAL INCLUDES ---
#include "InputManager.h"

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CONSTRUCTORS & DESTRUCTOR								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //

Interactive* Interactive::ms_focus[MAX_FOCUS_TYPES]{};

Interactive::Interactive()
{
}

Interactive::~Interactive()
{
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	EVENT API												//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void Interactive::HandleLeftClick()
{
	OnLeftClick();
	LeftClick.Invoke(this);
}

void Interactive::HandleRightClick()
{
	OnRightClick();
	RightClick.Invoke(this);
}

void Interactive::HandleMiddleClick()
{
	OnMiddleClick();
	MiddleClick.Invoke(this);
}



void Interactive::HandleLeftRelease()
{
	OnLeftRelease();
	LeftRelease.Invoke(this);
}

void Interactive::HandleRightRelease()
{
	OnRightRelease();
	RightRelease.Invoke(this);
}

void Interactive::HandleMiddleRelease()
{
	OnMiddleRelease();
	MiddleRelease.Invoke(this);
}



void Interactive::HandleMouseEnter()
{
	OnMouseEnter();
	MouseEnter.Invoke(this);
}

void Interactive::HandleMouseLeave()
{
	OnMouseLeave();
	MouseLeave.Invoke(this);
}

void Interactive::HandleMouseMove(int x, int y)
{
	OnMouseMove(x, y);
	MouseMove.Invoke(this, x, y);
}



void Interactive::HandleFocus()
{
	OnFocus();
	Focus.Invoke(this);
}

void Interactive::HandleFocusLost()
{
	OnFocusLost();
	FocusLost.Invoke(this);
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	SETTERS & GETTERS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC EVENT API										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void Interactive::SetFocus(Interactive* object, FocusType type)
{
	if (ms_focus[type] == object)
		return;

	if (object == nullptr)
	{
		RemoveFocus(type);
		return;
	}

	if (ms_focus[type] != nullptr)
	{
		ms_focus[type]->HandleFocusLost();
	}

	ms_focus[type] = object;
	ms_focus[type]->HandleFocus();
	INP->SetInputState(ms_focus[type]->GetRequiredInputState());
}

void Interactive::RemoveFocus(FocusType type)
{
	if (ms_focus[type] == nullptr)
		return;

	ms_focus[type]->HandleFocusLost();
	ms_focus[type] = nullptr;
	INP->SetInputState(InputState::INPUT_STATE_GAMEPLAY);
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	INTERNAL INPUT LOGIC									//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void Interactive::UpdateMouseEvents()
{
	bool hover = CheckMouseOver();

	if (hover && !m_isHover)
	{
		m_isHover = true;
		OnMouseEnter();
	}
	else if (!hover && m_isHover)
	{
		m_isHover = false;
		OnMouseLeave();
	}
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	INTERNAL VIRTUALS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void Interactive::OnLeftClick()
{
	if (!m_interactive)
		return;

	Interactive::SetFocus(this, m_focusType);
}

