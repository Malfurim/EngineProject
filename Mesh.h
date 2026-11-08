#pragma once

// --- INCLUDES ---
#include "ResourceBase.h"
#include <d3d11.h>
#include "VertexStructs.h"

// --- CONSTANTS ---


// --- MACROS & DEFINES ---


// --- ENUMS & STRUCTS ---


// --- FORWARD DECLARATIONS ---


class Mesh : public ResourceBase
{
	// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	Mesh();
	~Mesh() override;

	// --- CORE FUNCTIONS ---
	bool Initialize(const std::wstring& filePath, const std::wstring& name, bool isStatic);
	// void Update();
	// void Render();

	// --- PUBLIC VIRTUAL FUNCTIONS ---


	// --- CLASS API ---


	// --- GETTERS & SETTERS ---
	ID3D11Buffer* GetMeshVertexBuffer() const { return m_vertexBuffer; }
	ID3D11Buffer* GetMeshIndexBuffer() const {	return m_indexBuffer; }
	int GetIndexCount() const { return m_indexCount; }
	int GetVertexCount() const { return m_vertexCount; }
	DXFLOAT3 GetPivotPoint() const { return m_pivotPoint; }

	// --- STATIC CLASS API ---


protected:
	// --- CONSTRUCTORS ---


	// --- PROTECTED VIRTUAL FUNCTIONS ---


	// --- PROTECTED FUNCTIONS ---


private:
	// --- CONSTRUCTORS ---


	// --- PRIVATE FUNCTIONS ---


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
	ID3D11Buffer* m_vertexBuffer{ nullptr };
	ID3D11Buffer* m_indexBuffer{ nullptr };
	int m_vertexCount{ 0 };
	int m_indexCount{ 0 };
	DXFLOAT3 m_pivotPoint{ 0.0f, 0.0f, 0.0f };

	// --- STATIC GLOBAL STATES ---

};