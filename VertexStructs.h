#pragma once

#include "DXMath.h"

typedef DXFLOAT3 TPosition;
typedef DXFLOAT4 TColor;
typedef DXFLOAT3 TNormal;
typedef DXFLOAT4 TDiffuse;
typedef DXFLOAT2 TTextureCoordinate;

enum VertexFlag : unsigned int
{
	VERTEX_POSITION = 1 << 0,
	VERTEX_COLOR = 1 << 1,
	VERTEX_DIFFUSE = 1 << 2,
	VERTEX_NORMAL = 1 << 3,
	VERTEX_TEXCOORD = 1 << 4,
	VERTEX_TEXCOORD2 = 1 << 5,
};

struct SPVertex
{
	TPosition position{ 0.0f, 0.0f, 0.0f };

	static const unsigned int VertexLayout =
		VertexFlag::VERTEX_POSITION;
};

struct SPCVertex
{
	TPosition position{ 0.0f, 0.0f, 0.0f };
	TColor color{ 0.0f, 0.0f, 0.0f, 0.0f };

	static const unsigned int VertexLayout =
		VertexFlag::VERTEX_POSITION |
		VertexFlag::VERTEX_COLOR;
};

struct SPTVertex
{
	TPosition position{ 0.0f, 0.0f, 0.0f };
	TTextureCoordinate texCoord{ 0.0f, 0.0f };

	static const unsigned int VertexLayout =
		VertexFlag::VERTEX_POSITION |
		VertexFlag::VERTEX_TEXCOORD;
};

struct SPDTVertex
{
	TPosition position{ 0.0f, 0.0f, 0.0f };
	TDiffuse diffuse{ 1.0f, 1.0f, 1.0f, 1.0f };
	TTextureCoordinate texCoord{ 0.0f, 0.0f };

	static const unsigned int VertexLayout =
		VertexFlag::VERTEX_POSITION |
		VertexFlag::VERTEX_DIFFUSE |
		VertexFlag::VERTEX_TEXCOORD;
};

struct SPDNTVertex
{
	TPosition position{ 0.0f, 0.0f, 0.0f };
	TDiffuse diffuse{ 1.0f, 1.0f, 1.0f, 1.0f };
	TNormal normal{ 0.0f, 0.0f, 0.0f };
	TTextureCoordinate texCoord{ 0.0f, 0.0f };

	static const unsigned int VertexLayout =
		VertexFlag::VERTEX_POSITION |
		VertexFlag::VERTEX_DIFFUSE |
		VertexFlag::VERTEX_NORMAL |
		VertexFlag::VERTEX_TEXCOORD;
};

struct SPDNT2Vertex
{
	TPosition position{ 0.0f, 0.0f, 0.0f };
	TDiffuse diffuse{ 1.0f, 1.0f, 1.0f, 1.0f };
	TNormal normal{ 0.0f, 0.0f, 0.0f };
	TTextureCoordinate texCoord{ 0.0f, 0.0f };
	TTextureCoordinate texCoord2{ 0.0f, 0.0f };

	static const unsigned int VertexLayout =
		VertexFlag::VERTEX_POSITION |
		VertexFlag::VERTEX_DIFFUSE |
		VertexFlag::VERTEX_NORMAL |
		VertexFlag::VERTEX_TEXCOORD |
		VertexFlag::VERTEX_TEXCOORD2;
};