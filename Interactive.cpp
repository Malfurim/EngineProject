#include "Interactive.h"

// --- ADDITIONAL INCLUDES ---
#include "InputManager.h"

// --- MACROS & DEFINES ---


// --- FORWARD DECLARATIONS ---


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC GLOBAL STATES & DATA								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
Interactive* Interactive::ms_focus[MAX_FOCUS_TYPES]{};

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CONSTRUCTORS & DESTRUCTOR								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
Interactive::Interactive()
{
}

Interactive::~Interactive()
{
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CORE FUNCTIONS											//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//bool Interactive::Initialize()
//{
//	return true;
//}

//void Interactive::Update()
//{
//}

//void Interactive::Render()
//{
//}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	VIRTUAL FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CLASS API												//
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
//	GETTERS & SETTERS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC CLASS API										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void Interactive::SetFocus(Interactive* object, FocusType type)
{
	if (ms_focus[type] == object)	// If same element, return
		return;
	
	if (object == nullptr)			// If no element, return
	{
		RemoveFocus(type);
		return;
	}
	
	if (ms_focus[type] != nullptr)	// Remove old focus
	{
		RemoveFocus(type);
		//ms_focus[type]->HandleFocusLost();
	}
	
	ms_focus[type] = object;		// Set new focus
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
//	PROTECTED VIRTUALS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void Interactive::OnLeftClick()
{
	if (!m_interactive)
		return;

	Interactive::SetFocus(this, m_focusType);
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED FUNCTIONS										//
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
//	PRIVATE FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //

