#include "UIElement.h"
#include "Settings.h"
#include "DirectXManager.h"
#include "InputManager.h"
#include "Utils.h"

XMMATRIX UIElement::ms_worldMatrix = XMMatrixIdentity();
XMMATRIX UIElement::ms_viewMatrix = XMMatrixMultiply(XMMatrixIdentity(), XMMatrixTranslation(0.0f, 0.0f, 1.0f));
XMMATRIX UIElement::ms_orthoMatrix = XMMatrixOrthographicOffCenterLH(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, SCREEN_NEAR, SCREEN_DEPTH);

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
	LoadBuffers();
	SetRect(position, size);
	SetBackgroundColor(backgroundColor);
}

UIElement::~UIElement()
{
	m_parent = nullptr;
}

void UIElement::Render()
{
	GraphicBase::Render(ms_worldMatrix, ms_viewMatrix, ms_orthoMatrix);
}

void UIElement::Update()
{
	UpdateAbsolutePosition();

	if (!m_changed)
		return;

	float left = m_absolutePosition.X;
	float right = left + m_size.Width;
	float top = m_absolutePosition.Y;
	float bottom = top + m_size.Height;

	m_vertices[0].position = TPosition{ left, top, 0.0f };
	m_vertices[0].texCoord = TTextureCoordinate{ 0.0f, 0.0f };

	m_vertices[1].position = TPosition{ right, top, 0.0f };
	m_vertices[1].texCoord = TTextureCoordinate{ 1.0f, 0.0f };

	m_vertices[2].position = TPosition{ left, bottom, 0.0f };
	m_vertices[2].texCoord = TTextureCoordinate{ 0.0f, 1.0f };

	m_vertices[3].position = TPosition{ right, bottom, 0.0f };
	m_vertices[3].texCoord = TTextureCoordinate{ 1.0f, 1.0f };


	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	result = DXDEVICECONTEXT->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return;
	}
	
	memcpy((SPDNT2Vertex*)mappedResource.pData, (void*)m_vertices, VERTEXSTRUCTSIZE * m_vertexCount);
		
	DXDEVICECONTEXT->Unmap(m_vertexBuffer, 0);

	GraphicBase::Update();
	m_changed = false;
}

const UIElement* UIElement::GetParent()
{
	return m_parent;
}

void UIElement::SetPosition(Position position)
{ 
	m_localPosition = position;
	UpdateAbsolutePosition();
	m_changed = true;
}
void UIElement::SetPosition(float x, float y)
{
	m_localPosition = { x, y };
	UpdateAbsolutePosition();
	m_changed = true;
}

void UIElement::SetRect(float posX, float posY, float sizeX, float sizeY)
{
	SetRect({ posX, posY }, { sizeX, sizeY });
}

void UIElement::SetRect(Position position, Size size)
{
	m_localPosition = position;
	m_size = size;
	UpdateAbsolutePosition();
	m_changed = true;
}

void UIElement::SetParent(UIElement* element)
{
	m_parent = element;
	UpdateAbsolutePosition();
	m_changed = true;
}

void UIElement::SetZIndex(int index)
{
	m_zIndex = index;
	m_changed = true;
	if (m_parent == nullptr)
		return;
	m_parent->SortChildren();
}

int UIElement::GetZIndex()
{
	return m_zIndex;
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
		m_changed = true;
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
	}
}

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

