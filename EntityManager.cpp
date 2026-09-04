#include "EntityManager.h"

EntityManager* EntityManager::ms_instance = nullptr;

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
	while (!m_entities.empty())
	{
		delete m_entities[0];
		m_entities.erase(m_entities.begin());
	}

	ms_instance = nullptr;
}

void EntityManager::LoadEntity(const std::wstring& path)
{
	std::wstring name = path;
	int posSlash = path.find_last_of(L"/");
	if (posSlash >= 0)
	{
		name = name.substr(posSlash + 1, name.length());
	}
	name = name.substr(0, name.find_last_of(L"."));

	if (GetEntity(name) != nullptr)
		return;

	Entity* entity = new Entity();
	if (!entity->Initialize(path, m_entities.size()))
	{
		delete entity;
		entity = nullptr;
		return;
	}

	m_entities.push_back(entity);
}

void EntityManager::LoadEntitiesFromDirectory(const std::wstring& directoryPath, bool loadSubDirectory)
{
	HANDLE dir;
	WIN32_FIND_DATA file_data;
	std::vector<std::wstring> files;
	std::wstring path;

	if (directoryPath.find_last_of(L"/") == directoryPath.length() - 1)
		path = directoryPath.substr(0, directoryPath.length() - 1);
	else
		path = directoryPath;

	if ((dir = FindFirstFile((path + L"/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
		return; /* No files found */

	do
	{
		const std::wstring file_name = file_data.cFileName;
		const std::wstring full_file_name = path + L"/" + file_name;
		const bool is_directory = ((file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);

		if (file_name[0] == '.')
			continue;

		if (is_directory && loadSubDirectory)
			LoadEntitiesFromDirectory(full_file_name, loadSubDirectory);
		else
			files.push_back(full_file_name);
	} while (FindNextFile(dir, &file_data));

	while (!files.empty())
	{
		LoadEntity(files[0]);
		files.erase(files.begin());
	}

	FindClose(dir);
}

Entity* EntityManager::GetEntity(std::wstring name)
{
	for (int i = 0; i < (int)m_entities.size(); ++i)
	{
		Entity* element = m_entities[i];
		std::wstring resourceName = element->GetName();

		if (!wcscmp(name.c_str(), resourceName.c_str()))
		{
			return element;
		}
	}

	return nullptr;
}

Entity* EntityManager::GetEntity(int index)
{
	if (index > m_entities.size() || index < 0)
		return nullptr;
	return m_entities[index];
}

EntityManager* EntityManager::Instance()
{
	if (ms_instance == nullptr)
		ms_instance = new EntityManager();
	return ms_instance;
}

EntityManager* EntityManager::Get()
{
	return ms_instance;
}