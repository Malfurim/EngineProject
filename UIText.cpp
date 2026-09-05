#include "UIText.h"
#include <sstream>
#include "DirectXManager.h"
#include "FontShader.h"
#include "Font.h"
#include "Deleters.h"

FontShader* UIText::ms_textShader = nullptr;

UIText::UIText() : UIText({ 0, 0 }, { 0, 0 }, nullptr)
{
}

UIText::UIText(Position position, Size size, Font* font) : UIElement(position, size)
{
    ConfirmShader();
    SetFont(font);
    m_fontHeight = font->GetFontHeight();
    SetText(L"");
}

UIText::~UIText()
{
    m_text.clear();
}

void UIText::Shutdown()
{
    SAFE_DELETE(ms_textShader);
}

void UIText::Render()
{
    if (!Visibility)
        return;
    UIElement::Render();

    if (m_font == nullptr)
        return;

    unsigned int stride = VERTEXSTRUCTSIZE;
    unsigned int offset = 0;

    DXDEVICECONTEXT->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
    DXDEVICECONTEXT->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    DXDEVICECONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    ms_textShader->SetShaderParameters(m_font->GetTexture());
    ms_textShader->SetShaderParameters(ms_worldMatrix, ms_viewMatrix, ms_orthoMatrix);
    ms_textShader->Render(m_indexCount);
    ms_textShader->Reset();
}

void UIText::Update()
{
    if (!Visibility)
        return;
    UpdateAbsolutePosition();
    if (!m_changed)
        return;

    CalculateGeometry();

    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    result = DXDEVICECONTEXT->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
    {
        return;
    }

    memcpy((SPDNT2Vertex*)mappedResource.pData, (void*)m_vertices, VERTEXSTRUCTSIZE * m_vertexCount);
    DXDEVICECONTEXT->Unmap(m_vertexBuffer, 0);

    GraphicBase::Update();
    m_changed = false;
}

void UIText::SetText(std::wstring text)
{
    if (m_text == text)
        return;
    m_text = text;
    m_changed = true;

    EnsureVertexMemory(m_text.length() * 4, m_text.length() * 6, 16);
    CalculateTextWidth();
    CalculateGeometry();
    LoadBuffers();
}

std::wstring UIText::GetText()
{
    return m_text;
}

void UIText::SetFont(Font* font)
{
    if (m_font == font)
        return;
    m_font = font;
    m_changed = true;
}

void UIText::SetFontSize(float size)
{
    if (m_fontHeight == size)
        return;
    m_fontHeight = size;
    m_changed = true;
    CalculateTextWidth();
}

void UIText::SetFontColor(Color color)
{
    if (m_fontColor == color)
        return;
    m_fontColor = color;
    m_changed = true;
}

void UIText::SetTextAlignment(HorizontalTextAlignment horizontalTextAlignment)
{
    SetTextAlignment(horizontalTextAlignment, m_verticalAlign);
}

void UIText::SetTextAlignment(VerticalTextAlignment verticalTextAlignment)
{
    SetTextAlignment(m_horizontalAlign, verticalTextAlignment);
}

void UIText::SetTextAlignment(HorizontalTextAlignment horizontalTextAlignment, VerticalTextAlignment verticalTextAlignment)
{
    if (m_horizontalAlign == horizontalTextAlignment && m_verticalAlign == verticalTextAlignment)
        return;
    m_horizontalAlign = horizontalTextAlignment;
    m_verticalAlign = verticalTextAlignment;
    m_changed = true;
}

float UIText::GetFontHeight()
{
    return m_fontHeight;
}

float UIText::GetTextWidth()
{
    return m_textWidth;
}

void UIText::OnTextInput(wchar_t character)
{
    if (character == L'\b')
    {
        std::wstring text = GetText();

        if (!text.empty())
        {
            text.pop_back();
            SetText(text);
        }
    }
    else if (character == L'\r' || character == VK_ESCAPE)
    {
        RemoveFocus(this->GetFocusType());
    }
    else if (character >= 32)
    {
        std::wstring text = GetText();
        text += character;
        SetText(text);
    }
}

void UIText::ConfirmShader()
{
    if (ms_textShader == nullptr)
        ms_textShader = new FontShader();
}

float UIText::CalculateLineWidth(const std::wstring& text)
{
    if (m_font == nullptr)
        return 0.0f;

    float scale = m_fontHeight / m_font->GetFontHeight();
    float width = 0.0f;

    for (wchar_t character : text)
    {
        width += (m_font->GetCharacter(character).size * scale);
    }
    return width;
}

void UIText::CalculateTextWidth()
{
    std::vector<std::wstring> lines;
    std::wstringstream textStream(m_text);
    std::wstring singleLine;
    while (std::getline(textStream, singleLine))
    {
        lines.push_back(singleLine);
    }

    if (lines.empty() && !m_text.empty()) lines.push_back(m_text);

    float finalWidth = 0.0f;
    for (const auto& line : lines)
    {
        float lineWidth = CalculateLineWidth(line);
        if (lineWidth > finalWidth)
            finalWidth = lineWidth;
    }
    m_textWidth = finalWidth;
}

void UIText::CalculateGeometry()
{
    // Separate text into lines 
    std::vector<std::wstring> lines;
    std::wstringstream textStream(m_text);
    std::wstring singleLine;
    while (std::getline(textStream, singleLine))
    {
        lines.push_back(singleLine);
    }

    if (lines.empty() && !m_text.empty()) lines.push_back(m_text);

    unsigned long vertexIndex = 0;
    float nativeHeight = m_font->GetFontHeight();
    float scale = m_fontHeight / nativeHeight;

    float verticalSpace = m_size.Height - (lines.size() * m_fontHeight);

    float top;
    if (m_verticalAlign == VERTICAL_TEXT_ALIGNMENT_MIDDLE)              top = m_absolutePosition.Y + (verticalSpace * 0.5f);
    else if (m_verticalAlign == VERTICAL_TEXT_ALIGNMENT_BOTTOM)         top = m_absolutePosition.Y + verticalSpace;
    else                                                                top = m_absolutePosition.Y;

    // Go through each line
    for (const auto& line : lines)
    {
        float horizontalSpace = m_size.Width - CalculateLineWidth(line);
        float left;
        if (m_horizontalAlign == HORIZONTAL_TEXT_ALIGNMENT_CENTER)      left = m_absolutePosition.X + (horizontalSpace * 0.5f);
        else if (m_horizontalAlign == HORIZONTAL_TEXT_ALIGNMENT_RIGHT)  left = m_absolutePosition.X + horizontalSpace;
        else                                                            left = m_absolutePosition.X;

        // Go through each character of the line
        for (wchar_t character : line)
        {
            CharacterData data = m_font->GetCharacter(character);
            float renderWidth = data.size * scale;

            m_vertices[vertexIndex + 0].position = TPosition{ left, top, 0.0f };
            m_vertices[vertexIndex + 0].diffuse = TDiffuse{ m_fontColor.r, m_fontColor.g, m_fontColor.b };
            m_vertices[vertexIndex + 0].texCoord = TTextureCoordinate{ data.left, data.top };

            m_vertices[vertexIndex + 1].position = TPosition{ left + renderWidth, top, 0.0f };
            m_vertices[vertexIndex + 1].diffuse = TDiffuse{ m_fontColor.r, m_fontColor.g, m_fontColor.b };
            m_vertices[vertexIndex + 1].texCoord = TTextureCoordinate{ data.right, data.top };

            m_vertices[vertexIndex + 2].position = TPosition{ left, top + m_fontHeight, 0.0f };
            m_vertices[vertexIndex + 2].diffuse = TDiffuse{ m_fontColor.r, m_fontColor.g, m_fontColor.b };
            m_vertices[vertexIndex + 2].texCoord = TTextureCoordinate{ data.left, data.bottom };

            m_vertices[vertexIndex + 3].position = TPosition{ left + renderWidth, top + m_fontHeight, 0.0f };
            m_vertices[vertexIndex + 3].diffuse = TDiffuse{ m_fontColor.r, m_fontColor.g, m_fontColor.b };
            m_vertices[vertexIndex + 3].texCoord = TTextureCoordinate{ data.right, data.bottom };

            left += renderWidth;

            vertexIndex += 4;
        }

        top += m_fontHeight;
    }

    m_vertexCount = vertexIndex;
    m_indexCount = (vertexIndex / 4) * 6;
}

void UIText::AddTextProperty(TextProperty property)
{
    m_properties |= property;
    EnsureInputState();
}

void UIText::RemoveTextProperty(TextProperty property)
{
    m_properties &= ~property;
    EnsureInputState();
}

void UIText::SetTextProperties(unsigned int properties)
{
    m_properties = properties;
    EnsureInputState();
}

void UIText::EnsureInputState()
{
    if (HasTextProperty(TextProperty::TEXT_PROPERTY_EDITABLE))
    {
        m_requiredInputState = InputState::INPUT_STATE_TEXTEDITING;
        m_interactive = true;
    }
    else
    {
        m_requiredInputState = InputState::INPUT_STATE_GAMEPLAY;
        m_interactive = false;
    }
}