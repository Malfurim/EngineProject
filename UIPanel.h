#pragma once

// --- INCLUDES ---
#include "UIElement.h"
#include <vector>

// --- CONSTANTS ---


// --- MACROS & DEFINES ---


// --- ENUMS & STRUCTS ---


// --- FORWARD DECLARATIONS ---


class UIPanel : public UIElement
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	UIPanel();
	UIPanel(Position position, Size size);
	UIPanel(Position position, Size size, Color backgroundColor);
	~UIPanel();

	// --- CORE FUNCTIONS ---
	// bool Initialize();
	void Update();
	void Render();
	
	// --- VIRTUAL FUNCTIONS ---


	// --- CLASS API ---
	void AddChild(UIElement* element);
	bool ContainsChild(UIElement* element);
	bool RemoveChild(UIElement* element);
	bool RemoveChild(int index);
	void SortChildren();
	UIElement* GetElementAtPosition(float x, float y);

	// --- GETTERS & SETTERS ---
	UIElement* GetChild(int index);

	// --- STATIC CLASS API ---


protected:
	// --- CONSTRUCTORS ---


	// --- VIRTUAL FUNCTIONS ---


	// --- PROTECTED FUNCTIONS ---


private:
	// --- CONSTRUCTORS ---


	// --- PRIVATE FUNCTIONS ---


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


	// --- PRIVATE COMPONENT DATA ---
	std::vector<UIElement*> m_children;

	// --- STATIC GLOBAL STATES ---

};

