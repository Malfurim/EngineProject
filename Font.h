#pragma once

#include <d3d11.h>
#include <unordered_map>
#include "Texture.h"

struct CharacterData
{
	float left, right;
	float top, bottom;
	int size;
};

class Font
{
public:
	Font();
	~Font();

	bool Initialize(const std::wstring& filePath, Texture* textureData);
	bool LoadCharacterData(const std::wstring& filePath);
	bool LoadCharacterData2(const std::wstring& filePath);
	
	std::wstring GetName();
	ID3D11ShaderResourceView* GetTexture();
	CharacterData GetCharacter(wchar_t character);
	float GetFontHeight();

private:
	std::wstring m_name{};
	std::wstring m_path{};
	std::unordered_map<wchar_t, CharacterData> m_characters;
	Texture* m_texture{ nullptr };
	float m_fontHeight{ 0.0f };
	int m_spaceSize{ 0 };
};
