#pragma once

// --- INCLUDES ---
#include <vector>
#include "UIWindow.h"

// --- CONSTANTS ---


// --- MACROS & DEFINES ---
#define DEBUG DebugWindow::Instance()

// --- ENUMS & STRUCTS ---


// --- FORWARD DECLARATIONS ---


class DebugWindow : public UIWindow
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	~DebugWindow();
	
	// --- CORE FUNCTIONS ---
	// bool Initialize();
	// void Update();
	// void Render();

	// --- PUBLIC VIRTUAL FUNCTIONS ---


	// --- CLASS API ---
	void AddRow();
	void AddColumn();

	// --- GETTERS & SETTERS ---
	void SetGridSize(unsigned int columns, unsigned int rows);
	void SetText(unsigned int column, unsigned int row, const std::wstring& text);
	void AddText(unsigned int column, unsigned int row, const std::wstring& text);

	// --- STATIC CLASS API ---
	static DebugWindow* Instance();

protected:
	// --- CONSTRUCTORS ---


	// --- PROTECTED VIRTUAL FUNCTIONS ---


	// --- PROTECTED FUNCTIONS ---


private:
	// --- CONSTRUCTORS ---
	DebugWindow();

	// --- PRIVATE FUNCTIONS ---
	void EnsureTextBlocks();
	bool CheckVectorBounds(unsigned int column, unsigned int row);

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
	unsigned int m_textRows{ 1 };
	unsigned int m_textColumns{ 1 };
	unsigned int m_prevRows{ 0 };
	unsigned int m_prevColumns{ 0 };

	// --- PRIVATE COMPONENT DATA ---
	std::vector<UIText*> m_textBlocks;

	// --- STATIC GLOBAL STATES ---
	static DebugWindow* ms_instance;
};