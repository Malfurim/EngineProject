#include "FontShader.h"
#include "Utils.h"

FontShader::FontShader()
{
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	GetShaderBuffers(&vertexShaderBuffer, &pixelShaderBuffer, "shader_font", "VertexShaderEntry", "PixelShaderEntry", "fontVS", "fontPS");
	if (vertexShaderBuffer != nullptr && pixelShaderBuffer != nullptr)
		InitializeShaderComponents(vertexShaderBuffer, pixelShaderBuffer);
}

FontShader::~FontShader()
{
}
