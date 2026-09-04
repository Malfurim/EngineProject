#include "ResourceManager.h"
#include "Window.h"
#include "DirectXManager.h"
#include "Utils.h"

ResourceManager* ResourceManager::ms_instance = nullptr;

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	m_textureMap.clear();
	m_modelMap.clear();
	ms_instance = nullptr;
}

void ResourceManager::LoadAssets(const std::wstring& directoryPath)
{
	std::wstring basePath = directoryPath;
	if (!basePath.empty() && (basePath.back() == L'/' || basePath.back() == L'\\'))
	{
		basePath.pop_back();
	}

	ScanDirectory(basePath + L"/textures", &ResourceManager::LoadTexture);
	ScanDirectory(basePath + L"/ui", &ResourceManager::LoadUITexture);
	ScanDirectory(basePath + L"/models", &ResourceManager::LoadModel);
	ScanDirectory(basePath + L"/fonts", &ResourceManager::LoadFont);
}

void ResourceManager::ScanDirectory(const std::wstring& directoryPath, void (ResourceManager::*loadFunc)(const std::wstring&))
{
	HANDLE dir;
	WIN32_FIND_DATA fileData;

	if ((dir = FindFirstFile((directoryPath + L"/*").c_str(), &fileData)) == INVALID_HANDLE_VALUE)
		return; // No files found 

	do
	{
		const std::wstring fileName = fileData.cFileName;
		const std::wstring fullPath = directoryPath + L"/" + fileName;
		const bool is_directory = ((fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);

		if (fileName[0] == L'.')
			continue;		// skip "." and ".."

		if (is_directory)	// if it's a directory, scan the sub directory first
		{
			
			ScanDirectory(fullPath, loadFunc);
		}
		else				// otherwise load the file
		{
			(this->*loadFunc)(fullPath);
		}
	} while (FindNextFile(dir, &fileData));

	FindClose(dir);
}

void ResourceManager::LoadTexture(const std::wstring& path)
{
	std::wstring name = GetFilename(path);

	if (m_textureMap.find(name) != m_textureMap.end())
		return;

	Texture* texture = new Texture();
	if (!texture->Initialize(path.c_str(), true))
	{
		delete texture;
		texture = nullptr;
		return;
	}
	m_textureMap[name] = texture;
}

void ResourceManager::LoadUITexture(const std::wstring& path)
{
	std::wstring name = GetFilename(path);

	if (m_textureMap.find(name) != m_textureMap.end())
		return;

	Texture* texture = new Texture();
	if (!texture->Initialize(path.c_str(), false))
	{
		delete texture;
		texture = nullptr;
		return;
	}
	m_textureMap[name] = texture;
}

void ResourceManager::LoadModel(const std::wstring& path)
{
	std::wstring name = GetFilename(path);

	if (m_modelMap.find(name) != m_modelMap.end())
		return;

	Model* model = new Model();
	if (!model->Initialize(DXDEVICE, path.c_str(), false))
	{
		delete model;
		model = nullptr;
		return;
	}
	m_modelMap[name] = model;
}

void ResourceManager::LoadFont(const std::wstring& path)
{
	std::wstring name = GetFilename(path);

	if (m_fontMap.find(name) != m_fontMap.end())
		return;
	
	Font* font = new Font();
	if (!font->Initialize(path, GetTexture(name)))
	{
		delete font;
		font = nullptr;
		return;
	}
	m_fontMap[name] = font;
}

Texture* ResourceManager::GetTexture(std::wstring name)
{
	auto resource = m_textureMap.find(name);
	if (resource != m_textureMap.end())
	{
		return resource->second;
	}
	return nullptr;
}

Model* ResourceManager::GetModel(std::wstring name)
{
	auto resource = m_modelMap.find(name);
	if (resource != m_modelMap.end())
	{
		return resource->second;
	}
	return nullptr;
}

Font* ResourceManager::GetFont(std::wstring name)
{
	auto resource = m_fontMap.find(name);
	if (resource != m_fontMap.end())
	{
		return resource->second;
	}
	return nullptr;
}

ResourceManager* ResourceManager::Instance()
{
	if (ms_instance == nullptr)
		ms_instance = new ResourceManager();
	return ms_instance;
}
