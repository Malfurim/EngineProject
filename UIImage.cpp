#include "UIImage.h"

// --- ADDITIONAL INCLUDES ---
#include "DirectXManager.h"
#include "TextureShader.h"
#include "Texture.h"
#include "TimerManager.h"
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
	EnsureVertexMemory(sizeof(SPTVertex), 4, 6, 0);
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

	if (m_animated)
	{
		m_frameDelta += DELTA;
		if (m_frameDelta >= m_frameInterval)
		{
			m_frameDelta -= m_frameInterval;
			m_currentFrame++;
			if (m_currentFrame >= m_frames)
				m_currentFrame = 0;
			Invalidate(UpdateFlag::UPDATE_GEOMETRY);
		}
	}

	UIElement::Update();
}

void UIImage::Render()
{
	if (!Visibility)
		return;
	UIElement::Render();

	if (m_texture == nullptr)
		return;

	unsigned int stride = m_vertexSize;
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
void UIImage::EnableAnimation(unsigned int framesPerRow, unsigned int framesPerColumn, unsigned int framesPerSecond)
{
	if (framesPerSecond == 0)
		return;

	m_animated = true;
	m_textureFramesPerRow = (framesPerRow < 1) ? 1 : framesPerRow;
	m_textureFramesPerColumn = (framesPerColumn < 1) ? 1 : framesPerColumn;

	m_frames = m_textureFramesPerRow * m_textureFramesPerColumn;
	m_frameInterval = 1.0 / static_cast<double>(framesPerSecond);
	m_frameDelta = 0.0;
	m_currentFrame = 0;

	Invalidate(UpdateFlag::UPDATE_GEOMETRY);
}

void UIImage::DisableAnimation()
{
	m_animated = false;
	m_textureFramesPerRow = 1;
	m_textureFramesPerColumn = 1;
}

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
	SPTVertex* vertices = (SPTVertex*)m_vertices;

	float cellWidth = 1.0f / static_cast<float>(m_textureFramesPerRow);
	float cellHeight = 1.0f / static_cast<float>(m_textureFramesPerColumn);

	float uMin = static_cast<float>(m_currentFrame % m_textureFramesPerRow) * cellWidth;
	float uMax = uMin + cellWidth;
	float vMin = static_cast<float>(m_currentFrame / m_textureFramesPerRow) * cellHeight;
	float vMax = vMin + cellHeight;

	if (m_borderSize == 0.0f)
	{
		float left = floorf(m_absolutePosition.X);
		float right = floorf(m_absolutePosition.X + m_size.Width);
		float top = floorf(m_absolutePosition.Y);
		float bottom = floorf(m_absolutePosition.Y + m_size.Height);

		vertices[0].position = TPosition{ left, top, 0.0f };
		vertices[0].texCoord = TTextureCoordinate{ uMin, vMin };

		vertices[1].position = TPosition{ right, top, 0.0f };
		vertices[1].texCoord = TTextureCoordinate{ uMax, vMin };

		vertices[2].position = TPosition{ left, bottom, 0.0f };
		vertices[2].texCoord = TTextureCoordinate{ uMin, vMax };

		vertices[3].position = TPosition{ right, bottom, 0.0f };
		vertices[3].texCoord = TTextureCoordinate{ uMax, vMax };
	}
	else
	{
		float framesWidthPixels = m_texture->GetWidth() * cellWidth;
		float framesHeightPixels = m_texture->GetHeight() * cellHeight;

		// { position }, { texcoord }
		float Xvalues[4][2]
		{
			{ { floorf(m_absolutePosition.X)								},	{ uMin										} },
			{ { floorf(m_absolutePosition.X + m_borderSize)					},	{ uMin + m_borderSize / framesWidthPixels	} },
			{ { floorf(m_absolutePosition.X + m_size.Width - m_borderSize)	},	{ uMax - m_borderSize / framesWidthPixels	} },
			{ { floorf(m_absolutePosition.X + m_size.Width)					},	{ uMax										} }
		};
		float Yvalues[4][2]
		{
			{ { floorf(m_absolutePosition.Y)								},	{ vMin										} },
			{ { floorf(m_absolutePosition.Y + m_borderSize)					},	{ vMin + m_borderSize / framesHeightPixels	} },
			{ { floorf(m_absolutePosition.Y + m_size.Height - m_borderSize)	},	{ vMax - m_borderSize / framesHeightPixels	} },
			{ { floorf(m_absolutePosition.Y + m_size.Height)				},	{ vMax										} }
		};

		int rect = 0;
		for (unsigned int vertexIndex = 0; vertexIndex < m_vertexCount; vertexIndex += 4)
		{
			int x = rect % 3;
			int y = rect / 3;

			vertices[vertexIndex + 0].position = { Xvalues[x + 0][0], Yvalues[y + 0][0], 0.0f };
			vertices[vertexIndex + 0].texCoord = { Xvalues[x + 0][1], Yvalues[y + 0][1] };

			vertices[vertexIndex + 1].position = { Xvalues[x + 1][0], Yvalues[y + 0][0], 0.0f };
			vertices[vertexIndex + 1].texCoord = { Xvalues[x + 1][1], Yvalues[y + 0][1] };

			vertices[vertexIndex + 2].position = { Xvalues[x + 0][0], Yvalues[y + 1][0], 0.0f };
			vertices[vertexIndex + 2].texCoord = { Xvalues[x + 0][1], Yvalues[y + 1][1] };

			vertices[vertexIndex + 3].position = { Xvalues[x + 1][0], Yvalues[y + 1][0], 0.0f };
			vertices[vertexIndex + 3].texCoord = { Xvalues[x + 1][1], Yvalues[y + 1][1] };

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

