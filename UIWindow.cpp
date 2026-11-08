#include "UIWindow.h"

// --- ADDITIONAL INCLUDES ---
#include "UIText.h"
#include "UIImage.h"
#include "UIButton.h"
#include "ResourceManager.h"
#include "UserInterface.h"
#include "Deleters.h"

// --- MACROS & DEFINES ---


// --- FORWARD DECLARATIONS ---
UIWindow* UIWindow::ms_captured{ nullptr };
Position UIWindow::ms_previousMouseDragPosition{ 0.0f, 0.0f };

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC GLOBAL STATES & DATA								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CONSTRUCTORS & DESTRUCTOR								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
UIWindow::UIWindow(Position position, Size size, const std::wstring& titleText, bool openAtInitialPosition, WindowCloseButton closeButtonType, const WindowStyle& windowStyle, WindowDrag dragType) :
	UIPanel(position, size)
{
	m_openAtInitialPosition = openAtInitialPosition;
	m_initialPosition = position;
	m_dragType = dragType;
	m_interactive = true;

	if (windowStyle.backgroundTexture.empty())
	{
		SetBackgroundColor(windowStyle.backgroundColor);
		SetBackgroundBorderColor(windowStyle.borderColor);
		SetBorderSize(windowStyle.borderSize);
	}
	else
	{
		EnsureGraphicObject(size, windowStyle);
	}

	bool hasCloseButton = (closeButtonType != WindowCloseButton::CLOSEBUTTON_NONE);
	if (!titleText.empty())	EnsureTitleObject(size, windowStyle, titleText, hasCloseButton);
	if (hasCloseButton)		EnsureCloseButtonObject(size, windowStyle, closeButtonType);
	EnsureContentObject(size, windowStyle);
	m_initialized = true;
}

UIWindow::~UIWindow()
{
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CORE FUNCTIONS											//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//bool UIWindow::Initialize()
//{
//	return true;
//}

void UIWindow::Update()
{
	if (!Visibility)
		return;

	if (UIWindow::IsCaptured() && m_isDragging)
	{
		Position mouseDelta = MOUSEDELTA;
		if (mouseDelta.X != 0.0f || mouseDelta.Y != 0.0f)
		{
			SetPosition(GetPosition() + mouseDelta);
		}
	}

	UIPanel::Update();
}

void UIWindow::Render()
{
	if (!Visibility)
		return;

	UIPanel::Render();
}



// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PUBLIC VIRTUAL FUNCTIONS								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void UIWindow::AddChild(UIElement* element)
{
	if (!m_initialized)
		UIPanel::AddChild(element);
	else
		m_content->AddChild(element);
}

bool UIWindow::ContainsChild(UIElement* element)
{
	if (!m_initialized)
		return UIPanel::ContainsChild(element);
	else
		return m_content->ContainsChild(element);
}

bool UIWindow::RemoveChild(UIElement* element)
{
	if (!m_initialized)
		return UIPanel::RemoveChild(element);
	else
		return m_content->RemoveChild(element);
}

bool UIWindow::RemoveChild(int index)
{
	if (!m_initialized)
		return UIPanel::RemoveChild(index);
	else
		return m_content->RemoveChild(index);
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CLASS API												//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void UIWindow::Open()
{
	if (m_openAtInitialPosition)
	{
		SetPosition(m_initialPosition);
	}
	Visibility = true;
}

void UIWindow::Close()
{
	Visibility = false;
}

void UIWindow::Destroy()
{
	RemoveFocus(FocusType::FOCUS_TYPE_UI);
	UI->RemoveWindow(this);
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
void UIWindow::LeftClickInternal(float mouseX, float mouseY)
{
	if (m_dragType == WindowDrag::DRAG_NONE)
		return;

	if (m_isDragging)
		return;

	float left = m_absolutePosition.X;
	float right = left + m_size.Width;
	float top = m_absolutePosition.Y;
	float bottom = top + m_size.Height;

	if (m_dragType == WindowDrag::DRAG_TITLE)
		bottom = top + m_titleBar->GetSize().Height;

	if (mouseX >= left && mouseX <= right && mouseY >= top && mouseY <= bottom)
	{
		//ms_previousMouseDragPosition = { mouseX, mouseY };
		ms_captured = this;
		m_isDragging = true;
	}
}

void UIWindow::LeftReleaseInternal(float mouseX, float mouseY)
{
	ms_captured = nullptr;
	m_isDragging = false;
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PRIVATE FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void UIWindow::AddStructureChild(UIElement* element)
{
	if (element == nullptr)
		return;
	UIPanel::AddChild(element);
}

void UIWindow::EnsureTitleObject(Size size, const WindowStyle& windowStyle, const std::wstring& text, bool hasCloseButton)
{
	if (m_titleBar != nullptr)
		return;

	float sizeX = size.Width - (hasCloseButton ? windowStyle.closeButtonSize : 0);
	float sizeY = windowStyle.titleBarHeight;

	m_titleBar = new UIPanel();
	m_titleBar->SetPosition(0.0f, 0.0f);
	m_titleBar->SetSize(sizeX, sizeY);
	AddChild(m_titleBar);

	UIImage* titleImage = new UIImage();
	titleImage->SetPosition(0.0f, 0.0f);
	titleImage->SetSize(size.Width, sizeY);
	titleImage->SetBorderSize(windowStyle.borderSize);
	if (windowStyle.titleBarTexture.empty())
	{
		titleImage->SetBackgroundColor(windowStyle.titleBarColor);
		titleImage->SetBackgroundBorderColor(windowStyle.borderColor);
	}
	else
	{
		titleImage->SetImage(TEXTURE(windowStyle.titleBarTexture));
	}
	m_titleBar->AddChild(titleImage);

	UIText* titleText = new UIText();
	titleText->SetPosition(0.0f, windowStyle.borderSize + 1.0f);
	titleText->SetSize(sizeX, sizeY - (windowStyle.borderSize * 2.0f) - 2.0f);
	titleText->SetFont(FONT(windowStyle.titleBarFont));
	titleText->SetFontSizeTotal(sizeY - (windowStyle.borderSize * 2.0f));
	titleText->SetTextAlignment(HorizontalTextAlignment::HORIZONTAL_TEXT_ALIGNMENT_CENTER, VerticalTextAlignment::VERTICAL_TEXT_ALIGNMENT_MIDDLE);
	titleText->SetText(text);
	m_titleBar->AddChild(titleText);

	titleImage = nullptr;
	titleText = nullptr;
}

void UIWindow::EnsureCloseButtonObject(Size size, const WindowStyle& windowStyle, WindowCloseButton closeButtonType)
{
	if (m_closeButton != nullptr)
		return;

	m_closeButton = new UIButton();
	m_closeButton->SetPosition(size.Width - windowStyle.closeButtonSize, 0.0f);
	m_closeButton->SetSize(windowStyle.closeButtonSize, windowStyle.closeButtonSize);
	m_closeButton->SetImage(TEXTURE(windowStyle.closeButtonTexture + L"_disabled"), ButtonState::BUTTON_STATE_DISABLED);
	m_closeButton->SetImage(TEXTURE(windowStyle.closeButtonTexture + L"_idle"), ButtonState::BUTTON_STATE_IDLE);
	m_closeButton->SetImage(TEXTURE(windowStyle.closeButtonTexture + L"_hover"), ButtonState::BUTTON_STATE_HOVER);
	m_closeButton->SetImage(TEXTURE(windowStyle.closeButtonTexture + L"_pressed"), ButtonState::BUTTON_STATE_PRESSED);
	
	if (closeButtonType == WindowCloseButton::CLOSEBUTTON_CLOSE)
		m_closeButton->OnClick += [&](UIButton* sender) { Close(); };
	else
		m_closeButton->OnClick += [&](UIButton* sender) { Destroy(); };

	AddChild(m_closeButton);
}

void UIWindow::EnsureGraphicObject(Size size, const WindowStyle& windowStyle)
{
	if (m_graphicTexture != nullptr)
		return;

	m_graphicTexture = new UIImage();
	m_graphicTexture->SetPosition(0.0f, 0.0f);
	m_graphicTexture->SetSize(size);
	m_graphicTexture->SetBorderSize(windowStyle.borderSize);
	m_graphicTexture->SetImage(TEXTURE(windowStyle.backgroundTexture));
	AddChild(m_graphicTexture);
}

void UIWindow::EnsureContentObject(Size size, const WindowStyle& windowStyle)
{
	if (m_content != nullptr)
		return;

	m_content = new UIPanel();
	m_content->SetPosition(windowStyle.borderSize, windowStyle.titleBarHeight);
	m_content->SetSize(size.Width - (windowStyle.borderSize * 2), size.Height - windowStyle.borderSize - windowStyle.titleBarHeight);
	AddChild(m_content);
}
