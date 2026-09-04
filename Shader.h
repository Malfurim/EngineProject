#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <string>

using namespace DirectX;

class Shader
{
public:
	Shader();
	virtual ~Shader();

	virtual void Render(int indexCount);
	virtual void Reset();

	bool SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

protected:
	void GetShaderBuffers(ID3D10Blob** vertexShaderBuffer, ID3D10Blob** pixelShaderBuffer, const std::string& hlslFilename, const char* shaderVertexFunctionEntry, const char* shaderPixelFunctionEntry, const char* shaderVertexName, const char* shaderPixelName);
	virtual bool InitializeShaderComponents(ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer);

private:
	ID3D10Blob* LoadCompiledShader(const std::string& shaderName);
	void SaveCompiledShader(ID3D10Blob* shaderBuffer, std::string shaderFilename);
	ID3D10Blob* CompileShader(const char* shaderCode, SIZE_T shaderLength, const char* entryPoint, const char* targetVersion);
	virtual bool InitializeLayout(ID3D10Blob* vertexShaderBuffer);
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, LPCWSTR shaderFileName);
	std::string LoadShaderSourceFile(const std::string& filename);

protected:
	ID3D11InputLayout* m_layout{ nullptr };

private:
	ID3D11VertexShader* m_vertexShader{ nullptr };
	ID3D11PixelShader* m_pixelShader{ nullptr };
	ID3D11Buffer* m_matrixBuffer{ nullptr };

private:
	struct MatrixBufferType
	{
		XMMATRIX worldMatrix;
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
	};
};

