#pragma once

// --- INCLUDES ---
#include <string>
#include <unordered_map>

// --- CONSTANTS ---


// --- MACROS & DEFINES ---
#define RESOURCE ResourceManager::Instance()

// --- ENUMS & STRUCTS ---


// --- FORWARD DECLARATIONS ---
class Texture;
class Model;
class Font;

class ResourceManager
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	~ResourceManager();

	// --- CORE FUNCTIONS ---
	// bool Initialize();
	// void Update();
	// void Render();

	// --- VIRTUAL FUNCTIONS ---


	// --- CLASS API ---
	void LoadAssets(const std::wstring& directoryPath = L"assets");		// Loads base directory called assets into the program

	// --- GETTERS & SETTERS ---
	Texture* GetTexture(std::wstring name);
	Model* GetModel(std::wstring name);
	Font* GetFont(std::wstring name);

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
	void ScanDirectory(const std::wstring& directoryPath, void (ResourceManager::* loadFunc)(const std::wstring&));
	void LoadTexture(const std::wstring& path);
	void LoadUITexture(const std::wstring& path);
	void LoadModel(const std::wstring& path);
	void LoadFont(const std::wstring& path);

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
	std::unordered_map<std::wstring, Texture*> m_textureMap;
	std::unordered_map<std::wstring, Model*> m_modelMap;
	std::unordered_map<std::wstring, Font*> m_fontMap;

	// --- STATIC GLOBAL STATES ---
	static ResourceManager* ms_instance;
};

