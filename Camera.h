#pragma once

// --- INCLUDES ---
#include <DirectXMath.h>

// --- CONSTANTS ---


// --- MACROS & DEFINES ---


// --- ENUMS & FLAGS ---


// --- FORWARD DECLARATIONS ---
using namespace DirectX;

class Camera
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	Camera();
	~Camera();

	// --- VIRTUAL FUNCTIONS ---
	// virtual bool Initialize();
	// virtual void Update();
	virtual void Render();

	// --- CLASS API ---


	// --- GETTERS & SETTERS ---
	XMFLOAT3 GetPosition() const { return m_position; }
	void SetPosition(float x, float y, float z) { m_position = { x, y, z }; }
	
	XMFLOAT3 GetRotation() const { return m_rotation; }
	void SetRotation(float x, float y, float z) { m_rotation = { x, y, z }; }

	void GetViewMatrix(XMMATRIX& viewMatrix);

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
	// --- INTERNAL COMPONENT STATES ---


	// --- INTERNAL COMPONENT DATA ---


private:
	// --- PRIVATE COMPONENT STATES ---


	// --- PRIVATE COMPONENT DATA ---
	XMFLOAT3 m_position{};
	XMFLOAT3 m_rotation{};

	XMMATRIX m_viewMatrix{};

	// --- STATIC GLOBAL STATES ---

};

