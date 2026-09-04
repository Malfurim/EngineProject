#pragma once

#include "GraphicBase.h"
#include "Position.h"

struct Size
{
	float Width;
	float Height;

	Size operator+(const Size& right)
	{
		return Size{ Width + right.Width, Height + right.Height };
	}
	Size operator+(const float& right)
	{
		return Size{ Width + right, Height + right };
	}
	Size operator+=(const Size& right)
	{
		Width += right.Width;
		Height += right.Height;
		return *this;
	}
	Size operator+=(const float& right)
	{
		Width += right;
		Height += right;
		return *this;
	}
	Size operator-(const Size& right)
	{
		return Size{ Width - right.Width, Height - right.Height };
	}
	Size operator-(const float& right)
	{
		return Size{ Width - right, Height - right };
	}
	Size operator-=(const Size& right)
	{
		Width -= right.Width;
		Height -= right.Height;
		return *this;
	}
	Size operator-=(const float& right)
	{
		Width -= right;
		Height -= right;
		return *this;
	}
};

class UIElement : public GraphicBase
{
	friend class UIPanel;

public:
	virtual ~UIElement();

	virtual void Render();
	virtual void Update();

	void SetPosition(Position position);
	void SetPosition(float x, float y);
	Position GetPosition() { return m_localPosition; }
	float GetPositionLeft() { return m_localPosition.X; }
	float GetPositionTop() { return m_localPosition.Y; }

	void SetSize(Size size) { m_size = size; m_changed = true; }
	void SetSize(float width, float height) { m_size = { width, height }; m_changed = true; }
	Size GetSize() { return m_size; }
	float GetWidth() { return m_size.Width; }
	float GetHeight() { return m_size.Height; }

	void SetZIndex(int index);
	int GetZIndex();

	const UIElement* GetParent();

protected:
	UIElement();
	UIElement(Position position, Size size);
	UIElement(Position position, Size size, Color backgroundColor);
	
	void SetRect(Position position, Size size);
	void SetRect(float posX, float posY, float sizeX, float sizeY);
	void SetParent(UIElement* element);
	float GetAbsoluteLeft() { return m_absolutePosition.X; }
	float GetAbsoluteTop() { return m_absolutePosition.Y; }
	void UpdateAbsolutePosition();
	void EnsureVertexMemory(unsigned int vertexCount, unsigned int indexCount, unsigned int reserveCount);

	bool CheckMouseOver() const override;

	Position m_absolutePosition{0.0f, 0.0f};
	Position m_localPosition{0.0f, 0.0f};
	Size m_size{0.0f, 0.0f};
	int m_zIndex{ 0 };
	unsigned long m_vertexCapacity{ 0 };
	unsigned long m_indexCapacity{ 0 };
	bool m_changed{ false };

	static XMMATRIX ms_worldMatrix;
	static XMMATRIX ms_viewMatrix;
	static XMMATRIX ms_orthoMatrix;

private:
	virtual void SortChildren() {}
	UIElement* m_parent{ nullptr };
};

