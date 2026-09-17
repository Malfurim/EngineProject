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
	m_dynamic = true;
	EnsureVertexMemory(4, 6, 0);
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
	UpdateAbsolutePosition();

	if (!IsInvalidated())
		return;

	CalculateGeometry();

	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	result = DXDEVICECONTEXT->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return;
	}
	
	memcpy((SPDNT2Vertex*)mappedResource.pData, (void*)m_vertices, VERTEXSTRUCTSIZE * m_vertexCount);
		
	DXDEVICECONTEXT->Unmap(m_vertexBuffer, 0);

	(m_borderSize == 0.0f) ? GraphicBase::Update() : GraphicBase::Update(m_backgroundBorderColor);
	//GraphicBase::Update();
	Validate();
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
	UpdateAbsolutePosition();
	Invalidate();
}
void UIElement::SetPosition(float x, float y)
{
	m_localPosition = { x, y };
	UpdateAbsolutePosition();
	Invalidate();
}

void UIElement::SetBackgroundBorderColor(Color color)
{
	if (m_backgroundBorderColor == color)
		return;
	m_backgroundBorderColor = color;
	GraphicBase::Update(m_backgroundBorderColor);
}

void UIElement::SetBorderSize(float size)
{
	if (size < 0.0f)
		size = 0.0f;
	if (m_borderSize == size)
		return;
	m_borderSize = size;
	(m_borderSize == 0.0f) ? EnsureVertexMemory(4, 6, 0) : EnsureVertexMemory(36, 54, 0);
	CheckBackgroundType();
	Invalidate();
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
bool UIElement::CheckMouseOver() const
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
	if (m_borderSize == 0.0f)
	{
		float left = floorf(m_absolutePosition.X);
		float right = floorf(m_absolutePosition.X + m_size.Width);
		float top = floorf(m_absolutePosition.Y);
		float bottom = floorf(m_absolutePosition.Y + m_size.Height);

		m_vertices[0].position = TPosition{ left, top, 0.0f };
		m_vertices[1].position = TPosition{ right, top, 0.0f };
		m_vertices[2].position = TPosition{ left, bottom, 0.0f };
		m_vertices[3].position = TPosition{ right, bottom, 0.0f };
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

			m_vertices[vertexIndex + 0].position = { Xvalues[x + 0], Yvalues[y + 0], 0.0f };
			m_vertices[vertexIndex + 1].position = { Xvalues[x + 1], Yvalues[y + 0], 0.0f };
			m_vertices[vertexIndex + 2].position = { Xvalues[x + 0], Yvalues[y + 1], 0.0f };
			m_vertices[vertexIndex + 3].position = { Xvalues[x + 1], Yvalues[y + 1], 0.0f };

			rect++;
		}
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
	UpdateAbsolutePosition();
	Invalidate();
}

void UIElement::SetParent(UIElement* element)
{
	m_parent = element;
	UpdateAbsolutePosition();
	Invalidate();
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
		Invalidate();
	}
}

void UIElement::EnsureVertexMemory(unsigned int vertexCount, unsigned int indexCount, unsigned int reserveCount)
{
	if (m_vertices == nullptr || vertexCount > m_vertexCapacity || indexCount > m_indexCapacity)
	{
		SAFE_DELETE_ARRAY(m_vertices);
		SAFE_DELETE_ARRAY(m_indices);

		m_vertexCapacity = vertexCount + reserveCount * 4;
		m_indexCapacity = indexCount + reserveCount * 6;
		m_vertices = new SPDNT2Vertex[m_vertexCapacity];
		m_indices = new unsigned long[m_indexCapacity];

		for (unsigned long i = 0; i < m_indexCapacity; i += 6)
		{
			unsigned long rect = (i / 6) * 4;
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
}

void UIElement::CheckBackgroundType()
{
	if (m_backgroundColor.a == 0.0f)
		m_background = BackgroundType::BACKGROUND_NONE;
	else if (m_borderSize == 0.0f)
		m_background = BackgroundType::BACKGROUND_COLOR;
	else
		m_background = BackgroundType::BACKGROUND_BORDER;
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PRIVATE FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //

