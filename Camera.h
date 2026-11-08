#pragma once

// --- INCLUDES ---
#include "DXMath.h"

// --- CONSTANTS ---


// --- MACROS & DEFINES ---


// --- ENUMS & STRUCTS ---


// --- FORWARD DECLARATIONS ---

class Camera
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	Camera();
	~Camera();

	
	// --- CORE FUNCTIONS ---
	// bool Initialize();
	void Update();
	//void Render();

	// --- VIRTUAL FUNCTIONS ---


	// --- CLASS API ---

	// --- GETTERS & SETTERS ---
	DXFLOAT3 GetPosition() const { return m_position; }
	void SetPosition(float x, float y, float z) { m_position = { x, y, z }; }
	void SetPosition(DXFLOAT3 position) { m_position = position; }
	
	DXFLOAT3 GetRotation() const { return m_rotation; }
	void SetRotation(float x, float y, float z) { m_rotation = { x, y, z }; }
	void SetRotation(DXFLOAT3 rotation) { m_rotation = rotation; }

	DXMATRIX GetViewMatrix() const { return m_viewMatrix; }

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
	DXFLOAT3 m_position{};
	DXFLOAT3 m_rotation{};

	DXMATRIX m_viewMatrix{};

	// --- STATIC GLOBAL STATES ---

};

