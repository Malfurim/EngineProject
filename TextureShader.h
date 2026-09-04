#pragma once

#include "Shader.h"

class TextureShader : public Shader
{
public:
    TextureShader();
    virtual ~TextureShader();

    virtual void Render(int indexCount);
    virtual void Reset();

    bool SetShaderParameters(ID3D11ShaderResourceView* texture);
    bool SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

protected:
    virtual bool InitializeShaderComponents(ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer);
    virtual bool InitializeLayout(ID3D10Blob* vertexShaderBuffer);

protected:
    ID3D11SamplerState* m_samplerState{ nullptr };
};
