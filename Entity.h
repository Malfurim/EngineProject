#pragma once

#include "ResourceManager.h"

class Entity
{
public:
	Entity();
	~Entity();

	bool Initialize(const std::wstring& path, int index);

	void Render(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	Shader* GetShader();
	std::wstring GetName();
	std::wstring GetPath();

private:
	Model* m_model{ nullptr };
	Texture* m_texture{ nullptr };
	Shader* m_shader{ nullptr };
	std::wstring m_name{};
	std::wstring m_path{};

	int m_index{};
};

