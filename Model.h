#pragma once

#include <d3d11.h>
#include "DXMath.h"
#include <string>
#include "ResourceBase.h"

class Model : public ResourceBase
{
public:
	struct VertexType
	{
		DXFLOAT3 position;
		DXFLOAT2 texture;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	Model();
	~Model();

	bool Initialize(const std::wstring& filePath, const std::wstring& name, bool enableMipmap);
	bool LoadModel(const wchar_t* filename);

	ID3D11Buffer* GetModelVertexBuffer();
	ID3D11Buffer* GetModelIndexBuffer();
	int GetIndexCount();
	int GetVertexCount();

private:
	ID3D11Buffer* m_vertexBuffer{ nullptr };
	ID3D11Buffer* m_indexBuffer{ nullptr };
	int m_vertexCount{ 0 };
	int m_indexCount{ 0 };
	ModelType* m_model{ nullptr };
};

