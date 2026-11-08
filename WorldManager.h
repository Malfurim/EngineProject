#pragma once

// --- INCLUDES ---
#include <vector>
#include "DXMath.h"

// --- CONSTANTS ---


// --- MACROS & DEFINES ---
#define WORLD WorldManager::Instance()
#define WORLDUPDATE WorldManager::Instance()->Update()
#define WORLDRENDER WorldManager::Instance()->Render()

// --- FORWARD DECLARATIONS ---
class WorldObject;
class Mesh;
class Material;

// --- ENUMS & STRUCTS ---
struct WorldObjectDesc
{
	Mesh* mesh{ nullptr };
	Material* material{ nullptr };
	DXFLOAT3 position{ 0.0f, 0.0f, 0.0f };
	DXFLOAT3 rotation{ 0.0f, 0.0f, 0.0f };
	DXFLOAT3 scale{ 1.0f, 1.0f, 1.0f };
	bool visibility{ true };
};

class WorldManager
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	WorldManager();
	~WorldManager();

	// --- CORE FUNCTIONS ---
	// bool Initialize();
	void Update();
	void Render();

	// --- PUBLIC VIRTUAL FUNCTIONS ---


	// --- CLASS API ---
	WorldObject* CreateObject(const WorldObjectDesc& desc);
	bool DestroyObject(WorldObject* object);
	bool ContainsObject(WorldObject* object);

	// --- GETTERS & SETTERS ---


	// --- STATIC CLASS API ---
	static WorldManager* Instance();

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
	std::vector<WorldObject*> m_worldObjects;

	// --- STATIC GLOBAL STATES ---
	static WorldManager* ms_instance;
};