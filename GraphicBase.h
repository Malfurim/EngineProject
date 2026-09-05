#pragma once

#include <DirectXMath.h>
#include "Interactive.h"

class Shader;
struct ID3D11Buffer;

using namespace DirectX;

#define VERTEXSTRUCTSIZE sizeof(SPDNT2Vertex)

typedef XMFLOAT3 TPosition;
typedef XMFLOAT4 TColor;
typedef XMFLOAT3 TNormal;
typedef XMFLOAT3 TDiffuse;
typedef XMFLOAT2 TTextureCoordinate;

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

class GraphicBase : public Interactive
{
private:
	struct SPCVertex
	{
		TPosition position;
		TColor color;
	};

public:
	virtual ~GraphicBase();
	static void Shutdown();

	void SetBackgroundColor(Color color);
	Color GetBackgroundColor();

	bool Visibility{ true };

private:
	bool SetVertexBuffer();
	bool SetIndexBuffer();

protected:
	GraphicBase();
	virtual void Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	virtual void Update();

	bool LoadBuffers();

protected:
	BackgroundType m_background{ BACKGROUND_NONE };
	Color m_backgroundColor{ 0.0f, 0.0f, 0.0f, 0.0f };

	ID3D11Buffer* m_vertexBuffer{ nullptr };
	ID3D11Buffer* m_indexBuffer{ nullptr };
	SPDNT2Vertex* m_vertices{ nullptr };
	unsigned long* m_indices{ nullptr };

	int m_vertexCount{ 0 };
	int m_indexCount{ 0 };
	int m_index{ 0 };

	bool m_dynamic{ false };

private:
	ID3D11Buffer* m_vertexBufferBackground{ nullptr };

	static Shader* ms_shader;
};

