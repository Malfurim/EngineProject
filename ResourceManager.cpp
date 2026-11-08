#include "ResourceManager.h"

// --- ADDITIONAL INCLUDES ---
#ifdef _DEBUG
#include <Windows.h>
#endif

#include <nlohmann/json.hpp>
#include <fstream>
#include "DirectXManager.h"
#include "TimerManager.h"
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"
#include "Font.h"
#include "Shader.h"
#include "Parser.h"
#include "Utils.h"
#include "Deleters.h"

#include "MeshDX.h"

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
	UpdateUnusedAsset(delta, UNUSED_MESH_LIFETIME, m_dynamicMeshMap, m_unusedMeshs);
	UpdateUnusedAsset(delta, UNUSED_MESH_LIFETIME, m_dynamicAnimatedMeshMap, m_unusedAnimatedMeshs);
	UpdateUnusedAsset(delta, UNUSED_MATERIAL_LIFETIME, m_dynamicMaterialMap, m_unusedMaterials);
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

void ResourceManager::ReleaseMesh(const std::wstring& name)
{
	ReleaseAsset(name, m_dynamicMeshMap, m_unusedMeshs);
}

void ResourceManager::ReleaseMeshDX(const std::wstring& name)
{
	ReleaseAsset(name, m_dynamicAnimatedMeshMap, m_unusedAnimatedMeshs);
}

void ResourceManager::ReleaseMaterial(const std::wstring& name)
{
	ReleaseAsset(name, m_dynamicMaterialMap, m_unusedMaterials);
}

void ResourceManager::LoadUIShaders()
{
	GetShader(L"color", SPCVertex::VertexLayout);
	GetShader(L"texture", SPTVertex::VertexLayout);
	GetShader(L"font", SPDTVertex::VertexLayout);
}

void ResourceManager::LoadWorldShaders()
{
	GetShader(L"basic3Dlight", SPDNTVertex::VertexLayout);
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	GETTERS & SETTERS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
Texture* ResourceManager::GetTexture(const std::wstring& name, AssetPool poolMode)
{
	return GetAsset(name, poolMode, AssetType::ASSET_TYPE_TEXTURE, m_staticTextureMap, m_dynamicTextureMap, m_unusedTextures);
}

Mesh* ResourceManager::GetMesh(const std::wstring& name, AssetPool poolMode)
{
	return GetAsset(name, poolMode, AssetType::ASSET_TYPE_MESH, m_staticMeshMap, m_dynamicMeshMap, m_unusedMeshs);
}

MeshDX* ResourceManager::GetMeshDX(const std::wstring& name, AssetPool poolMode)
{
	return GetAsset(name, poolMode, AssetType::ASSET_TYPE_MESHDX, m_staticAnimatedMeshMap, m_dynamicAnimatedMeshMap, m_unusedAnimatedMeshs);
}

Material* ResourceManager::GetMaterial(const std::wstring& name, AssetPool poolMode)
{
	return GetAsset(name, poolMode, AssetType::ASSET_TYPE_MATERIAL, m_staticMaterialMap, m_dynamicMaterialMap, m_unusedMaterials);
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

Shader* ResourceManager::GetShader(const std::wstring& name, unsigned int layoutFlags)
{
	if (name == L"")
		return nullptr;

	auto resource = m_shaderMap.find(name);
	if (resource != m_shaderMap.end())
	{
		return resource->second;
	}

	if (layoutFlags == 0)
		return nullptr;

	Shader* shader = new Shader();
	if (!shader->Initialize(name, m_shaderMap.size() + 1, layoutFlags))
	{
		#ifdef _DEBUG
		MessageBox(NULL, (L"Shader not loaded: " + name).c_str(), L"Path Error", MB_OK);
		#endif
		delete shader;
		shader = nullptr;
		return nullptr;
	}

	m_shaderMap[name] = shader;
	return shader;
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
	nlohmann::json data;
	if (!Parser::LoadJsonFile(L"assets/styles.json", data))
	{
		return;
	}

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
	for (auto& resource : m_staticMeshMap)
	{
		SAFE_DELETE(resource.second);
	}
	m_staticMeshMap.clear();

	for (auto& resource : m_dynamicMeshMap)
	{
		SAFE_DELETE(resource.second.resource);
	}
	m_dynamicMeshMap.clear();

	for (auto& resource : m_staticAnimatedMeshMap)
	{
		SAFE_DELETE(resource.second);
	}
	m_staticAnimatedMeshMap.clear();

	for (auto& resource : m_dynamicAnimatedMeshMap)
	{
		SAFE_DELETE(resource.second.resource);
	}
	m_dynamicAnimatedMeshMap.clear();

	for (auto& resource : m_staticMaterialMap)
	{
		SAFE_DELETE(resource.second);
	}
	m_staticMaterialMap.clear();

	for (auto& resource : m_dynamicMaterialMap)
	{
		SAFE_DELETE(resource.second.resource);
	}
	m_dynamicMaterialMap.clear();

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

	for (auto& resource : m_shaderMap)
	{
		SAFE_DELETE(resource.second);
	}
	m_shaderMap.clear();
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

