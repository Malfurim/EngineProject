#include "Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
	m_name.clear();
}

bool Entity::Initialize(const std::wstring& path, int index)
{
	m_index = index;
	m_path = path;
	m_name = path;
	int pos = m_name.find_last_of(L"/");
	if (pos >= 0)
	{
		m_name = m_name.substr(pos + 1, m_name.length());
	}
	m_name = m_name.substr(0, m_name.find_last_of(L"."));

	std::wifstream file;
	wchar_t input{};
	std::wstring elementName{};
	file.open(path);

	// if it could not open the file then exit
	if (file.fail())
	{
		MessageBox(NULL, L"Entity file not found.", L"ERROR", MB_OK);
		return false;
	}

	// Model
	elementName = {};
	file.get(input);
	while (input != '\t')
	{
		elementName += input;
		file.get(input);
	}
	m_model = RESOURCE->GetModel(elementName);

	// Shader
	elementName = {};
	file.get(input);
	while (input != '\t')
	{
		elementName += input;
		file.get(input);
	}
	m_shader = RESOURCE->GetShader(elementName);

	// Texture
	elementName = {};
	file.get(input);
	while (input != '\t')
	{
		elementName += input;
		file.get(input);
	}
	m_texture = RESOURCE->GetTexture(elementName);

	file.close();
	return true;
}

void Entity::Render(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(Model::VertexType);
	offset = 0;
	
	ID3D11Buffer* vertexBuffer = m_model->GetModelVertexBuffer();
	
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_model->GetModelIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	m_shader->SetShaderParameters(deviceContext, m_texture->GetTexture());
	m_shader->SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	m_shader->Begin(deviceContext, m_model->GetIndexCount());

	m_shader->End(deviceContext);
}

Shader* Entity::GetShader()
{
	return m_shader;
}

std::wstring Entity::GetName()
{
	return m_name;
}

std::wstring Entity::GetPath()
{
	return m_path;
}
