#include "ResourceManager.h"

// --- ADDITIONAL INCLUDES ---
#include "Window.h"
#include "DirectXManager.h"
#include "TimerManager.h"
#include "Texture.h"
#include "Model.h"
#include "Font.h"
#include "Utils.h"
#include "Deleters.h"

// --- MACROS & DEFINES ---


// --- FORWARD DECLARATIONS ---


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC GLOBAL STATES & DATA								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
ResourceManager* ResourceManager::ms_instance = nullptr;
Texture* ResourceManager::ms_fallbackTexture = nullptr;

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CONSTRUCTORS & DESTRUCTOR								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
ResourceManager::ResourceManager()
{
	AddStaticDirectory(L"ui/");
	AddStaticDirectory(L"fonts/");

	ms_fallbackTexture = new Texture();
	if (!ms_fallbackTexture->Initialize(L"assets/textures/blank.png", L"blank", true))
	{
		#ifdef _DEBUG
		MessageBox(NULL, L"File assets/textures/blank.png doesn't exist", L"ERROR", MB_OK);
		#endif
	}
}

ResourceManager::~ResourceManager()
{
	ClearCache();
	ms_instance = nullptr;
	ms_fallbackTexture = nullptr;
}
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CORE FUNCTIONS											//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//bool ResourceManager::Initialize()
//{
//	return true;
//}

void ResourceManager::Update()
{
	double delta = DELTA;
	for (int i = m_unusedTextures.size() - 1; i >= 0; i--)
	{
		std::wstring name = m_unusedTextures[i];
		auto resource = m_dynamicTextureMap.find(name);
		if (resource == m_dynamicTextureMap.end())
		{
			m_unusedTextures.erase(m_unusedTextures.begin() + i);
			continue;
		}

		resource->second.timeSinceUnused += delta;
		if (resource->second.timeSinceUnused >= UNUSED_TEXTURE_LIFETIME)
		{
			delete resource->second.texture;
			resource->second.texture = nullptr;
			m_dynamicTextureMap.erase(name);
			m_unusedTextures.erase(m_unusedTextures.begin() + i);
		}
	}
}

//void ResourceManager::Render()
//{
//}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	VIRTUAL FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CLASS API												//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void ResourceManager::AddStaticDirectory(const std::wstring& directory)
{
	for (auto dir : m_disabledMipmapDirectories)
	{
		if (directory.compare(0, directory.length(), dir) == 0)
			return;
	}
	m_disabledMipmapDirectories.push_back(directory);
}

void ResourceManager::ReleaseTexture(const std::wstring& name)
{
	auto resource = m_dynamicTextureMap.find(name);
	if (resource == m_dynamicTextureMap.end())
		return;

	if (resource->second.referenceCount > 0)
	{
		resource->second.referenceCount--;
	}

	if (resource->second.referenceCount == 0)
	{
		resource->second.timeSinceUnused = 0.0;
		if (std::find(m_unusedTextures.begin(), m_unusedTextures.end(), resource->second.texture->GetName()) == m_unusedTextures.end())
		{
			m_unusedTextures.push_back(resource->second.texture->GetName());
		}
	}
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	GETTERS & SETTERS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
Texture* ResourceManager::GetTexture(const std::wstring& name, AssetPool poolMode)
{
	// Check if the texture is part of static directories
	bool isStatic = false;
	if (poolMode == AssetPool::ASSET_AUTOMATIC)
	{
		isStatic = CompareStaticDirectories(name);
	}
	else
	{
		isStatic = (poolMode == ASSET_STATIC);
	}

	// First try to find if the texture already exists
	if (isStatic)
	{
		auto resource = m_staticTextureMap.find(name);
		if (resource != m_staticTextureMap.end())
		{
			return resource->second;
		}
	}
	else
	{
		auto resource = m_dynamicTextureMap.find(name);
		if (resource != m_dynamicTextureMap.end())
		{
			resource->second.referenceCount++;
			resource->second.timeSinceUnused = 0.0;

			auto index = std::find(m_unusedTextures.begin(), m_unusedTextures.end(), name);
			if (index != m_unusedTextures.end())
			{
				m_unusedTextures.erase(index);
			}

			return resource->second.texture;
		}
	}

	// Second if it isn't found, try to create a new one
	std::wstring path = L"assets/textures/" + name + TEXTURE_FILE_EXTENSION;
	Texture* texture = new Texture();
	if (!texture->Initialize(path, name, !isStatic))
	{
		#ifdef _DEBUG
		MessageBox(NULL, (L"Texture not loaded: " + path).c_str(), L"Path Error", MB_OK);
		#endif
		delete texture;
		texture = nullptr;
		return ms_fallbackTexture;
	}

	// Save the texture
	if (isStatic)
	{
		m_staticTextureMap[name] = texture;
	}
	else
	{
		TextureResource res;
		res.texture = texture;
		res.referenceCount = 1;
		res.timeSinceUnused = 0.0;
		m_dynamicTextureMap[name] = res;
	}

	// Return newly created texture
	return texture;
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
	
	std::wstring path = L"assets/fonts/" + name + FONT_FILE_EXTENSION;
	Font* font = new Font();
	if (!font->Initialize(path, GetTexture(L"fonts/" + name)))
	{
		#ifdef _DEBUG
		MessageBox(NULL, (L"Font not loaded: " + path).c_str(), L"Path Error", MB_OK);
		#endif
		delete font;
		font = nullptr;
		return nullptr;
	}

	m_fontMap[name] = font;
	return font;
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC CLASS API										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
ResourceManager* ResourceManager::Instance()
{
	if (ms_instance == nullptr)
		ms_instance = new ResourceManager();
	return ms_instance;
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	VIRTUAL FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PRIVATE FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
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

void ResourceManager::UnloadAssets()
{
	for (auto& resource : m_modelMap)
	{
		SAFE_DELETE(resource.second);
	}
	m_modelMap.clear();

	for (auto& resource : m_staticTextureMap)
	{
		SAFE_DELETE(resource.second);
	}
	m_staticTextureMap.clear();

	for (auto& resource : m_dynamicTextureMap)
	{
		SAFE_DELETE(resource.second.texture);
	}
	m_dynamicTextureMap.clear();
}

void ResourceManager::ClearCache()
{
	UnloadAssets();

	for (auto& resource : m_fontMap)
	{
		SAFE_DELETE(resource.second);
	}
	m_fontMap.clear();
}

bool ResourceManager::CompareStaticDirectories(std::wstring path)
{
	for (auto dir : m_disabledMipmapDirectories)
	{
		if (path.compare(0, dir.length(), dir) == 0)
			return true;
	}
	return false;
}

