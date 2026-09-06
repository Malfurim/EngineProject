#pragma once

// --- INCLUDES ---
#include <d3d11.h>
#include <directxmath.h>
#include <string>

// --- CONSTANTS ---


// --- MACROS & DEFINES ---


// --- ENUMS & STRUCTS ---


// --- FORWARD DECLARATIONS ---
using namespace DirectX;


class Shader
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	Shader();
	virtual ~Shader();

	// --- CORE FUNCTIONS ---
	// bool Initialize();
	// void Update();
	virtual void Render(int indexCount);
	virtual void Reset();

	// --- VIRTUAL FUNCTIONS ---
	

	// --- CLASS API ---


	// --- GETTERS & SETTERS ---
	bool SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

	// --- STATIC CLASS API ---


protected:
	// --- CONSTRUCTORS ---


	// --- VIRTUAL FUNCTIONS ---
	virtual bool InitializeShaderComponents(ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer);

	// --- PROTECTED FUNCTIONS ---
	void GetShaderBuffers(ID3D10Blob** vertexShaderBuffer, ID3D10Blob** pixelShaderBuffer, const std::string& hlslFilename, const char* shaderVertexFunctionEntry, const char* shaderPixelFunctionEntry, const char* shaderVertexName, const char* shaderPixelName);

private:
	// --- CONSTRUCTORS ---


	// --- PRIVATE FUNCTIONS ---
	ID3D10Blob* LoadCompiledShader(const std::string& shaderName);
	void SaveCompiledShader(ID3D10Blob* shaderBuffer, std::string shaderFilename);
	ID3D10Blob* CompileShader(const char* shaderCode, SIZE_T shaderLength, const char* entryPoint, const char* targetVersion);
	virtual bool InitializeLayout(ID3D10Blob* vertexShaderBuffer);
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, LPCWSTR shaderFileName);
	std::string LoadShaderSourceFile(const std::string& filename);

// - PROPERTIES -
public:
	// --- PUBLIC COMPONENT STATES ---


	// --- PUBLIC COMPONENT DATA ---


protected:
	// --- INTERNAL CONSTANTS ---


	// --- INTERNAL STRUCTS & ENUMS ---


	// --- INTERNAL COMPONENT STATES ---


	// --- INTERNAL COMPONENT DATA ---
	ID3D11InputLayout* m_layout{ nullptr };

private:
	// --- PRIVATE CONSTANTS ---


	// --- PRIVATE STRUCTS & ENUMS ---
	struct MatrixBufferType
	{
		XMMATRIX worldMatrix;
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
	};

	// --- PRIVATE COMPONENT STATES ---


	// --- PRIVATE COMPONENT DATA ---
	ID3D11VertexShader* m_vertexShader{ nullptr };
	ID3D11PixelShader* m_pixelShader{ nullptr };
	ID3D11Buffer* m_matrixBuffer{ nullptr };

	// --- STATIC GLOBAL STATES ---
	
};

