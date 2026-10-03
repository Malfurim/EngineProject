#include "GraphicBase.h"

// --- ADDITIONAL INCLUDES ---
#include <d3d11.h>
#include <fstream>
#include "DirectXManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Shader.h"
#include "Deleters.h"

// --- MACROS & DEFINES ---


// --- FORWARD DECLARATIONS ---


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC GLOBAL STATES & DATA								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
Shader* GraphicBase::ms_shader = nullptr;

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CONSTRUCTORS & DESTRUCTOR								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
GraphicBase::GraphicBase()
{
	if (ms_shader == nullptr)
		ms_shader = new Shader();
}

GraphicBase::~GraphicBase()
{
	SAFE_DELETE_ARRAY(m_vertices);
	SAFE_DELETE_ARRAY(m_indices);
	SAFE_RELEASE(m_vertexBuffer);
	SAFE_RELEASE(m_indexBuffer);
	SAFE_DELETE_ARRAY(m_verticesBackground);
	SAFE_RELEASE(m_vertexBufferBackground);
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CORE FUNCTIONS											//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//bool GraphicBase::Initialize()
//{
//	return true;
//}

void GraphicBase::Update()
{
	if (m_background == BACKGROUND_NONE)
		return;

	UpdateBufferData(m_vertexBufferBackground, m_verticesBackground, sizeof(SPCVertex) * m_vertexCount);
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

void GraphicBase::Shutdown()
{
	SAFE_DELETE(ms_shader);
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	VIRTUAL FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CLASS API												//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	GETTERS & SETTERS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void GraphicBase::SetBackgroundColor(Color color)
{
	m_backgroundColor = color;
	if (m_background != BackgroundType::BACKGROUND_BORDER)
	{
		if (color.a == 0.0f)
			m_background = BackgroundType::BACKGROUND_NONE;
		else
			m_background = BackgroundType::BACKGROUND_COLOR;
	}
	Invalidate(UpdateFlag::UPDATE_BACKGROUND_STATE);
	Invalidate(UpdateFlag::UPDATE_COLORS);
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC CLASS API										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	VIRTUAL FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void GraphicBase::UpdateGeometry()
{
	if (m_background == BackgroundType::BACKGROUND_NONE || m_verticesBackground == nullptr || m_vertices == nullptr)
		return;

	uint8_t* vertices = (uint8_t*)m_vertices;
	for (int i = 0; i < m_vertexCount; i++)
	{
		m_verticesBackground[i].position = *(TPosition*)(vertices + (i * m_vertexSize));
	}
}

void GraphicBase::UpdateColors(Color borderColor)
{
	if (m_background == BackgroundType::BACKGROUND_NONE || m_verticesBackground == nullptr)
		return;

	if ((m_background == BackgroundType::BACKGROUND_BORDER || m_background == BackgroundType::BACKGROUND_BORDER_ONLY) && m_vertexCount == 36)
	{
		TColor finalBgColor = (m_background == BackgroundType::BACKGROUND_BORDER_ONLY)
			? TColor(0.0f, 0.0f, 0.0f, 0.0f)
			: TColor(m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, m_backgroundColor.a);

		TColor finalBorderColor = TColor(borderColor.r, borderColor.g, borderColor.b, borderColor.a);

		for (int i = 0; i < m_vertexCount; i++)
		{
			if (i >= 16 && i <= 19)
			{
				m_verticesBackground[i].color = finalBgColor;
			}
			else
			{
				m_verticesBackground[i].color = finalBorderColor;
			}
		}
	}
	else
	{
		TColor finalBgColor = TColor(m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, m_backgroundColor.a);
		for (int i = 0; i < m_vertexCount; i++)
		{
			m_verticesBackground[i].color = TColor(m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, m_backgroundColor.a);
		}
	}
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

	if (!LoadBackgroundBuffer())
	{
		return false;
	}

	return true;
}

bool GraphicBase::SetVertexBufferBackground()
{
	SAFE_RELEASE(m_vertexBufferBackground);

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = m_dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(SPCVertex) * m_vertexCapacity;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = m_dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = m_verticesBackground;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	HRESULT result = DXDEVICE->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBufferBackground);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool GraphicBase::LoadBackgroundBuffer()
{
	if (m_background != BackgroundType::BACKGROUND_NONE && m_vertexBufferBackground == nullptr)
	{
		m_verticesBackground = new SPCVertex[m_vertexCapacity];
		if (!SetVertexBufferBackground())
		{
			return false;
		}
	}
	return true;
}

bool GraphicBase::UpdateBufferData(ID3D11Buffer* targetBuffer, void* sourceData, unsigned int byteSize)
{
	if (sourceData == nullptr || targetBuffer == nullptr || byteSize == 0)
		return false;

	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	result = DXDEVICECONTEXT->Map(targetBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	memcpy(mappedResource.pData, sourceData, byteSize);
	DXDEVICECONTEXT->Unmap(targetBuffer, 0);
	return true;
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PRIVATE FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
bool GraphicBase::SetVertexBuffer()
{
	SAFE_RELEASE(m_vertexBuffer);

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = m_dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = m_vertexSize * m_vertexCapacity;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = m_dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
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
	
	return true;
}

bool GraphicBase::SetIndexBuffer()
{
	SAFE_RELEASE(m_indexBuffer);

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCapacity;
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
