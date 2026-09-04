#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

using namespace DirectX;

class Model
{
public:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	Model();
	~Model();

	bool Initialize(ID3D11Device* device, LPCWSTR filename, bool writeable = false);
	bool LoadModel(const wchar_t* filename);

	ID3D11Buffer* GetModelVertexBuffer();
	ID3D11Buffer* GetModelIndexBuffer();
	int GetIndexCount();
	int GetVertexCount();
	std::wstring GetName();

private:
	ID3D11Buffer* m_vertexBuffer{ nullptr };
	ID3D11Buffer* m_indexBuffer{ nullptr };
	int m_vertexCount{ 0 };
	int m_indexCount{ 0 };
	ModelType* m_model{ nullptr };
	std::wstring m_name{};
};

