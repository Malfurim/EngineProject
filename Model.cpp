#include "Model.h"

Model::Model()
{
}

Model::~Model()
{
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}

	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	if (m_model)
	{
		delete[] m_model;
		m_model = nullptr;
	}
}

bool Model::Initialize(ID3D11Device* device, LPCWSTR modelPath, bool writeable)
{
	// change to only load vertex and index buffers???

	LoadModel(modelPath);

	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Create the vertex array
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	// Create the index array
	indices = new unsigned long[m_indexCount];
	if (!indices)
		return false;

	// Load the vertex array and index array with data
	for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		//vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	// Set up vertex buffer description
	vertexBufferDesc.Usage = (writeable) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = (writeable) ? D3D11_CPU_ACCESS_WRITE : 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Set up vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up index buffer description
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Set up index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

/*
void Model::Render(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	m_shader->Begin(deviceContext, m_indexCount);

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_shader->End(deviceContext);
}
*/

bool Model::LoadModel(const wchar_t* filename)
{
	std::ifstream fin;
	char input;
	int i;

	// Get name of the model file
	m_name = filename;
	int pos = m_name.find_last_of(L"/");
	if (pos >= 0)
	{
		m_name = m_name.substr(pos + 1, m_name.length());
	}
	m_name = m_name.substr(0, m_name.find_last_of(L"."));

	// Open the model file
	fin.open(filename);

	// if it could not open the file then exit
	if (fin.fail())
	{
		return false;
	}

	// Read in the vertex count
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in
	m_model = new ModelType[m_vertexCount];

	// Jump one line down
	fin.get(input);

	// Read in the vertex data
	for (i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file
	fin.close();

	return true;
}

ID3D11Buffer* Model::GetModelVertexBuffer()
{
	return m_vertexBuffer;
}

ID3D11Buffer* Model::GetModelIndexBuffer()
{
	return m_indexBuffer;
}

int Model::GetIndexCount()
{
	return m_indexCount;
}

int Model::GetVertexCount()
{
	return m_vertexCount;
}

std::wstring Model::GetName()
{
	return m_name;
}
