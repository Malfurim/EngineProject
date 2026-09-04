#include "UIPanel.h"
#include <algorithm>
#include "Deleters.h"

UIPanel::UIPanel() : UIPanel({ 0,0 }, { 0,0 }, { 0, 0, 0, 0 })
{
}

UIPanel::UIPanel(Position position, Size size) : UIPanel(position, size, { 0, 0, 0, 0 })
{
}

UIPanel::UIPanel(Position position, Size size, Color backgroundColor) : UIElement(position, size, backgroundColor)
{
}

UIPanel::~UIPanel()
{
    SAFE_DELETE_VECTOR(m_children)
}

void UIPanel::Render()
{
    if (!Visibility)
        return;
    UIElement::Render();
    for (auto* child : m_children)
        child->Render();
}

void UIPanel::Update()
{
    if (!Visibility)
        return;
    UIElement::Update();
    for (auto* child : m_children)
        child->Update();
}

void UIPanel::AddChild(UIElement* element)
{
    if (element == nullptr)
        return;

    if (element->GetParent() != nullptr)
        return;

    if(ContainsChild(element))
        return;

    // Searches for first occurance when already added element has higher z index than currently being added element and saves it's index to add to correct position
    auto index = std::find_if(m_children.begin(), m_children.end(), [element](UIElement* element2) {
        return element2->m_zIndex > element->m_zIndex;
        });

    element->SetParent(this);
    m_children.insert(index, element);
    element->Update();
}

UIElement* UIPanel::GetChild(int index)
{
    if (index >= m_children.size() || index < 0)
        return nullptr;
    return m_children[index];
}

bool UIPanel::ContainsChild(UIElement* element)
{
    for (auto* child : m_children)
    {
        if (child == element)
            return true;
    }
    return false;
}

bool UIPanel::RemoveChild(UIElement* element)
{
    auto childIndex = std::find(m_children.begin(), m_children.end(), element);
    if (childIndex != m_children.end())
    {
        m_children.erase(childIndex);
        return true;
    }
    return false;
}

bool UIPanel::RemoveChild(int index)
{
    if (index >= m_children.size() || index < 0)
        return false;
    m_children.erase(m_children.begin() + index);
    return true;
}

void UIPanel::SortChildren()
{
    std::sort(m_children.begin(), m_children.end(), [](UIElement* element1, UIElement* element2) {
        return element1->m_zIndex < element2->m_zIndex;
        });
}

UIElement* UIPanel::GetElementAtPosition(float x, float y)
{
    if (!Visibility || !CheckMouseOver())
        return nullptr;

    for (auto it = m_children.rbegin(); it != m_children.rend(); ++it)
    {
        UIElement* element = *it;
        if (element == nullptr)
            continue;

        if (!element->Visibility)
            continue;

        if (element->CheckMouseOver())
        {
            UIPanel* elementPanel = dynamic_cast<UIPanel*>(element);
            if (elementPanel != nullptr)
            {
                UIElement* lowerElement = elementPanel->GetElementAtPosition(x, y);
                if (lowerElement != nullptr)
                    return lowerElement;
            }
            return element;
        }
    }
    return this;
}
