#pragma once

// --- INCLUDES ---
#include <d3d11.h>
#include <unordered_map>
#include "Texture.h"

// --- CONSTANTS ---


// --- MACROS & DEFINES ---


// --- ENUMS & FLAGS ---


// --- FORWARD DECLARATIONS ---
struct CharacterData
{
	float left, right;
	float top, bottom;
	int size;
};

class Font
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	Font();
	~Font();

	// --- VIRTUAL FUNCTIONS ---
	virtual bool Initialize(const std::wstring& filePath, Texture* textureData);
	// virtual void Update();
	// virtual void Render();

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
	// --- INTERNAL COMPONENT STATES ---


	// --- INTERNAL COMPONENT DATA ---


private:
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
