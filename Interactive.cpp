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
void Interactive::LeftClick(float mouseX, float mouseY)
{
	LeftClickInternal(mouseX, mouseY);
	OnLeftClick.Invoke(this, mouseX, mouseY);
}

void Interactive::RightClick(float mouseX, float mouseY)
{
	RightClickInternal(mouseX, mouseY);
	OnRightClick.Invoke(this, mouseX, mouseY);
}

void Interactive::MiddleClick(float mouseX, float mouseY)
{
	MiddleClickInternal(mouseX, mouseY);
	OnMiddleClick.Invoke(this, mouseX, mouseY);
}



void Interactive::LeftRelease(float mouseX, float mouseY)
{
	LeftReleaseInternal(mouseX, mouseY);
	OnLeftRelease.Invoke(this, mouseX, mouseY);
}

void Interactive::RightRelease(float mouseX, float mouseY)
{
	RightReleaseInternal(mouseX, mouseY);
	OnRightRelease.Invoke(this, mouseX, mouseY);
}

void Interactive::MiddleRelease(float mouseX, float mouseY)
{
	MiddleReleaseInternal(mouseX, mouseY);
	OnMiddleRelease.Invoke(this, mouseX, mouseY);
}



void Interactive::MouseEnter()
{
	MouseEnterInternal();
	OnMouseEnter.Invoke(this);
}

void Interactive::MouseLeave()
{
	MouseLeaveInternal();
	OnMouseLeave.Invoke(this);
}

void Interactive::MouseMove(float mouseX, float mouseY)
{
	MouseMoveInternal(mouseX, mouseY);
	OnMouseMove.Invoke(this, mouseX, mouseY);
}



void Interactive::Focus()
{
	FocusInternal();
	OnFocus.Invoke(this);
}

void Interactive::FocusLost()
{
	FocusLostInternal();
	OnFocusLost.Invoke(this);
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
	
	if (object == nullptr)			// If no element, remove focus and return
	{
		RemoveFocus(type);
		return;
	}
	
	if (ms_focus[type] != nullptr)	// If something focused, remove focus
	{
		RemoveFocus(type);
	}
	
	ms_focus[type] = object;		// Set new focus
	ms_focus[type]->Focus();		// Call Focus event
	INP->SetInputState(ms_focus[type]->GetRequiredInputState());
}

void Interactive::RemoveFocus(FocusType type)
{
	if (ms_focus[type] == nullptr)
		return;

	ms_focus[type]->FocusLost();
	ms_focus[type] = nullptr;
	INP->SetInputState(InputState::INPUT_STATE_GAMEPLAY);
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED VIRTUALS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void Interactive::UpdateMouseEvents()
{
	bool hover = CheckMouseOver();

	if (hover && !m_isHover)
	{
		m_isHover = true;
		MouseEnter();
	}
	else if (!hover && m_isHover)
	{
		m_isHover = false;
		MouseLeave();
	}
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PRIVATE FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //

