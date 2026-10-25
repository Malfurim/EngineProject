#include "ResourceManager.h"

// --- ADDITIONAL INCLUDES ---
#include <nlohmann/json.hpp>
#include <fstream>
#include "Window.h"
#include "DirectXManager.h"
#include "TimerManager.h"
#include "Texture.h"
#include "Model.h"
#include "Font.h"
#include "Utils.h"
#include "Deleters.h"

#include "ModelDX.h"

// --- MACROS & DEFINES ---


// --- FORWARD DECLARATIONS ---


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC GLOBAL STATES & DATA								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
ResourceManager* ResourceManager::ms_instance = nullptr;
Texture* ResourceManager::ms_fallbackTexture = nullptr;
Font* ResourceManager::ms_fallbackFont = nullptr;

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

	ms_fallbackFont = new Font();
	if (!ms_fallbackFont->Initialize(L"assets/fonts/default.txt", GetTexture(L"fonts/default")))
	{
		#ifdef _DEBUG
		MessageBox(NULL, L"File assets/fonts/default.txt doesn't exist", L"ERROR", MB_OK);
		#endif
	}

	LoadWindowStyles();
}

ResourceManager::~ResourceManager()
{
	ClearCache();
	ms_instance = nullptr;
	delete ms_fallbackTexture;
	ms_fallbackTexture = nullptr;
	delete ms_fallbackFont;
	ms_fallbackFont = nullptr;
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
	
	UpdateUnusedAsset(delta, UNUSED_TEXTURE_LIFETIME, m_dynamicTextureMap, m_unusedTextures);
	UpdateUnusedAsset(delta, UNUSED_MODEL_LIFETIME, m_dynamicModelMap, m_unusedModels);
	UpdateUnusedAsset(delta, UNUSED_MODEL_LIFETIME, m_dynamicAnimatedModelMap, m_unusedAnimatedModels);
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
	for (auto& dir : m_disabledMipmapDirectories)
	{
		if (directory.compare(0, directory.length(), dir) == 0)
			return;
	}
	m_disabledMipmapDirectories.push_back(directory);
}

void ResourceManager::ReleaseTexture(const std::wstring& name)
{
	ReleaseAsset(name, m_dynamicTextureMap, m_unusedTextures);
}

void ResourceManager::ReleaseModel(const std::wstring& name)
{
	ReleaseAsset(name, m_dynamicModelMap, m_unusedModels);
}

void ResourceManager::ReleaseModelDX(const std::wstring& name)
{
	ReleaseAsset(name, m_dynamicAnimatedModelMap, m_unusedAnimatedModels);
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	GETTERS & SETTERS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
Texture* ResourceManager::GetTexture(const std::wstring& name, AssetPool poolMode)
{
	return GetAsset(name, poolMode, AssetType::ASSET_TYPE_TEXTURE, m_staticTextureMap, m_dynamicTextureMap, m_unusedTextures);
}

Model* ResourceManager::GetModel(const std::wstring& name, AssetPool poolMode)
{
	return GetAsset(name, poolMode, AssetType::ASSET_TYPE_MODEL, m_staticModelMap, m_dynamicModelMap, m_unusedModels);
}

ModelDX* ResourceManager::GetModelDX(const std::wstring& name, AssetPool poolMode)
{
	return GetAsset(name, poolMode, AssetType::ASSET_TYPE_MODELDX, m_staticAnimatedModelMap, m_dynamicAnimatedModelMap, m_unusedAnimatedModels);
}

Font* ResourceManager::GetFont(const std::wstring& name)
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
		return ms_fallbackFont;
	}

	m_fontMap[name] = font;
	return font;
}

const WindowStyle& ResourceManager::GetWindowStyle(const std::wstring& name)
{
	auto resource = m_windowStyleMap.find(name);
	if (resource != m_windowStyleMap.end())
	{
		return resource->second;
	}

	return m_fallbackWindowStyle;
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
void ResourceManager::LoadWindowStyles()
{
	std::ifstream file("assets/styles.json");
	if (!file.is_open())
	{
		#ifdef _DEBUG
		MessageBox(NULL, L"File assets/styles.json doesn't exist", L"ERROR", MB_OK);
		#endif
		return;
	}

	nlohmann::json data;
	try
	{
		file >> data;	// Load method from nlohmann library
	}
	catch (const nlohmann::json::parse_error& e)
	{
		#ifdef _DEBUG
		std::string errStr = "Syntax error in JSON: " + std::string(e.what());
		MessageBoxA(NULL, errStr.c_str(), "JSON Parse Error", MB_OK);
		#endif
		file.close();
		return;
	}
	file.close();

	for (auto resource = data.begin(); resource != data.end(); resource++)
	{
		std::string name = resource.key();
		auto& styleData = resource.value();

		WindowStyle style;
		style.borderSize		= styleData.value("BorderSize", 1.0f);
		style.titleBarHeight	= styleData.value("TitleBarHeight", 24.0f);
		style.closeButtonSize	= styleData.value("CloseButtonSize", 20.0f);

		std::string fontStr = styleData.value("FontName", "default");
		style.titleBarFont = std::wstring(fontStr.begin(), fontStr.end());

		if (styleData.contains("BackgroundTexture"))
		{
			std::string textureStr = styleData["BackgroundTexture"];
			style.backgroundTexture = std::wstring(textureStr.begin(), textureStr.end());
		}

		if (styleData.contains("TitleBarTexture"))
		{
			std::string textureStr = styleData["TitleBarTexture"];
			style.titleBarTexture = std::wstring(textureStr.begin(), textureStr.end());
		}

		if (styleData.contains("CloseButtonTexture"))
		{
			std::string textureStr = styleData["CloseButtonTexture"];
			style.closeButtonTexture = std::wstring(textureStr.begin(), textureStr.end());
		}

		if (styleData.contains("BackgroundColor") && styleData["BackgroundColor"].is_array())
		{
			auto& color = styleData["BackgroundColor"];
			style.backgroundColor = Color{color[0], color[1], color[2], color[3]};
		}

		if (styleData.contains("TitleBarColor") && styleData["TitleBarColor"].is_array())
		{
			auto& color = styleData["TitleBarColor"];
			style.titleBarColor = Color{ color[0], color[1], color[2], color[3] };
		}

		if (styleData.contains("BorderColor") && styleData["BorderColor"].is_array())
		{
			auto& color = styleData["BorderColor"];
			style.borderColor = Color{ color[0], color[1], color[2], color[3] };
		}

		std::wstring wStyleName = std::wstring(name.begin(), name.end());
		m_windowStyleMap[wStyleName] = style;
	}
}

void ResourceManager::UnloadAssets()
{
	for (auto& resource : m_staticModelMap)
	{
		SAFE_DELETE(resource.second);
	}
	m_staticModelMap.clear();

	for (auto& resource : m_dynamicModelMap)
	{
		SAFE_DELETE(resource.second.resource);
	}
	m_dynamicModelMap.clear();

	for (auto& resource : m_staticAnimatedModelMap)
	{
		SAFE_DELETE(resource.second);
	}
	m_staticAnimatedModelMap.clear();

	for (auto& resource : m_dynamicAnimatedModelMap)
	{
		SAFE_DELETE(resource.second.resource);
	}
	m_dynamicAnimatedModelMap.clear();

	for (auto& resource : m_staticTextureMap)
	{
		SAFE_DELETE(resource.second);
	}
	m_staticTextureMap.clear();

	for (auto& resource : m_dynamicTextureMap)
	{
		SAFE_DELETE(resource.second.resource);
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
	for (auto& dir : m_disabledMipmapDirectories)
	{
		if (path.compare(0, dir.length(), dir) == 0)
			return true;
	}
	return false;
}

