#include "WorldManager.h"

// --- ADDITIONAL INCLUDES ---
#include "WorldObject.h"
#include "Deleters.h"

// --- MACROS & DEFINES ---


// --- FORWARD DECLARATIONS ---


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC GLOBAL STATES & DATA								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
WorldManager* WorldManager::ms_instance = nullptr;

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CONSTRUCTORS & DESTRUCTOR								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
WorldManager::WorldManager()
{
}

WorldManager::~WorldManager()
{
	SAFE_DELETE_VECTOR(m_worldObjects);
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CORE FUNCTIONS											//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//bool WorldManager::Initialize()
//{
//	return true;
//}

void WorldManager::Update()
{
	for (auto* obj : m_worldObjects)
	{
		obj->Update();
	}
}

void WorldManager::Render()
{
	for (auto* obj : m_worldObjects)
	{
		obj->Render();
	}
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PUBLIC VIRTUAL FUNCTIONS								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CLASS API												//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
WorldObject* WorldManager::CreateObject(const WorldObjectDesc& desc)
{
	WorldObject* object = WorldObject::Create(desc.mesh, desc.material);
	if (object != nullptr)
	{
		object->SetPosition(desc.position);
		object->SetRotation(desc.rotation);
		object->SetScale(desc.scale);
		object->Visibility = desc.visibility;
		m_worldObjects.push_back(object);
	}
	return object;
}

bool WorldManager::DestroyObject(WorldObject* object)
{
	auto it = std::find(m_worldObjects.begin(), m_worldObjects.end(), object);
	if (it != m_worldObjects.end())
	{
		WorldObject* object = *it;
		m_worldObjects.erase(it);
		delete object;
		return true;
	}
	return false;
}

bool WorldManager::ContainsObject(WorldObject* object)
{
	for (auto* obj : m_worldObjects)
	{
		if (obj == object)
			return true;
	}
	return false;
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	GETTERS & SETTERS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC CLASS API										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
WorldManager* WorldManager::Instance()
{
	if (ms_instance == nullptr)
		ms_instance = new WorldManager();
	return ms_instance;
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED VIRTUAL FUNCTIONS								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PRIVATE FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //

