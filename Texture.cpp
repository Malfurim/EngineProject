#include "Texture.h"

// --- ADDITIONAL INCLUDES ---
#include <d3d11.h>
#include <fstream>
#include "DirectXManager.h"
#include "Deleters.h"
#include "Utils.h"
#include <WICTextureLoader.h>

// --- MACROS & DEFINES ---


// --- FORWARD DECLARATIONS ---


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC GLOBAL STATES & DATA								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CONSTRUCTORS & DESTRUCTOR								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
Texture::Texture()
{
}

Texture::~Texture()
{
	SAFE_RELEASE(m_texture);
	m_name.clear();
	m_path.clear();
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CORE FUNCTIONS											//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
bool Texture::Initialize(const std::wstring& filePath, const std::wstring& name, bool enableMipmap)
{
	m_path = filePath;
	m_name = name;

	HRESULT result;
	ID3D11Resource* textureResource = nullptr;

	if (enableMipmap)
	{
		result = DirectX::CreateWICTextureFromFileEx(
			DXDEVICE, DXDEVICECONTEXT, filePath.c_str(), 0,
			D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
			0, D3D11_RESOURCE_MISC_GENERATE_MIPS, DirectX::WIC_LOADER_DEFAULT,
			&textureResource, &m_texture);
	}
	else
	{
		result = DirectX::CreateWICTextureFromFile(
			DXDEVICE, filePath.c_str(),
			&textureResource, &m_texture);
	}

	if (FAILED(result) || textureResource == nullptr)
	{
		return false;
	}

	ID3D11Texture2D* textureData = nullptr;
	result = textureResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&textureData);
	SAFE_RELEASE(textureResource);

	if (FAILED(result) || textureData == nullptr)
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC textureDesc;
	textureData->GetDesc(&textureDesc);
	m_width = textureDesc.Width;
	m_height = textureDesc.Height;
	SAFE_RELEASE(textureData);

	return true;
}

//void Texture::Update()
//{
//}

//void Texture::Render()
//{
//}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	VIRTUAL FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CLASS API												//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	GETTERS & SETTERS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC CLASS API										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	VIRTUAL FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PRIVATE FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //

