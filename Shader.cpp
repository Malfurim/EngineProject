#include "Shader.h"

// --- ADDITIONAL INCLUDES ---
#include <d3dcompiler.h>
#include <fstream>
#include "Window.h"
#include "DirectXManager.h"
#include "Deleters.h"
#include "Utils.h"

// --- MACROS & DEFINES ---


// --- FORWARD DECLARATIONS ---


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC GLOBAL STATES & DATA								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CONSTRUCTORS & DESTRUCTOR								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
Shader::Shader()		// Default constructor for color shader
{
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	GetShaderBuffers(&vertexShaderBuffer, &pixelShaderBuffer, "shader_color", "ColorVertexShader", "ColorPixelShader", "colorVS", "colorPS");
	if (vertexShaderBuffer != nullptr && pixelShaderBuffer != nullptr)
		InitializeShaderComponents(vertexShaderBuffer, pixelShaderBuffer);
}

Shader::~Shader()
{
	SAFE_RELEASE(m_matrixBuffer);
	SAFE_RELEASE(m_layout);
	SAFE_RELEASE(m_pixelShader);
	SAFE_RELEASE(m_vertexShader);
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CORE FUNCTIONS											//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//bool Shader::Initialize()
//{
//	return true;
//}

//void Shader::Update()
//{
//}

void Shader::Render(int indexCount)
{
	// Set the vertex input layout
	DXDEVICECONTEXT->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render
	DXDEVICECONTEXT->VSSetShader(m_vertexShader, nullptr, 0);
	DXDEVICECONTEXT->PSSetShader(m_pixelShader, nullptr, 0);
	
	// Render
	DXDEVICECONTEXT->DrawIndexed(indexCount, 0, 0);
}

void Shader::Reset()
{
	DXDEVICECONTEXT->IASetInputLayout(nullptr);
	DXDEVICECONTEXT->VSSetShader(nullptr, nullptr, 0);
	DXDEVICECONTEXT->PSSetShader(nullptr, nullptr, 0);
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
bool Shader::SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
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

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC CLASS API										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	VIRTUAL FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
bool Shader::InitializeShaderComponents(ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer)
{
	HRESULT result;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

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

	return true;
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void Shader::GetShaderBuffers(ID3D10Blob** vertexShaderBuffer, ID3D10Blob** pixelShaderBuffer, const std::string& hlslFilename, const char* shaderVertexFunctionEntry, const char* shaderPixelFunctionEntry, const char* shaderVertexName, const char* shaderPixelName)
{
	*vertexShaderBuffer = LoadCompiledShader(shaderVertexName);
	*pixelShaderBuffer = LoadCompiledShader(shaderPixelName);

	if (*vertexShaderBuffer == nullptr || *pixelShaderBuffer == nullptr)
	{
		std::string shaderCode = LoadShaderSourceFile(hlslFilename);
		if (shaderCode.empty())
			return;

		if (*vertexShaderBuffer == nullptr)
		{
			*vertexShaderBuffer = CompileShader(shaderCode.c_str(), shaderCode.length(), shaderVertexFunctionEntry, "vs_5_0");
			if (vertexShaderBuffer != nullptr)
				SaveCompiledShader(*vertexShaderBuffer, shaderVertexName);
		}
		if (*pixelShaderBuffer == nullptr)
		{
			*pixelShaderBuffer = CompileShader(shaderCode.c_str(), shaderCode.length(), shaderPixelFunctionEntry, "ps_5_0");
			if (pixelShaderBuffer != nullptr)
				SaveCompiledShader(*pixelShaderBuffer, shaderPixelName);
		}
	}
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PRIVATE FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
ID3D10Blob* Shader::LoadCompiledShader(const std::string& shaderName)
{
	std::string shaderPath = "assets/shaders/" + shaderName + ".cso";
	std::ifstream file(shaderPath, std::ios::in | std::ios::binary);
	if (file.fail())
		return nullptr;

	uintmax_t size = GetFileSize(shaderPath);
	char* shaderCode = new char[size];
	file.seekg(0, std::ios::beg);
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

void Shader::SaveCompiledShader(ID3D10Blob* shaderBuffer, std::string shaderFilename)
{
	char* bufferData = (char*)shaderBuffer->GetBufferPointer();
	std::string dir = "assets/shaders/";
	CreateDirectoryA(dir.c_str(), NULL);

	std::ofstream file(dir + shaderFilename + ".cso", std::ios::out | std::ios::binary);
	file.write(bufferData, shaderBuffer->GetBufferSize());
	file.close();
	bufferData = nullptr;
}

ID3D10Blob* Shader::CompileShader(const char* shaderCode, SIZE_T shaderLength, const char* entryPoint, const char* targetVersion)
{
	HRESULT result;
	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* shaderBuffer = nullptr;

	result = D3DCompile(shaderCode, shaderLength, nullptr, nullptr, nullptr, entryPoint, targetVersion, D3D10_SHADER_ENABLE_STRICTNESS, 0, &shaderBuffer, &errorMessage);
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

bool Shader::InitializeLayout(ID3D10Blob* vertexShaderBuffer)
{
	// Setup the layout of the data that goes into the shader
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

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

std::string Shader::LoadShaderSourceFile(const std::string& filename)
{
	std::string path = "assets/shaders/" + filename + ".hlsl";
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

