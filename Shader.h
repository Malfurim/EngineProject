#pragma once

// --- INCLUDES ---
#include <d3d11.h>
#include "DXMath.h"
#include <string>
#include "ResourceBase.h"

// --- CONSTANTS ---


// --- MACROS & DEFINES ---


// --- ENUMS & STRUCTS ---


// --- FORWARD DECLARATIONS ---
enum VertexFlag : unsigned int;

class Shader : public ResourceBase
{
	// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	Shader();
	~Shader() override;

	// --- CORE FUNCTIONS ---
	bool Initialize(const std::wstring& name, unsigned int id, unsigned int layoutFlags);
	// void Update();
	void Render(unsigned int indexCount);

	// --- PUBLIC VIRTUAL FUNCTIONS ---


	// --- CLASS API ---


	// --- GETTERS & SETTERS ---
	ID3D11VertexShader* GetVertexShader() const { return m_vertexShader; }
	ID3D11PixelShader* GetPixelShader() const { return m_pixelShader; }
	ID3D11InputLayout* GetLayout() const { return m_layout; }

	bool SetShaderParameters(DXMATRIX worldMatrix, DXMATRIX viewMatrix, DXMATRIX projectionMatrix);
	bool SetShaderParameters(ID3D11ShaderResourceView* texture);

	// --- STATIC CLASS API ---


protected:
	// --- CONSTRUCTORS ---


	// --- PROTECTED VIRTUAL FUNCTIONS ---


	// --- PROTECTED FUNCTIONS ---


private:
	// --- CONSTRUCTORS ---


	// --- PRIVATE FUNCTIONS ---
	bool InitializeShaderComponents(ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer);
	bool GetShaderBuffers(ID3D10Blob** vertexShaderBuffer, ID3D10Blob** pixelShaderBuffer, const std::wstring& hlslFilename, const char* shaderVertexFunctionEntry, const char* shaderPixelFunctionEntry, const std::wstring& shaderVertexName, const std::wstring& shaderPixelName);
	ID3D10Blob* LoadCompiledShader(const std::wstring& name);
	ID3D10Blob* CompileShader(const std::string& shaderCode, const char* entryPoint, const char* targetVersion);
	void SaveCompiledShader(ID3D10Blob* shaderBuffer, const std::wstring& shaderFilename);
	virtual bool InitializeLayout(ID3D10Blob* vertexShaderBuffer);
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, LPCWSTR shaderFileName);
	std::string LoadShaderSourceFile(const std::wstring& filename);

// - PROPERTIES -
public:
	// --- PUBLIC COMPONENT STATES ---


	// --- PUBLIC COMPONENT DATA ---


protected:
	// --- INTERNAL CONSTANTS ---


	// --- INTERNAL STRUCTS & ENUMS ---


	// --- INTERNAL COMPONENT STATES ---


	// --- INTERNAL COMPONENT DATA ---


private:
	// --- PRIVATE CONSTANTS ---


	// --- PRIVATE STRUCTS & ENUMS ---
	struct MatrixBufferType
	{
		DXMATRIX worldMatrix;
		DXMATRIX viewMatrix;
		DXMATRIX projectionMatrix;
	};

	// --- PRIVATE COMPONENT STATES ---
	unsigned int m_layoutFlags{ 0 };

	// --- PRIVATE COMPONENT DATA ---
	ID3D11InputLayout* m_layout{ nullptr };
	ID3D11VertexShader* m_vertexShader{ nullptr };
	ID3D11PixelShader* m_pixelShader{ nullptr };
	ID3D11SamplerState* m_samplerState{ nullptr };
	ID3D11Buffer* m_matrixBuffer{ nullptr };

	// --- STATIC GLOBAL STATES ---
	static unsigned int ms_setShaderID;
};