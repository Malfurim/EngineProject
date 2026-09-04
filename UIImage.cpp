#include "UIImage.h"
#include "DirectXManager.h"
#include "Utils.h"

TextureShader* UIImage::ms_imageShader = nullptr;

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

void UIImage::Shutdown()
{
	SAFE_DELETE(ms_imageShader);
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

void UIImage::Update()
{
	if (!Visibility)
		return;
	if (m_borderSize == 0.0f)
		UIElement::Update();
	else
	{
		UpdateAbsolutePosition();
		if (!m_changed)
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

		GraphicBase::Update();
		m_changed = false;
	}
}

void UIImage::SetSize(Size size)
{
	m_size = size;
	m_changed = true;
	CheckMinimalSize();
}

void UIImage::SetSize(float width, float height)
{
	m_size = { width, height };
	m_changed = true;
	CheckMinimalSize();
}

void UIImage::CheckMinimalSize()
{
	if (m_size.Width < m_borderSize * 2)
		m_size.Width = m_borderSize * 2;
	if (m_size.Height < m_borderSize * 2)
		m_size.Height = m_borderSize * 2;
}

void UIImage::SetImage(Texture* texture)
{
	if (texture != nullptr)
	{
		m_texture = texture;
		m_changed = true;
	}
}

void UIImage::SetBorder(float size)
{
	if (m_borderSize == size)
		return;

	m_borderSize = size;
	m_changed = true;

	(m_borderSize == 0.0f) ? EnsureVertexMemory(4, 6, 0) : EnsureVertexMemory(36, 54, 0);
	LoadBuffers();
}

void UIImage::ConfirmShader()
{
	if (ms_imageShader == nullptr)
		ms_imageShader = new TextureShader();
}

void UIImage::CalculateGeometry()
{
	// { position }, { texcoord }
	float Xvalues[4][2]
	{
		{ { m_absolutePosition.X									},	{ 0.0f											} },
		{ { m_absolutePosition.X + m_borderSize						},	{ 0.0f + m_borderSize / m_texture->GetWidth()	} },
		{ { m_absolutePosition.X + m_size.Width - m_borderSize		},	{ 1.0f - m_borderSize / m_texture->GetWidth()	} },
		{ { m_absolutePosition.X + m_size.Width						},	{ 1.0f											} }
	};
	float Yvalues[4][2]
	{
		{ { m_absolutePosition.Y									},	{ 0.0f											} },
		{ { m_absolutePosition.Y + m_borderSize						},	{ 0.0f + m_borderSize / m_texture->GetHeight()	} },
		{ { m_absolutePosition.Y + m_size.Height - m_borderSize		},	{ 1.0f - m_borderSize / m_texture->GetHeight()	} },
		{ { m_absolutePosition.Y + m_size.Height					},	{ 1.0f											} }
	};

	int rect = 0;
	for (int vertexIndex = 0; vertexIndex < m_vertexCount; vertexIndex += 4)
	{
		int x = rect % 3;
		int y = rect / 3;

		m_vertices[vertexIndex + 0].position = { Xvalues[x + 0][0], Yvalues[y + 0][0], 0.0f};
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