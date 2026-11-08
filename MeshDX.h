#pragma once

// --- INCLUDES ---
#include <string>
#include "ResourceBase.h"

// --- CONSTANTS ---


// --- MACROS & DEFINES ---


// --- ENUMS & STRUCTS ---


// --- FORWARD DECLARATIONS ---


class MeshDX : public ResourceBase
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	MeshDX();
	~MeshDX() override;

	// --- CORE FUNCTIONS ---
	bool Initialize(const std::wstring& filePath, const std::wstring& name, bool isStatic);
	// void Update();
	// void Render();

	// --- PUBLIC VIRTUAL FUNCTIONS ---


	// --- CLASS API ---


	// --- GETTERS & SETTERS ---


	// --- STATIC CLASS API ---


protected:
	// --- CONSTRUCTORS ---


	// --- PROTECTED VIRTUAL FUNCTIONS ---


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
	

	// --- STATIC GLOBAL STATES ---

};