#include "UIButton.h"

// --- ADDITIONAL INCLUDES ---
#include "UIImage.h"
#include "UIText.h"
#include "Deleters.h"
#include "Utils.h"
#include "TimerManager.h"

#include <Windows.h>

// --- MACROS & DEFINES ---


// --- FORWARD DECLARATIONS ---


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC GLOBAL STATES & DATA								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
UIButton* UIButton::ms_captured = nullptr;

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CONSTRUCTORS & DESTRUCTOR								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
UIButton::UIButton() : UIButton({ 0,0 }, { 0,0 })
{
}

UIButton::UIButton(Position position, Size size) : UIElement(position, size)
{
	m_interactive = true;
	EnsureVertexMemory(sizeof(SPVertex), 4, 6, 0);
}

UIButton::~UIButton()
{
	SAFE_DELETE(m_image);
	SAFE_DELETE(m_text);

	for (auto* texture : m_texture)
	{
		texture = nullptr;
	}
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CORE FUNCTIONS											//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//bool UIButton::Initialize()
//{
//	return true;
//}

void UIButton::Update()
{
	if (!Visibility)
		return;

	ButtonState previousState = m_buttonState;
	UpdateButtonState();

	if (UIButton::IsCaptured(this) && m_repeatable)
	{
		OnPress();
	}

	if (previousState != m_buttonState || m_colorChange)
	{
		UIElement::SetBackgroundColor(m_backgroundColor[m_buttonState]);
		UIElement::SetBackgroundBorderColor(m_borderColor[m_buttonState]);
		Invalidate(UpdateFlag::UPDATE_COLORS);
		m_colorChange = false;
	}

	UIElement::Update();

	if (m_image != nullptr)
	{
		m_image->Update();
	}

	if (m_text != nullptr)
	{
		m_text->Update();
	}
}

void UIButton::Render()
{
	if (!Visibility)
		return;

	UIElement::Render();

	if (m_image != nullptr)
	{
		m_image->Render();
	}

	if (m_text != nullptr)
	{
		m_text->Render();
	}
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PUBLIC VIRTUAL FUNCTIONS								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CLASS API												//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void UIButton::SetText(std::wstring text)
{
	EnsureTextObject();
	m_text->SetText(text);
}

void UIButton::SetFont(Font* font)
{
	EnsureTextObject();
	m_text->SetFont(font);
}

void UIButton::SetFontSize(float size)
{
	EnsureTextObject();
	m_text->SetFontSize(size);
}

void UIButton::SetFontColor(Color color)
{
	EnsureTextObject();
	m_text->SetFontColor(color);
}

void UIButton::SetMaxLength(int length)
{
	EnsureTextObject();
	m_text->SetMaxLength(length);
}

void UIButton::SetImage(Texture* texture)
{
	for (auto& stateTexture : m_texture)
	{
		stateTexture = texture;
	}
}

void UIButton::SetImage(Texture* texture, ButtonState state)
{
	m_texture[state] = texture;
}

void UIButton::SetBackgroundColor(Color color)
{
	for (auto& stateColor : m_backgroundColor)
	{
		stateColor = color;
	}
	m_colorChange = true;
}

void UIButton::SetBackgroundColor(Color color, ButtonState state)
{
	m_backgroundColor[state] = color;
	if (m_buttonState == state)
		m_colorChange = true;
}

void UIButton::SetBorderColor(Color color)
{
	for (auto& stateBorderColor : m_borderColor)
	{
		stateBorderColor = color;
	}
	m_colorChange = true;
}

void UIButton::SetBorderColor(Color color, ButtonState state)
{
	m_borderColor[state] = color;
	if (m_buttonState == state)
		m_colorChange = true;
}

void UIButton::SetBorderSize(float size)
{
	UIElement::SetBorderSize(size);
	if (m_image != nullptr)
	{
		m_image->SetBorderSize(size);
	}
	if (m_text != nullptr)
	{
		m_text->SetPosition(size, size);
		m_text->SetSize(m_size - size * 2.0f);
	}
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	GETTERS & SETTERS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC CLASS API										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED VIRTUAL FUNCTIONS								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void UIButton::LeftClickInternal(float mouseX, float mouseY)
{
	if (m_disabled)
		return;

	ms_captured = this;
	m_pressDelta = 0.0;
	m_elapsedTime = 0.0;
	if (m_repeatable)
	{
		OnClick(this);
	}
}

void UIButton::LeftReleaseInternal(float mouseX, float mouseY)
{
	if (m_disabled || ms_captured != this)
		return;

	ms_captured = nullptr;
	m_pressDelta = 0.0;
	m_elapsedTime = 0.0;

	if (!m_repeatable && IsMouseOver())
	{
		OnClick(this);
	}

	OnPress();
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PRIVATE FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void UIButton::UpdateButtonState()
{
	if (m_disabled)
	{
		m_buttonState = ButtonState::BUTTON_STATE_DISABLED;
		return;
	}

	if (ms_captured == this)
	{
		m_buttonState = ButtonState::BUTTON_STATE_PRESSED;
		return;
	}

	if (IsMouseOver())
	{
		m_buttonState = ButtonState::BUTTON_STATE_HOVER;
		return;
	}

	m_buttonState = ButtonState::BUTTON_STATE_IDLE;
}

void UIButton::EnsureImageObject()
{
	if (m_image != nullptr)
		return;

	m_image = new UIImage({ 0, 0 }, m_size);
	m_image->SetBorderSize(m_borderSize);
	m_image->SetParent(this);
}

void UIButton::EnsureTextObject()
{
	if (m_text != nullptr)
		return;

	m_text = new UIText({ m_borderSize, m_borderSize }, m_size - m_borderSize * 2.0f);
	m_text->SetTextAlignment(HorizontalTextAlignment::HORIZONTAL_TEXT_ALIGNMENT_CENTER, VerticalTextAlignment::VERTICAL_TEXT_ALIGNMENT_MIDDLE);
	m_text->SetParent(this);
}

void UIButton::OnPress()
{
	double delta = DELTA;
	m_elapsedTime += delta;

	if (m_elapsedTime >= PRESS_DELAY)
	{
		m_pressDelta += delta;
	}

	if (m_pressDelta >= PRESS_INTERVAL)
	{
		m_pressDelta -= PRESS_INTERVAL;
		OnClick(this);
	}
}

