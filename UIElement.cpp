#include "UIElement.h"

// --- ADDITIONAL INCLUDES ---
#include "Settings.h"
#include "DirectXManager.h"
#include "InputManager.h"
#include "Deleters.h"
#include "Utils.h"

// --- MACROS & DEFINES ---


// --- FORWARD DECLARATIONS ---

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC GLOBAL STATES & DATA								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
XMMATRIX UIElement::ms_worldMatrix = XMMatrixIdentity();
XMMATRIX UIElement::ms_viewMatrix = XMMatrixMultiply(XMMatrixIdentity(), XMMatrixTranslation(0.0f, 0.0f, 1.0f));
XMMATRIX UIElement::ms_orthoMatrix = XMMatrixOrthographicOffCenterLH(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, SCREEN_NEAR, SCREEN_DEPTH);

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CONSTRUCTORS & DESTRUCTOR								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
UIElement::UIElement() : UIElement({ 0, 0 }, { 0, 0 }, { 0, 0, 0, 0 })
{
}

UIElement::UIElement(Position position, Size size) : UIElement(position, size, { 0, 0, 0, 0 })
{
}

UIElement::UIElement(Position position, Size size, Color backgroundColor)
{
	SetRect(position, size);
	SetBackgroundColor(backgroundColor);
}

UIElement::~UIElement()
{
	m_parent = nullptr;
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CORE FUNCTIONS											//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//bool UIElement::Initialize()
//{
//	return true;
//}

void UIElement::Update()
{
	if (!Visibility)
		return;

	if (!IsInvalidated())
		return;

	if (IsInvalidated(UpdateFlag::UPDATE_POSITION))
	{
		UpdateAbsolutePosition();
		Validate(UpdateFlag::UPDATE_POSITION);
	}

	if (IsInvalidated(UpdateFlag::UPDATE_BACKGROUND_STATE))
	{
		CheckBackgroundType();
		LoadBackgroundBuffer();

		if (m_background != BackgroundType::BACKGROUND_NONE)
		{
			Invalidate(UpdateFlag::UPDATE_GEOMETRY);
			Invalidate(UpdateFlag::UPDATE_COLORS);
		}

		Validate(UpdateFlag::UPDATE_BACKGROUND_STATE);
	}

	if (IsInvalidated(UpdateFlag::UPDATE_GEOMETRY))
	{
		CalculateGeometry();
		UpdateBufferData(m_vertexBuffer, m_vertices, m_vertexSize * m_vertexCount);
		GraphicBase::UpdateGeometry();
		Validate(UpdateFlag::UPDATE_GEOMETRY);
	}

	if (IsInvalidated(UpdateFlag::UPDATE_COLORS))
	{
		GraphicBase::UpdateColors(m_backgroundBorderColor);
		Validate(UpdateFlag::UPDATE_COLORS);
	}

	GraphicBase::Update();
}

void UIElement::Render()
{
	GraphicBase::Render(ms_worldMatrix, ms_viewMatrix, ms_orthoMatrix);
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PUBLIC VIRTUAL FUNCTIONS								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CLASS API												//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	GETTERS & SETTERS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void UIElement::SetPosition(Position position)
{
	m_localPosition = position;
	Invalidate(UpdateFlag::UPDATE_POSITION);
}

void UIElement::SetPosition(float x, float y)
{
	m_localPosition = { x, y };
	Invalidate(UpdateFlag::UPDATE_POSITION);
}

void UIElement::SetBackgroundBorderColor(Color color)
{
	if (m_backgroundBorderColor == color)
		return;
	m_backgroundBorderColor = color;
	Invalidate(UpdateFlag::UPDATE_COLORS);
}

void UIElement::SetBorderSize(float size)
{
	if (size < 0.0f)
		size = 0.0f;
	if (m_borderSize == size)
		return;
	m_borderSize = size;
	(m_borderSize == 0.0f) ? EnsureVertexMemory(0, 4, 6, 0) : EnsureVertexMemory(0, 36, 54, 0);
	Invalidate(UpdateFlag::UPDATE_BACKGROUND_STATE);
	Invalidate(UpdateFlag::UPDATE_GEOMETRY);
	Invalidate(UpdateFlag::UPDATE_COLORS);
}

void UIElement::SetZIndex(int index)
{
	m_zIndex = index;
	Invalidate();
	if (m_parent == nullptr)
		return;
	m_parent->SortChildren();
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC CLASS API										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED VIRTUAL FUNCTIONS								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
bool UIElement::IsMouseOver() const
{
	if (!Visibility)
		return false;

	float mouseX = MOUSEX;
	float mouseY = MOUSEY;

	float left = m_absolutePosition.X;
	float right = left + m_size.Width;
	float top = m_absolutePosition.Y;
	float bottom = top + m_size.Height;

	return (mouseX >= left && mouseX <= right && mouseY >= top && mouseY <= bottom);
}

void UIElement::CalculateGeometry()
{
	uint8_t* vertices = (uint8_t*)m_vertices;

	auto GetPositionPtr = [&](unsigned int index) -> TPosition* {
		return (TPosition*)(vertices + (index * m_vertexSize));
		};


	if (m_borderSize == 0.0f)
	{
		float left = floorf(m_absolutePosition.X);
		float right = floorf(m_absolutePosition.X + m_size.Width);
		float top = floorf(m_absolutePosition.Y);
		float bottom = floorf(m_absolutePosition.Y + m_size.Height);

		*GetPositionPtr(0) = TPosition{ left, top, 0.0f };
		*GetPositionPtr(1) = TPosition{ right, top, 0.0f };
		*GetPositionPtr(2) = TPosition{ left, bottom, 0.0f };
		*GetPositionPtr(3) = TPosition{ right, bottom, 0.0f };
	}
	else
	{
		// { position }
		float Xvalues[4]
		{
			floorf(m_absolutePosition.X),
			floorf(m_absolutePosition.X + m_borderSize),
			floorf(m_absolutePosition.X + m_size.Width - m_borderSize),
			floorf(m_absolutePosition.X + m_size.Width)
		};
		float Yvalues[4]
		{
			floorf(m_absolutePosition.Y),
			floorf(m_absolutePosition.Y + m_borderSize),
			floorf(m_absolutePosition.Y + m_size.Height - m_borderSize),
			floorf(m_absolutePosition.Y + m_size.Height)
		};

		int rect = 0;
		for (int vertexIndex = 0; vertexIndex < m_vertexCount; vertexIndex += 4)
		{
			int x = rect % 3;
			int y = rect / 3;

			*GetPositionPtr(vertexIndex + 0) = { Xvalues[x + 0], Yvalues[y + 0], 0.0f };
			*GetPositionPtr(vertexIndex + 1) = { Xvalues[x + 1], Yvalues[y + 0], 0.0f };
			*GetPositionPtr(vertexIndex + 2) = { Xvalues[x + 0], Yvalues[y + 1], 0.0f };
			*GetPositionPtr(vertexIndex + 3) = { Xvalues[x + 1], Yvalues[y + 1], 0.0f };

			rect++;
		}
	}
}

void UIElement::UpdateAbsolutePosition()
{
	Position absolutePosition = m_localPosition;

	if (m_parent != nullptr)
	{
		absolutePosition.X += m_parent->GetAbsoluteLeft();
		absolutePosition.Y += m_parent->GetAbsoluteTop();
	}

	if (absolutePosition.X != m_absolutePosition.X || absolutePosition.Y != m_absolutePosition.Y)
	{
		m_absolutePosition = absolutePosition;
		Invalidate(UpdateFlag::UPDATE_GEOMETRY);
	}
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void UIElement::SetRect(float posX, float posY, float sizeX, float sizeY)
{
	SetRect({ posX, posY }, { sizeX, sizeY });
}

void UIElement::SetRect(Position position, Size size)
{
	m_localPosition = position;
	m_size = size;
	Invalidate(UpdateFlag::UPDATE_POSITION);
}

void UIElement::SetParent(UIElement* element)
{
	m_parent = element;
	Invalidate(UpdateFlag::UPDATE_POSITION);
}

void UIElement::EnsureVertexMemory(unsigned int vertexSize, unsigned int vertexCount, unsigned int indexCount, unsigned int reserveCount)
{
	unsigned int activeVertexSize = (vertexSize == 0) ? m_vertexSize : vertexSize;
	if (m_vertices != nullptr && m_vertexCapacity > vertexCount && m_indexCapacity > indexCount && m_vertexSize == activeVertexSize)
		return;

	m_dynamic = true;

	uint8_t* vertices = (uint8_t*)m_vertices;
	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(m_indices);

	m_vertexSize = activeVertexSize;
	m_vertexCapacity = vertexCount + reserveCount * 4;
	m_indexCapacity = indexCount + reserveCount * 6;

	unsigned int byteSize = m_vertexCapacity * activeVertexSize;
	m_vertices = (void*)new uint8_t[byteSize];
	memset(m_vertices, 0, byteSize);
	m_indices = new unsigned int[m_indexCapacity];

	if (m_vertexBufferBackground != nullptr)
	{
		SAFE_DELETE_ARRAY(m_verticesBackground);
		SAFE_RELEASE(m_vertexBufferBackground);
		m_verticesBackground = new SPCVertex[m_vertexCapacity];
	}

	for (unsigned int i = 0; i < m_indexCapacity; i += 6)
	{
		unsigned int rect = (i / 6) * 4;
		m_indices[i + 0] = rect + 0;
		m_indices[i + 1] = rect + 1;
		m_indices[i + 2] = rect + 2;
		m_indices[i + 3] = rect + 3;
		m_indices[i + 4] = rect + 2;
		m_indices[i + 5] = rect + 1;
	}

	m_vertexCount = vertexCount;
	m_indexCount = indexCount;

	LoadBuffers();
}

void UIElement::CheckBackgroundType()
{
	if (m_borderSize == 0.0f)
	{
		if (m_backgroundColor.a == 0.0f)
		{
			m_background = BackgroundType::BACKGROUND_NONE;
		}
		else
		{
			m_background = BackgroundType::BACKGROUND_COLOR;
		}
	}
	else if (m_backgroundColor.a > 0.0f)
	{
		m_background = BackgroundType::BACKGROUND_BORDER;
	}
	else
	{
		m_background = BackgroundType::BACKGROUND_BORDER_ONLY;
	}
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PRIVATE FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //

