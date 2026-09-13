#pragma once

// --- INCLUDES ---
#include <DirectXMath.h>
#include "Interactive.h"

// --- CONSTANTS ---


// --- MACROS & DEFINES ---
#define VERTEXSTRUCTSIZE sizeof(SPDNT2Vertex)
typedef DirectX::XMFLOAT3 TPosition;
typedef DirectX::XMFLOAT4 TColor;
typedef DirectX::XMFLOAT3 TNormal;
typedef DirectX::XMFLOAT3 TDiffuse;
typedef DirectX::XMFLOAT2 TTextureCoordinate;

// --- ENUMS & STRUCTS ---
enum BackgroundType
{
	BACKGROUND_NONE,
	BACKGROUND_SOLID,
	BACKGROUND_TRANSPARENT
};

struct Color
{
	float r;
	float g;
	float b;
	float a;

	bool operator==(const Color& other) const
	{
		return (r == other.r && g == other.g && b == other.b && a == other.a);
	}

	bool operator!=(const Color& other) const
	{
		return !(*this == other);
	}
};

struct SPDNT2Vertex
{
	TPosition position{ 0.0f, 0.0f, 0.0f };
	TDiffuse diffuse{ 0.0f, 0.0f, 0.0f };
	TNormal normal{ 0.0f, 0.0f, 0.0f };
	TTextureCoordinate texCoord{ 0.0f, 0.0f };
	TTextureCoordinate texCoord2{ 0.0f, 0.0f };
};

// --- FORWARD DECLARATIONS ---
class Shader;
struct ID3D11Buffer;
using namespace DirectX;

class GraphicBase : public Interactive
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	virtual ~GraphicBase();

	// --- CORE FUNCTIONS ---
	// bool Initialize();
	virtual void Update();
	virtual void Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	static void Shutdown();

	// --- VIRTUAL FUNCTIONS ---


	// --- CLASS API ---


	// --- GETTERS & SETTERS ---
	Color GetBackgroundColor() const { return m_backgroundColor; }
	void SetBackgroundColor(Color color);

	// --- STATIC CLASS API ---
	

protected:
	// --- CONSTRUCTORS ---
	GraphicBase();

	// --- VIRTUAL FUNCTIONS ---


	// --- PROTECTED FUNCTIONS ---
	bool LoadBuffers();

private:
	// --- CONSTRUCTORS ---


	// --- PRIVATE FUNCTIONS ---
	bool SetVertexBuffer();
	bool SetIndexBuffer();

// - PROPERTIES -
public:
	// --- PUBLIC COMPONENT STATES ---
	bool Visibility{ true };

	// --- PUBLIC COMPONENT DATA ---


protected:
	// --- INTERNAL CONSTANTS ---


	// --- INTERNAL STRUCTS & ENUMS ---


	// --- INTERNAL COMPONENT STATES ---
	bool m_dynamic{ false };

	// --- INTERNAL COMPONENT DATA ---
	BackgroundType m_background{ BACKGROUND_NONE };
	Color m_backgroundColor{ 0.0f, 0.0f, 0.0f, 0.0f };

	ID3D11Buffer* m_vertexBuffer{ nullptr };
	ID3D11Buffer* m_indexBuffer{ nullptr };
	SPDNT2Vertex* m_vertices{ nullptr };
	unsigned long* m_indices{ nullptr };

	int m_vertexCount{ 0 };
	int m_indexCount{ 0 };
	unsigned long m_vertexCapacity{ 0 };
	unsigned long m_indexCapacity{ 0 };

private:
	// --- PRIVATE CONSTANTS ---


	// --- PRIVATE STRUCTS & ENUMS ---
	struct SPCVertex
	{
		TPosition position;
		TColor color;
	};

	// --- PRIVATE COMPONENT STATES ---


	// --- PRIVATE COMPONENT DATA ---
	ID3D11Buffer* m_vertexBufferBackground{ nullptr };

	// --- STATIC GLOBAL STATES ---
	static Shader* ms_shader;
};

