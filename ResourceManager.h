#pragma once

#include "Texture.h"
#include "Model.h"
#include "Font.h"
#include <vector>
#include <string>
#include <unordered_map>


#define RESOURCE ResourceManager::Instance()

class ResourceManager
{
public:
	~ResourceManager();

	// Loads base directory called assets into the program
	void LoadAssets(const std::wstring& directoryPath = L"assets");

	Texture* GetTexture(std::wstring name);
	Model* GetModel(std::wstring name);
	Font* GetFont(std::wstring name);

	static ResourceManager* Instance();

private:
	ResourceManager();

	void ScanDirectory(const std::wstring& directoryPath, void (ResourceManager::*loadFunc)(const std::wstring&));

	void LoadTexture(const std::wstring& path);
	void LoadUITexture(const std::wstring& path);
	void LoadModel(const std::wstring& path);
	void LoadFont(const std::wstring& path);

	std::unordered_map<std::wstring, Texture*> m_textureMap;
	std::unordered_map<std::wstring, Model*> m_modelMap;
	std::unordered_map<std::wstring, Font*> m_fontMap;

	static ResourceManager* ms_instance;
};

