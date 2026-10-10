#pragma once

// --- INCLUDES ---
#include <string>

// --- CONSTANTS ---


// --- MACROS & DEFINES ---


// --- ENUMS & STRUCTS ---


// --- FORWARD DECLARATIONS ---
struct ID3D11ShaderResourceView;

class Texture
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	Texture();
	~Texture();
	
	// --- CORE FUNCTIONS ---
	bool Initialize(const std::wstring& filePath, const std::wstring& name, bool enableMipmap);
	// void Update();
	// void Render();

	// --- VIRTUAL FUNCTIONS ---


	// --- CLASS API ---


	// --- GETTERS & SETTERS ---
	ID3D11ShaderResourceView* GetTexture() const { return m_texture; }
	std::wstring GetName() const { return m_name; }
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
	std::wstring m_name;
	std::wstring m_path;
	int m_width{ 0 };
	int m_height{ 0 };

	// --- STATIC GLOBAL STATES ---

};
