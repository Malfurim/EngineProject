#pragma once

// --- INCLUDES ---
#include <string>
#include "ResourceBase.h"

// --- CONSTANTS ---


// --- MACROS & DEFINES ---


// --- ENUMS & STRUCTS ---


// --- FORWARD DECLARATIONS ---
struct ID3D11ShaderResourceView;

class Texture : public ResourceBase
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	Texture();
	~Texture() override;
	
	// --- CORE FUNCTIONS ---
	bool Initialize(const std::wstring& filePath, const std::wstring& name, bool enableMipmap);
	// void Update();
	// void Render();

	// --- VIRTUAL FUNCTIONS ---


	// --- CLASS API ---


	// --- GETTERS & SETTERS ---
	//ID3D11ShaderResourceView* GetTexture() const { return m_texture; }
	ID3D11ShaderResourceView* GetShaderResource() const { return m_texture; }
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	// --- STATIC CLASS API ---


protected:
	// --- CONSTRUCTORS ---


	// --- VIRTUAL FUNCTIONS ---


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


private:
	// --- PRIVATE CONSTANTS ---


	// --- PRIVATE STRUCTS & ENUMS ---


	// --- PRIVATE COMPONENT STATES ---


	// --- PRIVATE COMPONENT DATA ---
	ID3D11ShaderResourceView* m_texture{ nullptr };
	int m_width{ 0 };
	int m_height{ 0 };

	// --- STATIC GLOBAL STATES ---

};
