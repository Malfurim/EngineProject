#include "UIText.h"

// --- ADDITIONAL INCLUDES ---
#include <sstream>
#include "DirectXManager.h"
#include "TimerManager.h"
#include "FontShader.h"
#include "Font.h"
#include "Deleters.h"
#include "Utils.h"

// --- MACROS & DEFINES ---


// --- FORWARD DECLARATIONS ---


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC GLOBAL STATES & DATA								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
FontShader* UIText::ms_textShader = nullptr;

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CONSTRUCTORS & DESTRUCTOR								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
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

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CORE FUNCTIONS											//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//bool UIText::Initialize()
//{
//	return true;
//}

void UIText::Update()
{
    if (!Visibility)
        return;
    UpdateAbsolutePosition();

    if (IsFocused())
        CursorBlink();

    if (!IsInvalidated())
        return;

    ResetBlink();
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
    Validate();
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

void UIText::Shutdown()
{
    SAFE_DELETE(ms_textShader);
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	VIRTUAL FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void UIText::OnCharInput(wchar_t character)
{
    if (!HasTextProperty(TextProperty::TEXT_PROPERTY_EDITABLE))
        return;

    switch (character)
    {
    case L'\b':
        if (m_cursorIndex > 0)
        {
            m_text.erase(m_cursorIndex - 1, 1);
            m_cursorIndex--;
            m_updateCursorPosition = true;
            Invalidate();
        }
        break;

    case L'\r':     // ENTER
        if (HasTextProperty(TextProperty::TEXT_PROPERTY_MULTILINE))
        {
            m_text.insert(m_cursorIndex, 1, L'\n');
            m_cursorIndex++;
            m_updateCursorPosition = true;
            Invalidate();
        }
        else
        {
            RemoveFocus(this->GetFocusType());
        }
        break;

    case VK_ESCAPE:
        RemoveFocus(this->GetFocusType());
        break;

    default:
        if (character >= 32)
        {
            if (HasTextProperty(TextProperty::TEXT_PROPERTY_NUMBERS_ONLY) && (character < '0' || character > '9'))
                break;

            if (m_maxLength > 0 && static_cast<int>(m_text.length()) >= m_maxLength)
                break;

            m_text.insert(m_cursorIndex, 1, character);
            m_cursorIndex++;
            m_updateCursorPosition = true;
            Invalidate();
        }
        break;
    }

    if (IsInvalidated())
    {
        if (m_maxLength == 0)
            SetVertexMemory();
        CalculateTextWidth();
    }
}

void UIText::OnKeyDown(wchar_t key)
{
    if (!HasTextProperty(TextProperty::TEXT_PROPERTY_EDITABLE))
        return;

    switch (key)
    {
    case VK_DELETE:
        if (m_cursorIndex < m_text.length())
        {
            m_text.erase(m_cursorIndex, 1);
            m_updateCursorPosition = true;
            Invalidate();
        }
        break;

    case VK_LEFT:
        if (m_cursorIndex > 0)
        {
            m_cursorIndex--;
            m_updateCursorPosition = true;
            Invalidate();
        }
        break;

    case VK_RIGHT:
        if (m_cursorIndex < m_text.length())
        {
            m_cursorIndex++;
            m_updateCursorPosition = true;
            Invalidate();
        }
        break;

    case VK_UP:
        if (m_cursorPosition.Y - m_fontHeight < m_textTop)
        {
            m_cursorIndex = 0;
            m_updateCursorPosition = true;
            Invalidate();
        }
        else
        {
            m_cursorPosition.Y -= m_fontHeight;
            m_updateCursorPosition = false;
            m_locateCursorIndex = true;
            Invalidate();
        }
        break;

    case VK_DOWN:
        if (m_cursorPosition.Y + m_fontHeight > m_textBottom - m_fontHeight)
        {
            m_cursorIndex = m_text.length();
            m_updateCursorPosition = true;
            Invalidate();
        }
        else
        {
            m_cursorPosition.Y += m_fontHeight;
            m_updateCursorPosition = false;
            m_locateCursorIndex = true;
            Invalidate();
        }
        break;
    }

    if (IsInvalidated())
    {
        if (m_maxLength == 0)
            SetVertexMemory();
        CalculateTextWidth();
    }
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CLASS API												//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
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

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	GETTERS & SETTERS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void UIText::SetText(std::wstring text)
{
    if (m_maxLength > 0 && text.length() > static_cast<size_t>(m_maxLength))
        text = text.substr(0, m_maxLength);

    if (m_text == text)
        return;
    m_text = text;
    Invalidate();

    if (m_maxLength == 0)
        SetVertexMemory();

    CalculateTextWidth();
    CalculateGeometry();
}

void UIText::SetFont(Font* font)
{
    if (m_font == font || font == nullptr)
        return;
    m_font = font;
    if (m_fontHeight == 0.0f)
        m_fontHeight = font->GetFontHeight();
    Invalidate();
}

void UIText::SetFontSize(float size)
{
    if (m_fontHeight == size)
        return;
    m_fontHeight = size;
    Invalidate();
    CalculateTextWidth();
}

void UIText::SetFontColor(Color color)
{
    if (m_fontColor == color)
        return;
    m_fontColor = color;
    Invalidate();
}

void UIText::SetCursorColor(Color color)
{
    if (m_cursorColor == color)
        return;
    m_cursorColor = color;
    Invalidate();
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
    Invalidate();
}

void UIText::SetTextProperties(unsigned int properties)
{
    m_properties = properties;
    EnsureInputState();
}

void UIText::SetMaxLength(int length)
{
    if (m_maxLength == length)
        return;

    m_maxLength = length;
    SetVertexMemory();
    Invalidate();
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC CLASS API										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	VIRTUAL FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void UIText::LeftClickInternal(float mouseX, float mouseY)
{
    m_cursorPosition.X = mouseX;
    m_cursorPosition.Y = mouseY;
    m_updateCursorPosition = true;
    m_locateCursorIndex = true;
    Invalidate();
}

void UIText::FocusInternal()
{
    Invalidate();
}

void UIText::FocusLostInternal()
{
    Invalidate();
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PRIVATE FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void UIText::ConfirmShader()
{
    if (ms_textShader == nullptr)
        ms_textShader = new FontShader();
}

float UIText::CalculateLineWidth(const std::wstring& text) const
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

    // If text empty, push text anyway to get 1 line
    if (lines.empty() && !m_text.empty())
        lines.push_back(m_text);

    // Index for cursor index counting
    int index = 0;
    // Calculate index in vertex array for the cursor
    unsigned long cursorIndex = (m_text.length() - CountOf(m_text, '\n')) * 4;
    // Index for correct vertex indexing in vertices
    unsigned long vertexIndex = 0;
    float scale = m_fontHeight / m_font->GetFontHeight();

    float top = GetVerticalStartPosition(lines.size());
    Position cursorPosition = { 0.0f, top };
    // If text empty, put cursor in alignment start
    if (m_text.empty())
    {
        cursorPosition.X = GetHorizontalStartPosition(L"");
        cursorPosition.Y = GetVerticalStartPosition(1);
    }

    // Get top and bottom Y of rendered text
    m_textTop = top;
    m_textBottom = top + lines.size() * m_fontHeight;

    // Set max distance for distance check from cursor position to currently rendered character
    float currentDistance = m_size.Width * m_size.Width + (m_size.Height * m_fontHeight) * (m_size.Height * m_fontHeight);

    // Go through each line
    for (const auto& line : lines)
    {
        float left = GetHorizontalStartPosition(line);
        // Go through each character of the line
        for (wchar_t character : line)
        {
            CharacterData data = m_font->GetCharacter(character);
            float renderWidth = data.size * scale;
            SetVertices(vertexIndex, left, top, renderWidth, data);

            if (m_locateCursorIndex)
            {
                // Check if the cursor position is in character's render rectangle
                bool hitX = (m_cursorPosition.X >= left && m_cursorPosition.X < left + renderWidth);
                bool hitY = (m_cursorPosition.Y >= top && m_cursorPosition.Y < top + m_fontHeight);

                if (hitX && hitY)
                {
                    // If on left side and slightly on right, get character's index, else get next character on right
                    if (m_cursorPosition.X < left + (renderWidth * 0.6f))
                        m_cursorIndex = index;
                    else
                        m_cursorIndex = index + 1;
                    m_locateCursorIndex = false;
                }

                // If cursor position doesn't hit character, find closest character to the cursor position
                if (m_locateCursorIndex)
                {
                    // Calculate left and right render position of the character
                    float distanceLeft = m_cursorPosition.X - left;
                    float distanceRight = m_cursorPosition.X - (left + renderWidth);
                    // Calculate Y distance for Pythagorean formula
                    float distanceY = ((m_cursorPosition.Y - top) * m_fontHeight) * ((m_cursorPosition.Y - top) * m_fontHeight);

                    // Calculate real left and right distances using Pythagorean formula (X * X) + Y from above
                    distanceLeft = distanceLeft * distanceLeft + distanceY;
                    distanceRight = distanceRight * distanceRight + distanceY;

                    // Set left distance as current distance if closer than current one and set character's index
                    if (distanceLeft < currentDistance)
                    {
                        currentDistance = distanceLeft;
                        m_cursorIndex = index;
                    }
                    // Set right distance as current distance if closer than current one and set next character's index on right
                    if (distanceRight < currentDistance)
                    {
                        currentDistance = distanceRight;
                        m_cursorIndex = index + 1;
                    }
                }
            }

            // If character found in the line, set cursor position to found character
            if (index == m_cursorIndex && IsFocused())
            {
                cursorPosition = { left, top };
            }

            // Move right by character's width, offset vertex index for next character and increase character index
            left += renderWidth;
            vertexIndex += 4;
            index++;
        }

        // If cursor position doesn't hit character, check if it hits empty line through Y position
        if (m_locateCursorIndex)
        {
            if (m_cursorPosition.Y >= top && m_cursorPosition.Y < top + m_fontHeight && line.empty())
            {
                m_cursorIndex = index;
                m_locateCursorIndex = false;
            }
        }

        // If index found in empty line, set cursor position to \n character of the line
        if (index == m_cursorIndex && IsFocused())
        {
            cursorPosition = { left, top };
        }

        // Move to next line and increase character index for invisible \n
        top += m_fontHeight;
        index++;
    }

    // If cursor at the end of the text and empty line, set position to aligned X position of empty line
    if (m_cursorIndex == m_text.length() && IsFocused())
    {
        if (!m_text.empty() && m_text.back() == '\n')
        {
            cursorPosition = { GetHorizontalStartPosition(L""), top };
        }
    }

    // If focused, set vertices for cursor
    if (IsFocused())
    {
        CharacterData cursorData = m_font->GetCharacter('|');
        SetVertices(cursorIndex, cursorPosition.X, cursorPosition.Y, cursorData.size * scale, cursorData, m_cursorColor);
    }

    // If update cursor position requested, set cursor position from indexed character
    if (m_updateCursorPosition)
    {
        m_cursorPosition = cursorPosition;
    }

    // If focused, add vertices and indices to render cursor
    m_vertexCount = vertexIndex + (IsFocused() ? 4 : 0);
    m_indexCount = (vertexIndex / 4) * 6 + (IsFocused() ? 6 : 0);
    m_locateCursorIndex = false;
}

void UIText::SetVertices(unsigned long vertexStartIndex, float left, float top, float width, const CharacterData& data)
{
    SetVertices(vertexStartIndex, left, top, width, data, m_fontColor);
}

void UIText::SetVertices(unsigned long vertexStartIndex, float left, float top, float width, const CharacterData& data, const Color& color)
{
    m_vertices[vertexStartIndex + 0].position = TPosition{ left, top, 0.0f };
    m_vertices[vertexStartIndex + 0].diffuse = TDiffuse{ color.r, color.g, color.b };
    m_vertices[vertexStartIndex + 0].texCoord = TTextureCoordinate{ data.left, data.top };

    m_vertices[vertexStartIndex + 1].position = TPosition{ left + width, top, 0.0f };
    m_vertices[vertexStartIndex + 1].diffuse = TDiffuse{ color.r, color.g, color.b };
    m_vertices[vertexStartIndex + 1].texCoord = TTextureCoordinate{ data.right, data.top };

    m_vertices[vertexStartIndex + 2].position = TPosition{ left, top + m_fontHeight, 0.0f };
    m_vertices[vertexStartIndex + 2].diffuse = TDiffuse{ color.r, color.g, color.b };
    m_vertices[vertexStartIndex + 2].texCoord = TTextureCoordinate{ data.left, data.bottom };

    m_vertices[vertexStartIndex + 3].position = TPosition{ left + width, top + m_fontHeight, 0.0f };
    m_vertices[vertexStartIndex + 3].diffuse = TDiffuse{ color.r, color.g, color.b };
    m_vertices[vertexStartIndex + 3].texCoord = TTextureCoordinate{ data.right, data.bottom };
}

float UIText::GetVerticalStartPosition(size_t lineCount) const
{
    float verticalSpace = m_size.Height - lineCount * m_fontHeight;
    if (m_verticalAlign == VERTICAL_TEXT_ALIGNMENT_MIDDLE)
        return m_absolutePosition.Y + (verticalSpace * 0.5f);
    if (m_verticalAlign == VERTICAL_TEXT_ALIGNMENT_BOTTOM)
        return m_absolutePosition.Y + verticalSpace;
    return m_absolutePosition.Y;
}

float UIText::GetHorizontalStartPosition(const std::wstring& line) const
{
    float horizontalSpace = m_size.Width - CalculateLineWidth(line);
    if (m_horizontalAlign == HORIZONTAL_TEXT_ALIGNMENT_CENTER)
        return m_absolutePosition.X + (horizontalSpace * 0.5f);
    if (m_horizontalAlign == HORIZONTAL_TEXT_ALIGNMENT_RIGHT)
        return m_absolutePosition.X + horizontalSpace;
    return m_absolutePosition.X;
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

    if (m_maxLength == 0)
    {
        SetVertexMemory();
    } 
}

void UIText::SetVertexMemory()
{
    if (m_maxLength > 0)
    {
        EnsureVertexMemory(m_maxLength * 4 + 4, m_maxLength * 6 + 6, 0);    // if max length set, create buffers exactly for the capacity length
    }
    else
    {
        if (HasTextProperty(TextProperty::TEXT_PROPERTY_EDITABLE))
            EnsureVertexMemory(m_text.length() * 4 + 4, m_text.length() * 6 + 6, 16);   // add 4 vertices and 6 indices to render the cursor
        else
            EnsureVertexMemory(m_text.length() * 4, m_text.length() * 6, 16);
    }
}

void UIText::MoveTextCursor(CursorDirection direction)
{
    int newIndex = m_cursorIndex + direction;
    if (newIndex < 0)
        newIndex = 0;
    if (newIndex > m_text.length())
        newIndex = m_text.length();

    m_cursorIndex = newIndex;
    Invalidate();
}

void UIText::MoveTextCursor(float positionX, float positionY)
{

}

void UIText::CursorBlink()
{
    m_blinkTimer += DELTA;
    if (m_blinkTimer >= 0.5)
    {
        m_cursorBlink = !m_cursorBlink;
        m_indexCount = (m_vertexCount / 4) * 6 - (m_cursorBlink ? 6 : 0);
        m_blinkTimer -= 0.5;
    }
}

void UIText::ResetBlink()
{
    m_blinkTimer = 0.0;
    m_cursorBlink = false;
}
