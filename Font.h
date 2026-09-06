#pragma once

// --- INCLUDES ---
#include <d3d11.h>
#include <unordered_map>
#include "Texture.h"

// --- CONSTANTS ---


// --- MACROS & DEFINES ---


// --- ENUMS & STRUCTS ---
struct CharacterData
{
	float left, right;
	float top, bottom;
	int size;
};

// --- FORWARD DECLARATIONS ---


class Font
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	Font();
	~Font();
	// --- CORE FUNCTIONS ---
	
	bool Initialize(const std::wstring& filePath, Texture* textureData);
	// void Update();
	// void Render();
	
	// --- VIRTUAL FUNCTIONS ---


	// --- CLASS API ---
	bool LoadCharacterData(const std::wstring& filePath);
	bool LoadCharacterData2(const std::wstring& filePath);

	// --- GETTERS & SETTERS ---
	std::wstring GetName() const { return m_name; }
	ID3D11ShaderResourceView* GetTexture() const { return m_texture->GetTexture(); }
	CharacterData GetCharacter(wchar_t character);
	float GetFontHeight() const { return m_fontHeight; }

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
	std::wstring m_name{};
	std::wstring m_path{};
	std::unordered_map<wchar_t, CharacterData> m_characters;
	Texture* m_texture{ nullptr };
	float m_fontHeight{ 0.0f };
	int m_spaceSize{ 0 };

	// --- STATIC GLOBAL STATES ---

};
