#pragma once

#include "Entity.h"
#include <string>
#include <vector>

#define ENTITY EntityManager::Instance()

class EntityManager
{
public:
	~EntityManager();

	void LoadEntity(const std::wstring& path);
	void LoadEntitiesFromDirectory(const std::wstring& directoryPath, bool loadSubDirectory);

	Entity* GetEntity(std::wstring entityName);
	Entity* GetEntity(int index);

	static EntityManager* Instance();
	static EntityManager* Get();

	int num = 5;

private:
	EntityManager();

	std::vector<Entity*> m_entities;

	static EntityManager* ms_instance;
};
