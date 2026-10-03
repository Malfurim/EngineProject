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
	BACKGROUND_COLOR,
	BACKGROUND_BORDER,
	BACKGROUND_BORDER_ONLY,
};

enum UpdateFlag : unsigned int
{
	UPDATE_NONE					= 0,
	UPDATE_GEOMETRY				= 1 << 0,
	UPDATE_COLORS				= 1 << 1,
	UPDATE_TEXT					= 1 << 2,
	UPDATE_POSITION				= 1 << 3,
	UPDATE_BACKGROUND_STATE		= 1 << 4,

	UPDATE_ALL = UPDATE_GEOMETRY | UPDATE_COLORS | UPDATE_TEXT | UPDATE_POSITION | UPDATE_BACKGROUND_STATE
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

struct SPVertex
{
	TPosition position{ 0.0f, 0.0f, 0.0f };
};

struct SPCVertex
{
	TPosition position{ 0.0f, 0.0f, 0.0f };
	TColor color{ 0.0f, 0.0f, 0.0f, 0.0f };
};

struct SPTVertex
{
	TPosition position{ 0.0f, 0.0f, 0.0f };
	TTextureCoordinate texCoord{ 0.0f, 0.0f };
};

struct SPDTVertex
{
	TPosition position{ 0.0f, 0.0f, 0.0f };
	TDiffuse diffuse{ 0.0f, 0.0f, 0.0f };
	TTextureCoordinate texCoord{ 0.0f, 0.0f };
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
	void Invalidate() { m_updateFlags = UpdateFlag::UPDATE_ALL; }
	void Invalidate(UpdateFlag flag) { m_updateFlags |= flag; }
	bool IsInvalidated() const { return m_updateFlags != UPDATE_NONE; }
	bool IsInvalidated(UpdateFlag flag) const { return (m_updateFlags & flag) != 0; }

	// --- GETTERS & SETTERS ---
	Color GetBackgroundColor() const { return m_backgroundColor; }
	void SetBackgroundColor(Color color);

	// --- STATIC CLASS API ---
	

protected:
	// --- CONSTRUCTORS ---
	GraphicBase();

	// --- VIRTUAL FUNCTIONS ---


	// --- PROTECTED FUNCTIONS ---
	void Validate(UpdateFlag flag) { m_updateFlags &= ~flag; }
	void UpdateGeometry();
	void UpdateColors(Color borderColor = { 0.0f, 0.0f, 0.0f, 0.0 });
	bool LoadBuffers();
	bool SetVertexBufferBackground();
	bool LoadBackgroundBuffer();
	bool UpdateBufferData(ID3D11Buffer* targetBuffer, void* sourceData, unsigned int byteSize);

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
	unsigned int m_updateFlags{ UpdateFlag::UPDATE_NONE };

	// --- INTERNAL COMPONENT DATA ---
	BackgroundType m_background{ BACKGROUND_NONE };
	Color m_backgroundColor{ 0.0f, 0.0f, 0.0f, 0.0f };

	ID3D11Buffer* m_vertexBuffer{ nullptr };
	ID3D11Buffer* m_vertexBufferBackground{ nullptr };
	ID3D11Buffer* m_indexBuffer{ nullptr };
	void* m_vertices{ nullptr };
	unsigned int m_vertexSize{ 0 };
	SPCVertex* m_verticesBackground{ nullptr };
	unsigned int* m_indices{ nullptr };

	unsigned int m_vertexCount{ 0 };
	unsigned int m_indexCount{ 0 };
	unsigned int m_vertexCapacity{ 0 };
	unsigned int m_indexCapacity{ 0 };

private:
	// --- PRIVATE CONSTANTS ---


	// --- PRIVATE STRUCTS & ENUMS ---
	

	// --- PRIVATE COMPONENT STATES ---


	// --- PRIVATE COMPONENT DATA ---
	

	// --- STATIC GLOBAL STATES ---
	static Shader* ms_shader;
};

