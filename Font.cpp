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
//	CORE FUNCTIONS											//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
bool Font::Initialize(const std::wstring& filePath, Texture* textureData)
{
	m_path = filePath;
	m_name = GetFilename(filePath);
	m_texture = textureData;

	if (!LoadCharacterData(filePath))
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
//	VIRTUAL FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CLASS API												//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
// for future custom made font images with constant 1 pixel space between characters
// first line numbers -> ascenderHeight(Š), capHeight(S), descenderHeight(g)
// font data in format: # 0 8 -> where:
// # - character
// 0 - line number on the image (0 = first)
// 8 - width of the character in pixels

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

	float ascender = 0.0f;
	float cap = 0.0f;
	float descender = 0.0f;
	float total = 0.0f;

	file >> ascender;
	file >> cap;
	file >> descender;
	file.ignore(1000, L'\n');

	total = ascender + cap + descender;
	m_fontHeight = { ascender, cap, descender, total };

	float oldLineNumber = 0.0f;
	float lineHeight = m_fontHeight.totalHeight / (float)m_texture->GetHeight();
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

