#include "Font.h"

// --- ADDITIONAL INCLUDES ---
#include <fstream>
#include <sstream>
#include "Utils.h"

// --- MACROS & DEFINES ---


// --- FORWARD DECLARATIONS ---


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC GLOBAL STATES & DATA								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CONSTRUCTORS & DESTRUCTOR								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
Font::Font()
{
}

Font::~Font()
{
	m_characters.clear();
	m_name.clear();
	m_path.clear();
	m_texture = nullptr;
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	VIRTUAL FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
bool Font::Initialize(const std::wstring& filePath, Texture* textureData)
{
	m_path = filePath;
	m_name = GetFilename(filePath);
	m_texture = textureData;

	if (!LoadCharacterData2(filePath))
	{
		return false;
	}
	return true;
}

//void Font::Update()
//{
//}

//void Font::Render()
//{
//}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CLASS API												//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
bool Font::LoadCharacterData(const std::wstring& filePath)
{
	std::wifstream file;
	CharacterData data;
	std::wstring line;
	

	file.open(filePath);
	if (file.fail())
	{
		return false;
	}

	while (std::getline(file, line))
	{
		if (line.empty())
			continue;

		wchar_t dataIndex = line[0];

		std::wstringstream lineStream(line.substr(1));

		CharacterData data;
		if (lineStream >> data.left >> data.right >> data.size)
		{
			m_characters[dataIndex] = data;
		}
	}

	file.close();

	return true;
}

// for future custom made font images with constant 1 pixel space between characters
// font data in format: # 0 8 -> where:
// # - character
// 0 - line number on the image (0 = first)
// 8 - width of the character in pixels
bool Font::LoadCharacterData2(const std::wstring& filePath)
{
	std::wifstream file;
	CharacterData data;
	std::wstring line;

	file.open(filePath);
	if (file.fail())
	{
		return false;
	}

	file >> m_fontHeight;
	file.ignore(1000, L'\n');

	float oldLineNumber = 0.0f;
	float lineHeight = m_fontHeight / (float)m_texture->GetHeight();
	float pointWidth = 1.0f / (float)m_texture->GetWidth();
	float x = 0.0f;

	while (std::getline(file, line))
	{
		if (line.empty())
			continue;

		if (line.back() == L'\r')
		{
			line.pop_back();
		}

		wchar_t dataIndex = line[0];

		std::wstringstream lineStream(line.substr(1));

		CharacterData data;
		float lineNumber;
		if (lineStream >> lineNumber >> data.size)
		{
			if (oldLineNumber != lineNumber)
			{
				x = 0.0f;
				oldLineNumber = lineNumber;
			}

			data.left = x + 0.5f * pointWidth;
			data.right = x + (data.size - 0.5f) * pointWidth;
			data.top = lineNumber * lineHeight;
			data.bottom = (lineNumber + 1.0f) * lineHeight;

			m_characters[dataIndex] = data;
			x += ((data.size + 1.0f) * pointWidth);
		}
	}

	file.close();

	return true;
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	GETTERS & SETTERS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
CharacterData Font::GetCharacter(wchar_t unicodeChar)
{
	auto character = m_characters.find(unicodeChar);
	if (character != m_characters.end())
	{
		return character->second;
	}

	// if not found, return ?
	character = m_characters.find(L'?');
	if (character != m_characters.end())
	{
		return character->second;
	}

	// if ? doesn't exist in the list return empty
	return CharacterData();
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC CLASS API										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	VIRTUAL FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PRIVATE FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //

