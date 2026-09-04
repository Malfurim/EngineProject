#pragma once

#include "UIElement.h"
#include <vector>
#include <algorithm>

class UIPanel : public UIElement
{
public:
	UIPanel();
	UIPanel(Position position, Size size);
	UIPanel(Position position, Size size, Color backgroundColor);
	~UIPanel();

	void Render();
	void Update();

	void AddChild(UIElement* element);
	UIElement* GetChild(int index);
	bool ContainsChild(UIElement* element);
	bool RemoveChild(UIElement* element);
	bool RemoveChild(int index);
	void SortChildren();

	UIElement* GetElementAtPosition(float x, float y);

private:
	std::vector<UIElement*> m_children;
};

