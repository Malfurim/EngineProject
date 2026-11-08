#include "Parser.h"

// --- ADDITIONAL INCLUDES ---
#ifdef _DEBUG
#include <Windows.h>
#endif

#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>
#include "VertexStructs.h"
#include "Deleters.h"

// --- MACROS & DEFINES ---


// --- FORWARD DECLARATIONS ---


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC GLOBAL STATES & DATA								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CONSTRUCTORS & DESTRUCTOR								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CORE FUNCTIONS											//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//bool $itemname$::Initialize()
//{
//	return true;
//}

//void $itemname$::Update()
//{
//}

//void $itemname$::Render()
//{
//}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PUBLIC VIRTUAL FUNCTIONS								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CLASS API												//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	GETTERS & SETTERS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC CLASS API										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
bool Parser::LoadMeshFromFile(const std::wstring& filePath, MeshData& outData)
{
	std::wifstream stream;
	stream.open(filePath.c_str());
	if (stream.fail())
	{
		return false;
	}

	float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
	float maxX = FLT_MIN, maxY = FLT_MIN, maxZ = FLT_MIN;

	DXFLOAT3 position;
	std::vector<Vector3> positions;
	std::vector<Vector2> textureCoords;
	std::map<VertexKey, unsigned int> uniqueVertices;

	std::wstring line;
	while (std::getline(stream, line))
	{
		if (line.length() == 0)
			continue;

		std::wstringstream lineStream(line.substr(2));
		switch (line[0])
		{
		case L'v':
			position = ParseVertex(lineStream, positions);
			minX = min(minX, position.x);
			minY = min(minY, position.y);
			minZ = min(minZ, position.z);
			maxX = max(maxX, position.x);
			maxY = max(maxY, position.y);
			maxZ = max(maxZ, position.z);
			break;

		case L't':
			ParseTextureCoord(lineStream, textureCoords);
			break;

		case L'f':
			ParseFace(lineStream, positions, textureCoords, uniqueVertices, outData);
			break;

		default:
			break;
		}
	}

	outData.pivotPoint = { (minX + maxX) * 0.5f, (minY + maxY) * 0.5f, (minZ + maxZ) * 0.5f };

	positions.clear();
	textureCoords.clear();
	stream.close();

	return true;
}

bool Parser::LoadJsonFile(const std::wstring& filePath, nlohmann::json& outData)
{
	std::ifstream file;
	if (filePath.length() >= 5 && filePath.substr(filePath.length() - 5) == L".json")
		file.open(filePath);
	else
		file.open(filePath + L".json");

	if (!file.is_open())
	{
		#ifdef _DEBUG
		std::wstring errorMsg = L"File " + filePath + L" doesn't exist.";
		MessageBox(NULL, errorMsg.c_str(), L"ERROR", MB_OK);
		#endif
		return false;
	}

	try
	{
		file >> outData;	// Load method from nlohmann library
	}
	catch (const nlohmann::json::parse_error& e)
	{
		#ifdef _DEBUG
		std::string errStr = "Syntax error in JSON: " + std::string(e.what());
		MessageBoxA(NULL, errStr.c_str(), "JSON Parse Error", MB_OK);
		#endif
		file.close();
		return false;
	}
	file.close();
	return true;
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED VIRTUAL FUNCTIONS								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PRIVATE FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
DXFLOAT3 Parser::ParseVertex(std::wistream& stream, std::vector<Vector3>& outPositionsVector)
{
	float x, y, z;
	if (stream >> x >> y >> z)
	{
		outPositionsVector.push_back(Vector3{ x, y, z });
		return { x, y, z };
	}
}

void Parser::ParseTextureCoord(std::wistream& stream, std::vector<Vector2>& outTextureCoords)
{
	float x, y;
	if (stream >> x >> y)
	{
		outTextureCoords.push_back(Vector2{ x, y });
	}
}

void Parser::ParseFace(std::wistream& stream, const std::vector<Vector3>& positions, const std::vector<Vector2>& textureCoords, std::map<VertexKey, unsigned int>& uniqueVertices, MeshData& outData)
{
	unsigned int position[3];
	unsigned int textureCoord[3];
	wchar_t slash;
	float normalX, normalY, normalZ;

	for (int i = 0; i < 3; i++)
	{
		if (!(stream >> position[i] >> slash >> textureCoord[i]))
		{
			return;
		}
	}

	if (!(stream >>  normalX >> normalY >> normalZ))
	{
		return;
	}
		
	for (int i = 0; i < 3; i++)
	{
		VertexKey key = std::make_tuple(position[i], textureCoord[i], normalX, normalY, normalZ);
		auto vertexMapped = uniqueVertices.find(key);
		if (vertexMapped == uniqueVertices.end())
		{
			// Vertex not found, first appearance, create vertex data and assign index
			Vector3 pos = positions[position[i] - 1];
			Vector2 tex = textureCoords[textureCoord[i] - 1];
			SPDNTVertex vertex;
			vertex.position = TPosition(pos.X, pos.Y, pos.Z);
			vertex.texCoord = TTextureCoordinate(tex.X, tex.Y);
			vertex.normal = TNormal(normalX, normalY, normalZ);
			vertex.diffuse = TDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
			
			unsigned int index = static_cast<unsigned int>(outData.vertices.size());

			outData.vertices.push_back(vertex);
			outData.indices.push_back(index);
			uniqueVertices[key] = index;
		}
		else
		{
			// Vertex found, load saved vertex's index into indices
			unsigned int index = vertexMapped->second;
			outData.indices.push_back(index);
		}
	}
}
