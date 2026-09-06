#pragma once

// --- INCLUDES ---
#include "GraphicBase.h"
#include "Position.h"

// --- CONSTANTS ---


// --- MACROS & DEFINES ---


// --- ENUMS & STRUCTS ---
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

// --- FORWARD DECLARATIONS ---


class UIElement : public GraphicBase
{
	friend class UIPanel;
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	virtual ~UIElement();

	// --- CORE FUNCTIONS ---
	// bool Initialize();
	virtual void Update();
	virtual void Render();

	// --- VIRTUAL FUNCTIONS ---


	// --- CLASS API ---


	// --- GETTERS & SETTERS ---
	Position GetPosition() const { return m_localPosition; }
	float GetPositionLeft() const { return m_localPosition.X; }
	float GetPositionTop() const { return m_localPosition.Y; }
	float GetAbsoluteLeft() { return m_absolutePosition.X; }
	float GetAbsoluteTop() { return m_absolutePosition.Y; }
	void SetPosition(Position position);
	void SetPosition(float x, float y);

	Size GetSize() const { return m_size; }
	float GetWidth() const { return m_size.Width; }
	float GetHeight() const { return m_size.Height; }
	virtual void SetSize(Size size) { m_size = size; m_changed = true; }
	virtual void SetSize(float width, float height) { m_size = { width, height }; m_changed = true; }

	void SetZIndex(int index);
	int GetZIndex() const { return m_zIndex; }

	const UIElement* GetParent() const { return m_parent; }

	// --- STATIC CLASS API ---


protected:
	// --- CONSTRUCTORS ---
	UIElement();
	UIElement(Position position, Size size);
	UIElement(Position position, Size size, Color backgroundColor);

	// --- VIRTUAL FUNCTIONS ---
	bool CheckMouseOver() const override;

	// --- PROTECTED FUNCTIONS ---
	void SetRect(float posX, float posY, float sizeX, float sizeY);
	void SetRect(Position position, Size size);
	void SetParent(UIElement* element);
	void UpdateAbsolutePosition();
	void EnsureVertexMemory(unsigned int vertexCount, unsigned int indexCount, unsigned int reserveCount);

private:
	// --- CONSTRUCTORS ---


	// --- PRIVATE FUNCTIONS ---
	virtual void SortChildren() {}

// - PROPERTIES -
public:
	// --- PUBLIC COMPONENT STATES ---


	// --- PUBLIC COMPONENT DATA ---


protected:
	// --- INTERNAL CONSTANTS ---


	// --- INTERNAL STRUCTS & ENUMS ---


	// --- INTERNAL COMPONENT STATES ---
	bool m_changed{ false };

	// --- INTERNAL COMPONENT DATA ---
	Position m_absolutePosition{ 0.0f, 0.0f };
	Position m_localPosition{ 0.0f, 0.0f };
	Size m_size{ 0.0f, 0.0f };
	int m_zIndex{ 0 };
	unsigned long m_vertexCapacity{ 0 };
	unsigned long m_indexCapacity{ 0 };

	static XMMATRIX ms_worldMatrix;
	static XMMATRIX ms_viewMatrix;
	static XMMATRIX ms_orthoMatrix;

private:
	// --- PRIVATE CONSTANTS ---


	// --- PRIVATE STRUCTS & ENUMS ---


	// --- PRIVATE COMPONENT STATES ---


	// --- PRIVATE COMPONENT DATA ---
	UIElement* m_parent{ nullptr };

	// --- STATIC GLOBAL STATES ---

};

