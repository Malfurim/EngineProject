#pragma once

// --- INCLUDES ---
#include "Interactive.h"
#include "DXMath.h"

// --- CONSTANTS ---


// --- MACROS & DEFINES ---


// --- ENUMS & STRUCTS ---


// --- FORWARD DECLARATIONS ---
class Mesh;
class Material;

class WorldObject : public Interactive
{
protected:
	enum UpdateFlag : unsigned int
	{
		UPDATE_NONE = 0,
		UPDATE_POSITION = 1 << 0,

		UPDATE_ALL = UPDATE_POSITION
	};

// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	~WorldObject() override;

	// --- CORE FUNCTIONS ---
	// bool Initialize();
	void Update();
	void Render();

	// --- PUBLIC VIRTUAL FUNCTIONS ---


	// --- CLASS API ---


	// --- GETTERS & SETTERS ---
	void SetPosition(float x, float y, float z) { m_position = { x, y, z }; Invalidate(UpdateFlag::UPDATE_POSITION); }
	void SetPosition(DXFLOAT3 position) { m_position = position; Invalidate(UpdateFlag::UPDATE_POSITION); }
	DXFLOAT3 GetPosition() const { return m_position; }

	void SetRotation(float x, float y, float z) { m_rotation = { x, y, z }; Invalidate(UpdateFlag::UPDATE_POSITION); }
	void SetRotation(DXFLOAT3 rotation) { m_rotation = rotation; Invalidate(UpdateFlag::UPDATE_POSITION); }
	DXFLOAT3 GetRotation() const { return m_rotation; }

	void SetScale(float x, float y, float z) { m_scale = { x, y, z }; Invalidate(UpdateFlag::UPDATE_POSITION); }
	void SetScale(DXFLOAT3 scale) { m_scale = scale; Invalidate(UpdateFlag::UPDATE_POSITION); }
	DXFLOAT3 GetScale() const { return m_scale; }
	

	// --- STATIC CLASS API ---
	static WorldObject* Create(Mesh* mesh, Material* material);
	static void SetViewMatrix(const DXMATRIX& viewMatrix) { ms_viewMatrix = viewMatrix; }
	static void SetProjectionMatrix(const DXMATRIX& projectionMatrix) { ms_projectionMatrix = projectionMatrix; }

protected:
	// --- CONSTRUCTORS ---
	WorldObject(Mesh* mesh, Material* material);

	// --- PROTECTED VIRTUAL FUNCTIONS ---
	bool IsMouseOver() const override;

	// --- PROTECTED FUNCTIONS ---
	void Invalidate() { m_updateFlags = UpdateFlag::UPDATE_ALL; }
	void Invalidate(UpdateFlag flag) { m_updateFlags |= flag; }
	bool IsInvalidated() const { return m_updateFlags != UPDATE_NONE; }
	bool IsInvalidated(UpdateFlag flag) const { return (m_updateFlags & flag) != 0; }
	void Validate(UpdateFlag flag) { m_updateFlags &= ~flag; }

private:
	// --- CONSTRUCTORS ---


	// --- PRIVATE FUNCTIONS ---
	

// - PROPERTIES -
public:
	// --- PUBLIC COMPONENT STATES ---
	bool Visibility{ true };

	// --- PUBLIC COMPONENT DATA ---


protected:
	// --- INTERNAL CONSTANTS ---


	// --- INTERNAL STRUCTS & ENUMS ---


	// --- INTERNAL COMPONENT STATES ---
	unsigned int m_updateFlags{ UpdateFlag::UPDATE_NONE };

	// --- INTERNAL COMPONENT DATA ---
	Mesh* m_mesh{ nullptr };
	Material* m_material{ nullptr };
	DXFLOAT3 m_position{ 0.0f, 0.0f, 0.0f };
	DXFLOAT3 m_rotation{ 0.0f, 0.0f, 0.0f };
	DXFLOAT3 m_scale{ 1.0f, 1.0f, 1.0f };

	DXMATRIX m_worldMatrix;
	static DXMATRIX ms_viewMatrix;
	static DXMATRIX ms_projectionMatrix;

private:
	// --- PRIVATE CONSTANTS ---


	// --- PRIVATE STRUCTS & ENUMS ---


	// --- PRIVATE COMPONENT STATES ---
	

	// --- PRIVATE COMPONENT DATA ---
	

	// --- STATIC GLOBAL STATES ---

};