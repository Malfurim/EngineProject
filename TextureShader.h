#pragma once

// --- INCLUDES ---
#include "Shader.h"

// --- CONSTANTS ---


// --- MACROS & DEFINES ---


// --- ENUMS & STRUCTS ---


// --- FORWARD DECLARATIONS ---


class TextureShader : public Shader
{
// - INTERFACE -
public:
    // --- CONSTRUCTORS & DESTRUCTOR ---
    TextureShader();
    virtual ~TextureShader();

    // --- CORE FUNCTIONS ---
    // bool Initialize();
    // void Update();
    virtual void Render(int indexCount);
    virtual void Reset();

    // --- VIRTUAL FUNCTIONS ---


    // --- CLASS API ---
   

    // --- GETTERS & SETTERS ---
    bool SetShaderParameters(ID3D11ShaderResourceView* texture);
    bool SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

    // --- STATIC CLASS API ---
    

protected:
    // --- CONSTRUCTORS ---


    // --- VIRTUAL FUNCTIONS ---
    virtual bool InitializeShaderComponents(ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer);
    virtual bool InitializeLayout(ID3D10Blob* vertexShaderBuffer);

    // --- PROTECTED FUNCTIONS ---


private:
    // --- CONSTRUCTORS ---


    // --- PRIVATE FUNCTIONS ---


// - PROPERTIES -
public:
    // --- PUBLIC COMPONENT STATES ---


    // --- PUBLIC COMPONENT DATA ---

protected:
    // --- INTERNAL CONSTANTS ---


    // --- INTERNAL STRUCTS & ENUMS ---


    // --- INTERNAL COMPONENT STATES ---


    // --- INTERNAL COMPONENT DATA ---
    ID3D11SamplerState* m_samplerState{ nullptr };

private:
    // --- PRIVATE CONSTANTS ---


    // --- PRIVATE STRUCTS & ENUMS ---


    // --- PRIVATE COMPONENT STATES ---


    // --- PRIVATE COMPONENT DATA ---


    // --- STATIC GLOBAL STATES ---

};
