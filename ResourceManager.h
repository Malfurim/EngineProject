#pragma once

// --- INCLUDES ---
#include <string>
#include <unordered_map>
#include "ResourceBase.h"
#include "WindowStyle.h"

// --- CONSTANTS ---
static const std::wstring TEXTURE_FILE_EXTENSION = L".png";
static const std::wstring MESH_FILE_EXTENSION = L".txt";
static const std::wstring MATERIAL_FILE_EXTENSION = L".json";
static const std::wstring FONT_FILE_EXTENSION = L".txt";

static constexpr double UNUSED_TEXTURE_LIFETIME = 300.0;
static constexpr double UNUSED_MESH_LIFETIME = 600.0;
static constexpr double UNUSED_MATERIAL_LIFETIME = 600.0;

// --- MACROS & DEFINES ---
#define RESOURCE ResourceManager::Instance()
#define RESOURCEUPDATE ResourceManager::Instance()->Update()

#define TEXTURERELEASE(t) ResourceManager::Instance()->ReleaseTexture(t)
#define TEXTURE(t) ResourceManager::Instance()->GetTexture(t)
#define TEXTURE2(t, u) ResourceManager::Instance()->GetTexture(t, u)

#define MESHRELEASE(t) ResourceManager::Instance()->ReleaseMesh(t)
#define MESH(t) ResourceManager::Instance()->GetMesh(t)
#define MESH2(t, u) ResourceManager::Instance()->GetMesh(t, u)

#define MESHDXRELEASE(t) ResourceManager::Instance()->ReleaseMeshDX(t)
#define MESHDX(t) ResourceManager::Instance()->GetMeshDX(t)
#define MESHDX2(t, u) ResourceManager::Instance()->GetMeshDX(t, u)

#define MATERIALRELEASE(t) ResourceManager::Instance()->ReleaseMaterial(t)
#define MATERIAL(t) ResourceManager::Instance()->GetMaterial(t)
#define MATERIAL2(t, u) ResourceManager::Instance()->GetMaterial(t, u)

#define FONT(t) ResourceManager::Instance()->GetFont(t)
#define SHADER(t) ResourceManager::Instance()->GetShader(t)
#define WINDOWSTYLE(t) ResourceManager::Instance()->GetWindowStyle(t)

// --- FORWARD DECLARATIONS ---
class Texture;
class Mesh;
class MeshDX;
class Font;
class Shader;
class Material;

/*
#ifdef _DEBUG // MessageBoxW
extern "C" __declspec(dllimport) int __stdcall MessageBoxW(
	void* hWnd,
	const wchar_t* lpText,
	const wchar_t* lpCaption,
	unsigned int uType
);
#endif
*/

// --- ENUMS & STRUCTS ---
enum class AssetPool
{
	ASSET_AUTOMATIC,
	ASSET_STATIC,
	ASSET_DYNAMIC,
};

enum class AssetType
{
	ASSET_TYPE_TEXTURE,
	ASSET_TYPE_MESH,
	ASSET_TYPE_MESHDX,
	ASSET_TYPE_MATERIAL
};

template <typename T>
struct Resource
{
	T* resource{ nullptr };
	unsigned int referenceCount{ 0 };
	double timeSinceUnused{ 0.0 };
};

class ResourceManager
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	~ResourceManager();

	// --- CORE FUNCTIONS ---
	// bool Initialize();
	void Update();
	// void Render();

	// --- VIRTUAL FUNCTIONS ---


	// --- CLASS API ---
	void AddStaticDirectory(const std::wstring& directory);
	void ReleaseTexture(const std::wstring& name);
	void ReleaseMesh(const std::wstring& name);
	void ReleaseMeshDX(const std::wstring& name);
	void ReleaseMaterial(const std::wstring& name);
	void LoadUIShaders();
	void LoadWorldShaders();

	// --- GETTERS & SETTERS ---
	Texture* GetTexture(const std::wstring& name, AssetPool poolMode = AssetPool::ASSET_AUTOMATIC);
	Mesh* GetMesh(const std::wstring& name, AssetPool poolMode = AssetPool::ASSET_AUTOMATIC);
	MeshDX* GetMeshDX(const std::wstring& name, AssetPool poolMode = AssetPool::ASSET_AUTOMATIC);
	Material* GetMaterial(const std::wstring& name, AssetPool poolMode = AssetPool::ASSET_AUTOMATIC);
	Font* GetFont(const std::wstring& name);
	Shader* GetShader(const std::wstring& name, unsigned int layoutFlags = 0);
	const WindowStyle& GetWindowStyle(const std::wstring& name);

	// TEMPORARY FOR DEBUG
	const std::unordered_map<std::wstring, Texture*>& GetStaticTextureMap() const { return m_staticTextureMap; }
	const std::unordered_map<std::wstring, Resource<Texture>>& GetDynamicTextureMap() const { return m_dynamicTextureMap; }
	const std::vector<std::wstring>& GetUnusedTexturesVector() const { return m_unusedTextures; }
	const std::unordered_map<std::wstring, Mesh*>& GetStaticMeshMap() const { return m_staticMeshMap; }
	const std::unordered_map<std::wstring, Resource<Mesh>>& GetDynamicMeshMap() const { return m_dynamicMeshMap; }
	const std::vector<std::wstring>& GetUnusedMeshVector() const { return m_unusedMeshs; }
	const std::unordered_map<std::wstring, MeshDX*>& GetStaticAnimatedMeshMap() const { return m_staticAnimatedMeshMap; }
	const std::unordered_map<std::wstring, Resource<MeshDX>>& GetDynamicAnimatedMeshMap() const { return m_dynamicAnimatedMeshMap; }
	const std::vector<std::wstring>& GetUnusedAnimatedMeshVector() const { return m_unusedAnimatedMeshs; }
	const std::unordered_map<std::wstring, Material*>& GetStaticMaterialMap() const { return m_staticMaterialMap; }
	const std::unordered_map<std::wstring, Resource<Material>>& GetDynamicMaterialMap() const { return m_dynamicMaterialMap; }
	const std::vector<std::wstring>& GetUnusedMaterialVector() const { return m_unusedMaterials; }
	const std::unordered_map<std::wstring, Font*>& GetFontMap() const { return m_fontMap; }
	const std::unordered_map<std::wstring, Shader*>& GetShaderMap() const { return m_shaderMap; }

	// --- STATIC CLASS API ---
	static ResourceManager* Instance();

protected:
	// --- CONSTRUCTORS ---


	// --- VIRTUAL FUNCTIONS ---


	// --- PROTECTED FUNCTIONS ---


private:
	// --- CONSTRUCTORS ---
	ResourceManager();

	// --- PRIVATE FUNCTIONS ---
	template <typename T>
	inline T* GetAsset(const std::wstring& name, AssetPool poolMode, AssetType type, std::unordered_map<std::wstring, T*>& staticMap, std::unordered_map<std::wstring, Resource<T>>& dynamicMap, std::vector<std::wstring>& unusedVector);
	template <typename T>
	inline void ReleaseAsset(const std::wstring& name, std::unordered_map<std::wstring, Resource<T>>& dynamicMap, std::vector<std::wstring>& unusedVector);
	template <typename T>
	inline void UpdateUnusedAsset(const double delta, const double unusedLifetime, std::unordered_map<std::wstring, Resource<T>>& dynamicMap, std::vector<std::wstring>& unusedVector);
	void LoadWindowStyles();
	void UnloadAssets();
	void ClearCache();
	bool CompareStaticDirectories(std::wstring path);

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
	std::unordered_map<std::wstring, Texture*> m_staticTextureMap;
	std::unordered_map<std::wstring, Resource<Texture>> m_dynamicTextureMap;

	std::unordered_map<std::wstring, Mesh*> m_staticMeshMap;
	std::unordered_map<std::wstring, Resource<Mesh>> m_dynamicMeshMap;

	std::unordered_map<std::wstring, MeshDX*> m_staticAnimatedMeshMap;
	std::unordered_map<std::wstring, Resource<MeshDX>> m_dynamicAnimatedMeshMap;

	std::unordered_map<std::wstring, Material*> m_staticMaterialMap;
	std::unordered_map<std::wstring, Resource<Material>> m_dynamicMaterialMap;

	std::unordered_map<std::wstring, Font*> m_fontMap;

	std::unordered_map<std::wstring, Shader*> m_shaderMap;

	std::unordered_map<std::wstring, WindowStyle> m_windowStyleMap;

	std::vector<std::wstring> m_disabledMipmapDirectories;

	std::vector<std::wstring> m_unusedTextures;
	std::vector<std::wstring> m_unusedMeshs;
	std::vector<std::wstring> m_unusedAnimatedMeshs;
	std::vector<std::wstring> m_unusedMaterials;

	WindowStyle m_fallbackWindowStyle;

	// --- STATIC GLOBAL STATES ---
	static ResourceManager* ms_instance;
	static Texture* ms_fallbackTexture;
	static Font* ms_fallbackFont;
};

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	INLINE TEMPLATE FUNCTIONS								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //

template<typename T>
inline T* ResourceManager::GetAsset(const std::wstring& name, AssetPool poolMode, AssetType type, std::unordered_map<std::wstring, T*>& staticMap, std::unordered_map<std::wstring, Resource<T>>& dynamicMap, std::vector<std::wstring>& unusedVector)
{
	if (name == L"")
		return nullptr;

	// Check if template has correct class type that inherits ResourceBase
	static_assert(std::is_base_of<ResourceBase, T>::value, "Template type T must inherit from ResourceBase!");

	// Check if the texture is part of static directories
	bool isStatic = (poolMode == AssetPool::ASSET_AUTOMATIC) ? CompareStaticDirectories(name) : (poolMode == AssetPool::ASSET_STATIC);

	// First try to find if the texture already exists
	if (isStatic)
	{
		auto resource = staticMap.find(name);
		if (resource != staticMap.end())
		{
			return resource->second;
		}
	}
	else
	{
		auto resource = dynamicMap.find(name);
		if (resource != dynamicMap.end())
		{
			resource->second.referenceCount++;
			resource->second.timeSinceUnused = 0.0;

			auto index = std::find(unusedVector.begin(), unusedVector.end(), name);
			if (index != unusedVector.end())
			{
				unusedVector.erase(index);
			}

			return resource->second.resource;
		}
	}

	// Get resource data and file extension based on the template type
	std::wstring fileExtension = L"";
	std::wstring fileDirectory = L"";
	T* fallbackResouce = nullptr;
	switch (type)
	{
	case AssetType::ASSET_TYPE_TEXTURE:
		fileExtension = TEXTURE_FILE_EXTENSION;
		fileDirectory = L"textures/";
		fallbackResouce = reinterpret_cast<T*>(ms_fallbackTexture);
		break;

	case AssetType::ASSET_TYPE_MESH:
		fileExtension = MESH_FILE_EXTENSION;
		fileDirectory = L"meshs/";
		fallbackResouce = nullptr;
		break;

	case AssetType::ASSET_TYPE_MESHDX:
		fileExtension = MESH_FILE_EXTENSION;
		fileDirectory = L"meshs/animated/";
		fallbackResouce = nullptr;
		break;

	case AssetType::ASSET_TYPE_MATERIAL:
		fileExtension = MATERIAL_FILE_EXTENSION;
		fileDirectory = L"materials/";
		fallbackResouce = nullptr;
		break;

		// Unknown typename, return nothing
	default:
		return nullptr;
	}

	// Second if it isn't found, try to create a new one
	std::wstring path = L"assets/" + fileDirectory + name + fileExtension;
	T* resource = new T();
	if (!resource->Initialize(path, name, !isStatic))
	{
		#ifdef _DEBUG
		//MessageBoxW(NULL, (L"Resource not loaded: " + path).c_str(), L"Path Error", MB_OK);
		#endif
		delete resource;
		resource = nullptr;
		return fallbackResouce;
	}

	// Save the texture
	if (isStatic)
	{
		staticMap[name] = resource;
	}
	else
	{
		Resource<T> res;
		res.resource = resource;
		res.referenceCount = 1;
		res.timeSinceUnused = 0.0;
		dynamicMap[name] = res;
	}

	// Return newly created texture
	return resource;
}

template<typename T>
inline void ResourceManager::ReleaseAsset(const std::wstring& name, std::unordered_map<std::wstring, Resource<T>>& dynamicMap, std::vector<std::wstring>& unusedVector)
{
	auto resource = dynamicMap.find(name);
	if (resource == dynamicMap.end())
		return;

	if (resource->second.referenceCount > 0)
	{
		resource->second.referenceCount--;
	}

	if (resource->second.referenceCount == 0)
	{
		resource->second.timeSinceUnused = 0.0;
		if (std::find(unusedVector.begin(), unusedVector.end(), resource->second.resource->GetName()) == unusedVector.end())
		{
			unusedVector.push_back(resource->second.resource->GetName());
		}
	}
}

template<typename T>
inline void ResourceManager::UpdateUnusedAsset(const double delta, const double unusedLifetime, std::unordered_map<std::wstring, Resource<T>>& dynamicMap, std::vector<std::wstring>& unusedVector)
{
	for (int i = unusedVector.size() - 1; i >= 0; i--)
	{
		std::wstring name = unusedVector[i];
		auto resource = dynamicMap.find(name);
		if (resource == dynamicMap.end())
		{
			unusedVector.erase(unusedVector.begin() + i);
			continue;
		}

		resource->second.timeSinceUnused += delta;
		if (resource->second.timeSinceUnused >= unusedLifetime)
		{
			delete resource->second.resource;
			resource->second.resource = nullptr;
			dynamicMap.erase(name);
			unusedVector.erase(unusedVector.begin() + i);
		}
	}
}

