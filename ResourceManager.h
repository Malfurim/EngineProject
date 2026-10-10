#pragma once

// --- INCLUDES ---
#include <string>
#include <unordered_map>

// --- CONSTANTS ---
static const std::wstring TEXTURE_FILE_EXTENSION = L".png";
static const std::wstring FONT_FILE_EXTENSION = L".txt";

static constexpr double UNUSED_TEXTURE_LIFETIME = 300.0;

// --- MACROS & DEFINES ---
#define RESOURCE ResourceManager::Instance()
#define RESOURCEUPDATE ResourceManager::Instance()->Update()
#define TEXTURERELEASE(t) ResourceManager::Instance()->ReleaseTexture(t)

// --- FORWARD DECLARATIONS ---
class Texture;
class Model;
class Font;

// --- ENUMS & STRUCTS ---
enum AssetPool
{
	ASSET_AUTOMATIC,
	ASSET_STATIC,
	ASSET_DYNAMIC,
};

struct TextureResource
{
	Texture* texture{ nullptr };
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

	// --- GETTERS & SETTERS ---
	Texture* GetTexture(const std::wstring& name, AssetPool poolMode = AssetPool::ASSET_AUTOMATIC);
	Model* GetModel(std::wstring name);
	Font* GetFont(std::wstring name);

	// TEMPORARY FOR DEBUG
	const std::unordered_map<std::wstring, Texture*>& GetStaticTextureMap() const { return m_staticTextureMap; }
	const std::unordered_map<std::wstring, TextureResource>& GetDynamicTextureMap() const { return m_dynamicTextureMap; }
	const std::vector<std::wstring>& GetUnusedTexturesVector() const { return m_unusedTextures; }

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
	void LoadModel(const std::wstring& path);

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
	std::unordered_map<std::wstring, Model*> m_modelMap;
	std::unordered_map<std::wstring, Font*> m_fontMap;

	std::unordered_map<std::wstring, Texture*> m_staticTextureMap;
	std::unordered_map<std::wstring, TextureResource> m_dynamicTextureMap;

	std::vector<std::wstring> m_disabledMipmapDirectories;
	std::vector<std::wstring> m_unusedTextures;

	// --- STATIC GLOBAL STATES ---
	static ResourceManager* ms_instance;
	static Texture* ms_fallbackTexture;
};

