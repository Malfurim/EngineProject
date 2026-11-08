#include "Shader.h"

// --- ADDITIONAL INCLUDES ---
#include <d3dcompiler.h>
#include <fstream>
#include <string>
#include <vector>
#include "Window.h"
#include "DirectXManager.h"
#include "VertexStructs.h"
#include "ShaderVersions.h"
#include "Deleters.h"
#include "Utils.h"

// --- MACROS & DEFINES ---


// --- FORWARD DECLARATIONS ---


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC GLOBAL STATES & DATA								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
unsigned int Shader::ms_setShaderID = 0;

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CONSTRUCTORS & DESTRUCTOR								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
Shader::Shader()
{
}

Shader::~Shader()
{
	SAFE_RELEASE(m_matrixBuffer);
	SAFE_RELEASE(m_layout);
	SAFE_RELEASE(m_pixelShader);
	SAFE_RELEASE(m_vertexShader);
	SAFE_RELEASE(m_samplerState);
	m_name.clear();
	m_path.clear();
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CORE FUNCTIONS											//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
bool Shader::Initialize(const std::wstring& name, unsigned int id, unsigned int layoutFlags)
{
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	if (!GetShaderBuffers(&vertexShaderBuffer, &pixelShaderBuffer, name, "VertexShaderEntry", "PixelShaderEntry", name + L"VS", name + L"PS"))
		return false;
	if (vertexShaderBuffer != nullptr && pixelShaderBuffer != nullptr)
	{
		m_layoutFlags = layoutFlags;
		if (!InitializeShaderComponents(vertexShaderBuffer, pixelShaderBuffer))
			return false;
	}

	m_name = name;
	m_id = id;
	return true;
}

//void Shader::Update()
//{
//}

void Shader::Render(unsigned int indexCount)
{
	if (ms_setShaderID != m_id)
	{
		// Set the vertex input layout
		DXDEVICECONTEXT->IASetInputLayout(m_layout);

		// Set the vertex and pixel shaders that will be used to render
		DXDEVICECONTEXT->VSSetShader(m_vertexShader, nullptr, 0);
		DXDEVICECONTEXT->PSSetShader(m_pixelShader, nullptr, 0);

		if (m_layoutFlags & VertexFlag::VERTEX_TEXCOORD)
		{
			DXDEVICECONTEXT->PSSetSamplers(0, 1, &m_samplerState);
		}

		ms_setShaderID = m_id;
	}

	// Render
	DXDEVICECONTEXT->DrawIndexed(indexCount, 0, 0);
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
bool Shader::SetShaderParameters(DXMATRIX worldMatrix, DXMATRIX viewMatrix, DXMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;

	// Transport all matrices
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// Lock buffer
	result = DXDEVICECONTEXT->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->worldMatrix = worldMatrix;
	dataPtr->viewMatrix = viewMatrix;
	dataPtr->projectionMatrix = projectionMatrix;

	// Unlock buffer
	DXDEVICECONTEXT->Unmap(m_matrixBuffer, 0);

	// Update the values in the shader
	DXDEVICECONTEXT->VSSetConstantBuffers(0, 1, &m_matrixBuffer);

	return true;
}

bool Shader::SetShaderParameters(ID3D11ShaderResourceView* texture)
{
	if (texture == nullptr)
		return false;

	DXDEVICECONTEXT->PSSetShaderResources(0, 1, &texture);
	return true;
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC CLASS API										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED VIRTUAL FUNCTIONS								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PRIVATE FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
bool Shader::InitializeShaderComponents(ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer)
{
	HRESULT result;
	// Create vertex shader buffer
	result = DXDEVICE->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &m_vertexShader);
	if (FAILED(result))
	{
		MessageBox(NULL, L"failed on vertex", L"fail", MB_OK);
		return false;
	}

	// Create pixel shader buffer
	result = DXDEVICE->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &m_pixelShader);
	if (FAILED(result))
	{
		MessageBox(NULL, L"failed on pixel", L"fail", MB_OK);
		return false;
	}

	if (!InitializeLayout(vertexShaderBuffer))
	{
		MessageBox(NULL, L"failed on layout", L"fail", MB_OK);
		return false;
	}

	// Release shader buffers
	SAFE_RELEASE(vertexShaderBuffer);
	SAFE_RELEASE(pixelShaderBuffer);

	// Setup the matrix buffer description
	D3D11_BUFFER_DESC matrixBufferDesc{};
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer
	result = DXDEVICE->CreateBuffer(&matrixBufferDesc, nullptr, &m_matrixBuffer);
	if (FAILED(result))
	{
		MessageBox(NULL, L"failed on matrix", L"fail", MB_OK);
		return false;
	}

	if (m_layoutFlags & VertexFlag::VERTEX_TEXCOORD)
	{
		// Create a texture sampler state description.
		D3D11_SAMPLER_DESC samplerDesc{};
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
	}

	return true;
}

bool Shader::GetShaderBuffers(ID3D10Blob** vertexShaderBuffer, ID3D10Blob** pixelShaderBuffer, const std::wstring& hlslFilename, const char* shaderVertexFunctionEntry, const char* shaderPixelFunctionEntry, const std::wstring& shaderVertexName, const std::wstring& shaderPixelName)
{
	*vertexShaderBuffer = LoadCompiledShader(shaderVertexName);
	*pixelShaderBuffer = LoadCompiledShader(shaderPixelName);

	if (*vertexShaderBuffer == nullptr || *pixelShaderBuffer == nullptr)
	{
		std::string shaderCode = LoadShaderSourceFile(hlslFilename);
		if (shaderCode.empty())
			return false;

		if (*vertexShaderBuffer == nullptr)
		{
			*vertexShaderBuffer = CompileShader(shaderCode, shaderVertexFunctionEntry, "vs_5_0");
			if (*vertexShaderBuffer != nullptr)
				SaveCompiledShader(*vertexShaderBuffer, shaderVertexName);
		}
		if (*pixelShaderBuffer == nullptr)
		{
			*pixelShaderBuffer = CompileShader(shaderCode.c_str(), shaderPixelFunctionEntry, "ps_5_0");
			if (*pixelShaderBuffer != nullptr)
				SaveCompiledShader(*pixelShaderBuffer, shaderPixelName);
		}
	}

	return true;
}

ID3D10Blob* Shader::LoadCompiledShader(const std::wstring& name)
{
	std::wstring shaderPath = L"assets/shaders/" + name + L".cso";
	std::ifstream file(shaderPath, std::ios::in | std::ios::binary);
	if (file.fail())
		return nullptr;

	unsigned int version = 0;
	file.read(reinterpret_cast<char*>(&version), sizeof(version));
	if (version != ShaderVersion::VERSION)
	{
		file.close();
		DeleteFile(shaderPath.c_str());
		return nullptr;
	}

	uintmax_t size = GetFileSize(shaderPath) - sizeof(unsigned int);
	char* shaderCode = new char[size];
	file.read(shaderCode, size);
	file.close();

	ID3D10Blob* buffer = nullptr;
	HRESULT result = D3D10CreateBlob(size, &buffer);
	if (FAILED(result))
	{
		delete[] shaderCode;
		shaderCode = nullptr;
		buffer->Release();
		buffer = nullptr;
		return nullptr;
	}

	memcpy(buffer->GetBufferPointer(), shaderCode, size);

	delete[] shaderCode;
	shaderCode = nullptr;

	return buffer;
}

ID3D10Blob* Shader::CompileShader(const std::string& shaderCode, const char* entryPoint, const char* targetVersion)
{
	HRESULT result;
	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* shaderBuffer = nullptr;

	result = D3DCompile(shaderCode.c_str(), shaderCode.length(), nullptr, nullptr, nullptr, entryPoint, targetVersion, D3D10_SHADER_ENABLE_STRICTNESS, 0, &shaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, WINDOWHWND, L"Shader ERROR");
		}
		else
		{
			MessageBox(WINDOWHWND, L"Error compiling Shader from program code", L"Error in shader code", MB_OK);
		}
		return nullptr;
	}
	return shaderBuffer;
}

void Shader::SaveCompiledShader(ID3D10Blob* shaderBuffer, const std::wstring& shaderFilename)
{
	char* bufferData = (char*)shaderBuffer->GetBufferPointer();
	std::wstring dir = L"assets/shaders/";
	CreateDirectory(dir.c_str(), NULL);

	std::ofstream file(dir + shaderFilename + L".cso", std::ios::out | std::ios::binary);
	unsigned int version = ShaderVersion::VERSION;
	file.write(reinterpret_cast<const char*>(&version), sizeof(version));
	file.write(bufferData, shaderBuffer->GetBufferSize());
	file.close();
	bufferData = nullptr;
}

bool Shader::InitializeLayout(ID3D10Blob* vertexShaderBuffer)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> polygonLayout;
	unsigned int currentOffset = 0;

	if (m_layoutFlags & VertexFlag::VERTEX_POSITION)
	{
		polygonLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		currentOffset += sizeof(TPosition); 
	}

	if (m_layoutFlags & VertexFlag::VERTEX_COLOR)
	{
		polygonLayout.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		currentOffset += sizeof(TColor);
	}

	if (m_layoutFlags & VertexFlag::VERTEX_DIFFUSE)
	{
		polygonLayout.push_back({ "DIFFUSE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		currentOffset += sizeof(TDiffuse);
	}

	if (m_layoutFlags & VertexFlag::VERTEX_NORMAL)
	{
		polygonLayout.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		currentOffset += sizeof(TNormal);
	}

	if (m_layoutFlags & VertexFlag::VERTEX_TEXCOORD)
	{
		polygonLayout.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		currentOffset += sizeof(TTextureCoordinate); 
	}
	
	if (polygonLayout.empty())
		return false;

	HRESULT result;
	result = DXDEVICE->CreateInputLayout(polygonLayout.data(), static_cast<unsigned int>(polygonLayout.size()), vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void Shader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, LPCWSTR shaderFileName)
{
	char* compileErrors = (char*)errorMessage->GetBufferPointer();
	unsigned long bufferSize = errorMessage->GetBufferSize();

	std::ofstream fout;

	// Open file to write
	fout.open("shader-error.txt");

	for (unsigned int i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close file
	fout.close();

	// Release
	errorMessage->Release();
	errorMessage = nullptr;

	MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for message.", shaderFileName, MB_OK);
}

std::string Shader::LoadShaderSourceFile(const std::wstring& filename)
{
	std::wstring path = L"assets/shaders/" + filename + L".hlsl";
	std::ifstream file(path);

	if (file.fail())
	{
		MessageBox(WINDOWHWND, L"Could not open HLSL source file!", L"Shader Error", MB_OK);
		return "";
	}

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	
	return content;
}

