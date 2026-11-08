#pragma once

// --- INCLUDES ---
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <nlohmann/json_fwd.hpp>
#include "Vectors.h"
#include "VertexStructs.h"

// --- CONSTANTS ---


// --- MACROS & DEFINES ---


// --- ENUMS & STRUCTS ---
struct MeshData
{
	std::vector<SPDNTVertex> vertices;
	std::vector<unsigned int> indices;
	DXFLOAT3 pivotPoint;
};

// --- FORWARD DECLARATIONS ---
using VertexKey = std::tuple<int, int, float, float, float>;

class Parser
{
	// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	

	// --- CORE FUNCTIONS ---
	// bool Initialize();
	// void Update();
	// void Render();

	// --- PUBLIC VIRTUAL FUNCTIONS ---


	// --- CLASS API ---


	// --- GETTERS & SETTERS ---


	// --- STATIC CLASS API ---
	static bool LoadMeshFromFile(const std::wstring& filePath, MeshData& outData);
	static bool LoadJsonFile(const std::wstring& filePath, nlohmann::json& outData);

protected:
	// --- CONSTRUCTORS ---


	// --- PROTECTED VIRTUAL FUNCTIONS ---


	// --- PROTECTED FUNCTIONS ---


private:
	// --- CONSTRUCTORS ---
	Parser() = delete;
	~Parser() = delete;

	// --- PRIVATE FUNCTIONS ---
	static DXFLOAT3 ParseVertex(std::wistream& stream, std::vector<Vector3>& outPositions);
	static void ParseTextureCoord(std::wistream& stream, std::vector<Vector2>& outTextureCoords);
	static void ParseFace(std::wistream& stream, const std::vector<Vector3>& positions, const std::vector<Vector2>& textureCoords, std::map<VertexKey, unsigned int>& uniqueVertices, MeshData& outData);

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


	// --- STATIC GLOBAL STATES ---

};