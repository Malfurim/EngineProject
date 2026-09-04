#include "TextureShader.h"
#include "DirectXManager.h"
#include "Deleters.h"

TextureShader::TextureShader()
{
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	GetShaderBuffers(&vertexShaderBuffer, &pixelShaderBuffer, "shader_texture", "VertexShaderEntry", "PixelShaderEntry", "textureVS", "texturePS");
	if (vertexShaderBuffer != nullptr && pixelShaderBuffer != nullptr)
		InitializeShaderComponents(vertexShaderBuffer, pixelShaderBuffer);
}

TextureShader::~TextureShader()
{
	SAFE_RELEASE(m_samplerState);
}

void TextureShader::Render(int indexCount)
{
	DXDEVICECONTEXT->PSSetSamplers(0, 1, &m_samplerState);
	Shader::Render(indexCount);
}

void TextureShader::Reset()
{
	DXDEVICECONTEXT->PSSetSamplers(0, 0, nullptr);
	Shader::Reset();
}

bool TextureShader::SetShaderParameters(ID3D11ShaderResourceView* texture)
{
	if (texture == nullptr)
		return false;

	ID3D11ShaderResourceView* arraySRV[1] = { texture };

	DXDEVICECONTEXT->PSSetShaderResources(0, 1, arraySRV);
	return true;
}

bool TextureShader::SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	return Shader::SetShaderParameters(worldMatrix, viewMatrix, projectionMatrix);
}

bool TextureShader::InitializeShaderComponents(ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer)
{
	Shader::InitializeShaderComponents(vertexShaderBuffer, pixelShaderBuffer);

	// Create a texture sampler state description.
	HRESULT result;
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = DXDEVICE->CreateSamplerState(&samplerDesc, &m_samplerState);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool TextureShader::InitializeLayout(ID3D10Blob* vertexShaderBuffer)
{
	// Setup the layout of the data that goes into the shader
	D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "DIFFUSE";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "TEXCOORD";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	polygonLayout[4].SemanticName = "TEXCOORD";
	polygonLayout[4].SemanticIndex = 1;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[4].InputSlot = 0;
	polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[4].InstanceDataStepRate = 0;

	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout
	HRESULT result;
	result = DXDEVICE->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}
