#include "UIImage.h"

// --- ADDITIONAL INCLUDES ---
#include "DirectXManager.h"
#include "TextureShader.h"
#include "Texture.h"
#include "Deleters.h"

// --- MACROS & DEFINES ---


// --- FORWARD DECLARATIONS ---


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC GLOBAL STATES & DATA								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
TextureShader* UIImage::ms_imageShader = nullptr;

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CONSTRUCTORS & DESTRUCTOR								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
UIImage::UIImage() : UIImage({ 0,0 }, { 0,0 }, { 0, 0, 0, 0 })
{
}

UIImage::UIImage(Position position, Size size) : UIImage(position, size, { 0, 0, 0, 0 })
{
}

UIImage::UIImage(Position position, Size size, Color backgroundColor) : UIElement(position, size, backgroundColor)
{
	ConfirmShader();
}

UIImage::~UIImage()
{
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CORE FUNCTIONS											//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//bool UIImage::Initialize()
//{
//	return true;
//}

void UIImage::Update()
{
	if (!Visibility)
		return;
	UIElement::Update();
}

void UIImage::Render()
{
	if (!Visibility)
		return;
	UIElement::Render();

	if (m_texture == nullptr)
		return;

	unsigned int stride = VERTEXSTRUCTSIZE;
	unsigned int offset = 0;

	DXDEVICECONTEXT->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	DXDEVICECONTEXT->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	DXDEVICECONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ms_imageShader->SetShaderParameters(m_texture->GetTexture());
	ms_imageShader->SetShaderParameters(ms_worldMatrix, ms_viewMatrix, ms_orthoMatrix);
	ms_imageShader->Render(m_indexCount);
	ms_imageShader->Reset();
}

void UIImage::Shutdown()
{
	SAFE_DELETE(ms_imageShader);
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
void UIImage::SetSize(Size size)
{
	m_size = size;
	Invalidate();
	CheckMinimalSize();
}

void UIImage::SetSize(float width, float height)
{
	m_size = { width, height };
	Invalidate();
	CheckMinimalSize();
}

void UIImage::SetImage(Texture* texture)
{
	if (texture != nullptr)
	{
		m_texture = texture;
		Invalidate();
	}
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC CLASS API										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED VIRTUAL FUNCTIONS								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void UIImage::CalculateGeometry()
{
	if (m_borderSize == 0.0f)
	{
		float left = floorf(m_absolutePosition.X);
		float right = floorf(m_absolutePosition.X + m_size.Width);
		float top = floorf(m_absolutePosition.Y);
		float bottom = floorf(m_absolutePosition.Y + m_size.Height);

		m_vertices[0].position = TPosition{ left, top, 0.0f };
		m_vertices[0].texCoord = TTextureCoordinate{ 0.0f, 0.0f };

		m_vertices[1].position = TPosition{ right, top, 0.0f };
		m_vertices[1].texCoord = TTextureCoordinate{ 1.0f, 0.0f };

		m_vertices[2].position = TPosition{ left, bottom, 0.0f };
		m_vertices[2].texCoord = TTextureCoordinate{ 0.0f, 1.0f };

		m_vertices[3].position = TPosition{ right, bottom, 0.0f };
		m_vertices[3].texCoord = TTextureCoordinate{ 1.0f, 1.0f };
	}
	else
	{
		// { position }, { texcoord }
		float Xvalues[4][2]
		{
			{ { floorf(m_absolutePosition.X)								},	{ 0.0f											} },
			{ { floorf(m_absolutePosition.X + m_borderSize)					},	{ 0.0f + m_borderSize / m_texture->GetWidth()	} },
			{ { floorf(m_absolutePosition.X + m_size.Width - m_borderSize)	},	{ 1.0f - m_borderSize / m_texture->GetWidth()	} },
			{ { floorf(m_absolutePosition.X + m_size.Width)					},	{ 1.0f											} }
		};
		float Yvalues[4][2]
		{
			{ { floorf(m_absolutePosition.Y)								},	{ 0.0f											} },
			{ { floorf(m_absolutePosition.Y + m_borderSize)					},	{ 0.0f + m_borderSize / m_texture->GetHeight()	} },
			{ { floorf(m_absolutePosition.Y + m_size.Height - m_borderSize)	},	{ 1.0f - m_borderSize / m_texture->GetHeight()	} },
			{ { floorf(m_absolutePosition.Y + m_size.Height)				},	{ 1.0f											} }
		};

		int rect = 0;
		for (int vertexIndex = 0; vertexIndex < m_vertexCount; vertexIndex += 4)
		{
			int x = rect % 3;
			int y = rect / 3;

			m_vertices[vertexIndex + 0].position = { Xvalues[x + 0][0], Yvalues[y + 0][0], 0.0f };
			m_vertices[vertexIndex + 0].texCoord = { Xvalues[x + 0][1], Yvalues[y + 0][1] };

			m_vertices[vertexIndex + 1].position = { Xvalues[x + 1][0], Yvalues[y + 0][0], 0.0f };
			m_vertices[vertexIndex + 1].texCoord = { Xvalues[x + 1][1], Yvalues[y + 0][1] };

			m_vertices[vertexIndex + 2].position = { Xvalues[x + 0][0], Yvalues[y + 1][0], 0.0f };
			m_vertices[vertexIndex + 2].texCoord = { Xvalues[x + 0][1], Yvalues[y + 1][1] };

			m_vertices[vertexIndex + 3].position = { Xvalues[x + 1][0], Yvalues[y + 1][0], 0.0f };
			m_vertices[vertexIndex + 3].texCoord = { Xvalues[x + 1][1], Yvalues[y + 1][1] };

			rect++;
		}
	}
}


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PRIVATE FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void UIImage::CheckMinimalSize()
{
	if (m_size.Width < m_borderSize * 2)
		m_size.Width = m_borderSize * 2;
	if (m_size.Height < m_borderSize * 2)
		m_size.Height = m_borderSize * 2;
}

void UIImage::ConfirmShader()
{
	if (ms_imageShader == nullptr)
		ms_imageShader = new TextureShader();
}

