#pragma once

#include "UIElement.h"
#include "FontShader.h"

enum TextProperty : unsigned int
{
	TEXT_PROPERTY_NONE			= 0,
	TEXT_PROPERTY_EDITABLE		= 1 << 0,	// 1
	TEXT_PROPERTY_MULTILINE		= 1 << 1,	// 2
	TEXT_PROPERTY_NUMBERS_ONLY	= 1 << 2,	// 4
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

class UIText : public UIElement
{
public:
	UIText();
	UIText(Position position, Size size, Font* font);
	~UIText();
	static void Shutdown();

	void Render();
	void Update();

	void SetText(std::wstring text);
	std::wstring GetText();

	void SetFont(Font* font);
	void SetFontSize(float size);
	void SetFontColor(Color color);
	void SetTextAlignment(HorizontalTextAlignment horizontalTextAlignment);
	void SetTextAlignment(VerticalTextAlignment verticalTextAlignment);
	void SetTextAlignment(HorizontalTextAlignment horizontalTextAlignment, VerticalTextAlignment verticalTextAlignment);
	float GetFontHeight();
	float GetTextWidth();

	void SetTextProperties(unsigned int properties);
	void AddTextProperty(TextProperty property);
	void RemoveTextProperty(TextProperty property);

	unsigned int GetTextProperties() { return m_properties; }
	bool HasTextProperty(TextProperty property) const { return (m_properties & property) != 0; }

	void OnTextInput(wchar_t character) override;
	bool CapturesKeyboard() const override { return (m_properties & TextProperty::TEXT_PROPERTY_EDITABLE); }

private:
	void ConfirmShader();
	float CalculateLineWidth(const std::wstring& text);
	void CalculateTextWidth();
	void CalculateGeometry();
	void EnsureInputState();

private:
	static FontShader* ms_textShader;

	Color m_fontColor{ 1.0f, 1.0f, 1.0f, 1.0f };

	Font* m_font{ nullptr };

	float m_fontHeight{ 0.0f };
	float m_textWidth{ 0.0f };
	int m_maxLength{ 0 };

	unsigned int m_properties{ TextProperty::TEXT_PROPERTY_NONE };

	HorizontalTextAlignment m_horizontalAlign{ HorizontalTextAlignment::HORIZONTAL_TEXT_ALIGNMENT_LEFT };
	VerticalTextAlignment m_verticalAlign{ VerticalTextAlignment::VERTICAL_TEXT_ALIGNMENT_TOP };

	std::wstring m_text;
};

