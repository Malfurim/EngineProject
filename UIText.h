#pragma once

// --- INCLUDES ---
#include "UIElement.h"
#include <string>

// --- CONSTANTS ---


// --- MACROS & DEFINES ---


// --- ENUMS & STRUCTS ---
enum TextProperty : unsigned int
{
	TEXT_PROPERTY_NONE = 0,
	TEXT_PROPERTY_EDITABLE = 1 << 0,	// 1
	TEXT_PROPERTY_MULTILINE = 1 << 1,	// 2
	TEXT_PROPERTY_NUMBERS_ONLY = 1 << 2,	// 4
};

enum HorizontalTextAlignment
{
	HORIZONTAL_TEXT_ALIGNMENT_LEFT,
	HORIZONTAL_TEXT_ALIGNMENT_CENTER,
	HORIZONTAL_TEXT_ALIGNMENT_RIGHT
};

enum VerticalTextAlignment
{
	VERTICAL_TEXT_ALIGNMENT_TOP,
	VERTICAL_TEXT_ALIGNMENT_MIDDLE,
	VERTICAL_TEXT_ALIGNMENT_BOTTOM
};

// --- FORWARD DECLARATIONS ---
class FontShader;
class Font;

class UIText : public UIElement
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	UIText();
	UIText(Position position, Size size, Font* font);
	~UIText();

	// --- CORE FUNCTIONS ---
	// bool Initialize();
	void Update();
	void Render();
	static void Shutdown();

	// --- VIRTUAL FUNCTIONS ---
	void OnTextInput(wchar_t character) override;
	bool CapturesKeyboard() const override { return (m_properties & TextProperty::TEXT_PROPERTY_EDITABLE); }

	// --- CLASS API ---
	void AddTextProperty(TextProperty property);
	void RemoveTextProperty(TextProperty property);

	// --- GETTERS & SETTERS ---
	std::wstring GetText() const { return m_text; }
	float GetFontHeight() const { return m_fontHeight; }
	float GetTextWidth() const { return m_textWidth; }

	void SetText(std::wstring text);
	void SetFont(Font* font);
	void SetFontSize(float size);
	void SetFontColor(Color color);
	void SetTextAlignment(HorizontalTextAlignment horizontalTextAlignment);
	void SetTextAlignment(VerticalTextAlignment verticalTextAlignment);
	void SetTextAlignment(HorizontalTextAlignment horizontalTextAlignment, VerticalTextAlignment verticalTextAlignment);
	void SetTextProperties(unsigned int properties);

	unsigned int GetTextProperties() { return m_properties; }
	bool HasTextProperty(TextProperty property) const { return (m_properties & property) != 0; }

	// --- STATIC CLASS API ---


protected:
	// --- CONSTRUCTORS ---


	// --- VIRTUAL FUNCTIONS ---


	// --- PROTECTED FUNCTIONS ---


private:
	// --- CONSTRUCTORS ---


	// --- PRIVATE FUNCTIONS ---
	void ConfirmShader();
	float CalculateLineWidth(const std::wstring& text);
	void CalculateTextWidth();
	void CalculateGeometry();
	void EnsureInputState();

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
	unsigned int m_properties{ TextProperty::TEXT_PROPERTY_NONE };

	// --- PRIVATE COMPONENT DATA ---
	Color m_fontColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	Font* m_font{ nullptr };
	float m_fontHeight{ 0.0f };
	float m_textWidth{ 0.0f };
	int m_maxLength{ 0 };

	HorizontalTextAlignment m_horizontalAlign{ HorizontalTextAlignment::HORIZONTAL_TEXT_ALIGNMENT_LEFT };
	VerticalTextAlignment m_verticalAlign{ VerticalTextAlignment::VERTICAL_TEXT_ALIGNMENT_TOP };

	std::wstring m_text;

	// --- STATIC GLOBAL STATES ---
	static FontShader* ms_textShader;
};

