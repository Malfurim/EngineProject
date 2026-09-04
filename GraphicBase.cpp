#include "GraphicBase.h"
#include "DirectXManager.h"
#include "ResourceManager.h"
#include "Utils.h"

Shader* GraphicBase::ms_shader = nullptr;
GraphicBase* GraphicBase::ms_focus = nullptr;

GraphicBase::GraphicBase()
{
	if (ms_shader == nullptr)
		ms_shader = new Shader();

	OnLeftClick += [this] { this->Focus(); };
}

GraphicBase::~GraphicBase()
{
	SAFE_DELETE_ARRAY(m_vertices);
	SAFE_DELETE_ARRAY(m_indices);
	SAFE_RELEASE(m_vertexBufferBackground);
	SAFE_RELEASE(m_vertexBuffer);
	SAFE_RELEASE(m_indexBuffer);
}

void GraphicBase::Shutdown()
{
	SAFE_DELETE(ms_shader);
}

void GraphicBase::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	if (m_background == BACKGROUND_NONE)
		return;
	
	unsigned int stride = sizeof(SPCVertex);
	unsigned int offset = 0;

	DXDEVICECONTEXT->IASetVertexBuffers(0, 1, &m_vertexBufferBackground, &stride, &offset);
	DXDEVICECONTEXT->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	DXDEVICECONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ms_shader->SetShaderParameters(worldMatrix, viewMatrix, projectionMatrix);
	ms_shader->Render(m_indexCount);
	ms_shader->Reset();
}

void GraphicBase::Update()
{
	if (m_background == BACKGROUND_NONE)
		return;

	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	result = DXDEVICECONTEXT->Map(m_vertexBufferBackground, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return;
	}

	SPCVertex* vertices = (SPCVertex*)mappedResource.pData;
	for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = m_vertices[i].position;
		vertices[i].color = TColor(m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, m_backgroundColor.a);
	}

	DXDEVICECONTEXT->Unmap(m_vertexBufferBackground, 0);
}

void GraphicBase::SetBackgroundColor(Color color)
{
	m_backgroundColor = color;
	if (color.a == 0.0f)		m_background = BACKGROUND_NONE;
	else if (color.a == 1.0f)	m_background = BACKGROUND_SOLID;
	else						m_background = BACKGROUND_TRANSPARENT;
	GraphicBase::Update();
}

bool GraphicBase::LoadBuffers()
{
	if (m_vertexCount == 0)
	{
		return false;
	}

	if (!SetVertexBuffer())
	{
		return false;
	}

	if (!SetIndexBuffer())
	{
		return false;
	}

	return true;
}

bool GraphicBase::SetVertexBuffer()
{
	SAFE_RELEASE(m_vertexBuffer);
	SAFE_RELEASE(m_vertexBufferBackground);

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = (m_dynamic) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = VERTEXSTRUCTSIZE * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = (m_dynamic) ? D3D11_CPU_ACCESS_WRITE : 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = m_vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	
	HRESULT result = DXDEVICE->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	vertexBufferDesc.ByteWidth = sizeof(SPCVertex) * m_vertexCount;
	result = DXDEVICE->CreateBuffer(&vertexBufferDesc, nullptr, &m_vertexBufferBackground);
	if (FAILED(result))
	{
		return false;
	}
	
	return true;
}

bool GraphicBase::SetIndexBuffer()
{
	SAFE_RELEASE(m_indexBuffer);

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = m_indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	
	HRESULT result = DXDEVICE->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	
	return true;
}

Color GraphicBase::GetBackgroundColor()
{
	return m_backgroundColor;
}

void GraphicBase::Focus()
{
	if (ms_focus == this)
		return;

	if (ms_focus != nullptr)
	{
		ms_focus->OnFocusLost.Invoke(ms_focus);
	}

	ms_focus = this;
	OnFocus.Invoke(this);

	INP->SetInputState(ms_focus->GetRequiredInputState());
}

void GraphicBase::Unfocus()
{
	MessageBox(NULL, L"un focusing something", L"unfocus", MB_OK);

	if (ms_focus == nullptr)
		return;

	ms_focus->OnFocusLost.Invoke(ms_focus);
	ms_focus = nullptr;
	INP->SetInputState(InputState::INPUT_STATE_GAMEPLAY);
}

void GraphicBase::UpdateMouseEvents()
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