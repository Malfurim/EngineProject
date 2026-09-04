#include "Texture.h"
#include "DirectXManager.h"

Texture::Texture()
{
}

Texture::~Texture()
{
	if (m_texture)
	{
		m_texture->Release();
		m_texture = nullptr;
	}

	if (m_textureData)
	{
		m_textureData->Release();
		m_textureData = nullptr;
	}
	if (m_imageData)
	{
		delete[] m_imageData;
		m_imageData = nullptr;
	}

	m_name.clear();
	m_path.clear();
}

bool Texture::Initialize(const std::wstring& fileName, bool enableMipmap)
{
	bool result;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hResult;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	m_path = fileName;
	m_name = fileName;

	int pos = m_name.find_last_of(L"/");
	if (pos >= 0)
	{
		m_name = m_name.substr(pos + 1, m_name.length());
	}
	m_name = m_name.substr(0, m_name.find_last_of(L"."));

	// Load the texture from file
	result = LoadImageData(fileName);
	if (!result)
	{
		return false;
	}

	// Setup the description of the texture
	textureDesc.Width = m_width;
	textureDesc.Height = m_height;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.CPUAccessFlags = 0;

	if (enableMipmap)	// for 3D
	{
		textureDesc.MipLevels = 0;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}
	else				// for 2D, UI
	{
		textureDesc.MipLevels = 1;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.MiscFlags = 0;
	}

	// Create the empty texture
	hResult = DXDEVICE->CreateTexture2D(&textureDesc, nullptr, &m_textureData);
	if (FAILED(hResult))
	{
		return false;
	}

	// Set the rwo pitch of the image data
	rowPitch = (m_width * 4) * sizeof(unsigned char);

	// Copy the image data into the texture
	DXDEVICECONTEXT->UpdateSubresource(m_textureData, 0, nullptr, m_imageData, rowPitch, 0);

	// Setup the shader resource view desription
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = (enableMipmap) ? -1 : 1;

	// Create the shader resource view for the texture
	hResult = DXDEVICE->CreateShaderResourceView(m_textureData, &srvDesc, &m_texture);
	if (FAILED(hResult))
	{
		return false;
	}

	// Generate mipmaps for the 3D texture
	if (enableMipmap)
		DXDEVICECONTEXT->GenerateMips(m_texture);

	// Release the image data
	delete[] m_imageData;
	m_imageData = nullptr;

	return true;
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return m_texture;
}

std::wstring Texture::GetName()
{
	return m_name;
}

int Texture::GetWidth()
{
	return m_width;
}

int Texture::GetHeight()
{
	return m_height;
}

bool Texture::LoadImageData(const std::wstring& fileName)
{
	// Get file extension and check if it is something known and process it
	std::wstring fileEx = fileName;
	fileEx = fileEx.substr(fileEx.find_last_of(L".") + 1, fileEx.length());

	if (!wcscmp(fileEx.c_str(), L"bmp"))
	{
		return LoadBMPImage(fileName);
	}
	else if (!wcscmp(fileEx.c_str(), L"png"))
	{
		return LoadPNGImage(fileName);
	}
	else if (!wcscmp(fileEx.c_str(), L"jpg"))
	{
		return LoadJPGImage(fileName);
	}
	else if (!wcscmp(fileEx.c_str(), L"tga"))
	{
		return LoadTGAImage(fileName);
	}
	else
	{
		MessageBox(0, L"Error loading an image of unknown format.", L"ERROR", MB_OK);
		return false;
	}

	return true;
}

bool Texture::LoadBMPImage(const std::wstring& fileName)
{
	int error;
	int count, index, offset, x, y;
	int colorBytes = 4;
	unsigned int imageSize;
	unsigned char* image;
	FILE* filePtr;
	BitmapHeader header1;
	BitmapHeader2 header2;

	// Open the file
	error = _wfopen_s(&filePtr, fileName.c_str(), L"rb");
	if (error != 0)
	{
		MessageBox(0, L"Error opening a file", L"ERROR", MB_OK);
		return false;
	}

	if (filePtr == nullptr)
	{
		return false;
	}

	// Read in first header and check it is BM type
	count = (unsigned int)fread(&header1, sizeof(BitmapHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	if (header1.type[0] != 'B' && header1.type[1] != 'M')
	{
		MessageBox(0, L"File is not a proper BMP image", L"ERROR loading image", MB_OK);
		return false;
	}

	// Check size of the second header
	if (header1.headerSize != 40)
	{
		MessageBox(0, L"File is incorrect BMP image", L"ERROR loading image", MB_OK);
		return false;
	}

	// Read in width and height
	count = (unsigned int)fread(&header2, sizeof(BitmapHeader2), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	m_width = header2.width;
	m_height = header2.height;
	m_bitsPerPixel = header2.bitsPerPixel;

	if (header2.compression != 0)
	{
		MessageBox(0, L"Cannot read compressed BMP image", L"ERROR loading image", MB_OK);
		return false;
	}

	// Check if the image is 24 or 32 bits per pixel
	if (m_bitsPerPixel == 24)
	{
		colorBytes = 3;
	}

	// Prepare array for the image data
	imageSize = m_width * m_height * colorBytes;
	image = new unsigned char[imageSize];

	// Read in the image data
	count = (unsigned int)fread(image, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		MessageBox(0, L"Error reading image. Size doesn't match read pixel count.", L"ERROR laoding image", MB_OK);
		return false;
	}

	// Close the file
	error = fclose(filePtr);
	if (error != 0)
	{
		MessageBox(0, L"ERROR CLOSE", L"INFO", MB_OK);
		return false;
	}

	// Allocate space for the image data
	m_imageData = new unsigned char[m_width * m_height * 4];

	// Set the last pixel line as first
	offset = (m_width * m_height * colorBytes) - (m_width * colorBytes);
	index = 0;
	for (y = 0; y < m_height; y++)
	{
		for (x = 0; x < m_width; x++)
		{
			m_imageData[index + 0] = image[offset + 2];		// Red
			m_imageData[index + 1] = image[offset + 1];		// Green
			m_imageData[index + 2] = image[offset + 0];		// Blue
			if (m_bitsPerPixel == 24)
				m_imageData[index + 3] = 255;	// Alpha
			else
				m_imageData[index + 3] = image[offset + 3];	// Alpha

			offset += colorBytes;
			index += 4;
		}

		// Move back to new line of pixels
		offset -= (m_width * colorBytes * 2);
	}

	// Release the image data
	delete[] image;
	image = nullptr;

	return true;
}

bool Texture::LoadPNGImage(const std::wstring& fileName)
{
	return false;
}

bool Texture::LoadJPGImage(const std::wstring& fileName)
{
	return false;
}

bool Texture::LoadTGAImage(const std::wstring& fileName)
{
	int error;
	int count, index, offset, x, y;
	int colorBytes = 4;
	unsigned int imageSize;
	unsigned char* image;
	FILE* filePtr;
	TargaHeader header;

	// Open the file
	error = _wfopen_s(&filePtr, fileName.c_str(), L"rb");
	if (error != 0)
	{
		MessageBox(0, L"Error opening a file", L"ERROR", MB_OK);
		return false;
	}
	
	if (filePtr == nullptr)
	{
		return false;
	}

	// Read in the file header
	count = (unsigned int)fread(&header, sizeof(TargaHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Get the information from the header
	m_width = header.width;
	m_height = header.height;
	m_bitsPerPixel = header.bitsPerPixel;

	// Check if the image is 24 or 32 bits per pixel
	if (m_bitsPerPixel != 32)
	{
		// 24 bits per pixel targa image not implemented
		return false;
	}

	// Prepare array for the image data
	imageSize = m_width * m_height * colorBytes;
	image = new unsigned char[imageSize];

	// Read in the image data
	count = (unsigned int)fread(image, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		MessageBox(0, L"Error reading image. Size doesn't match read pixel count.", L"ERROR laoding image", MB_OK);
		return false;
	}

	// Close the file
	error = fclose(filePtr);
	if (error != 0)
	{
		MessageBox(0, L"ERROR CLOSE", L"INFO", MB_OK);
		return false;
	}

	// Allocate space for the image data
	m_imageData = new unsigned char[m_width * m_height * 4];

	// Set the last pixel line as first
	offset = (m_width * m_height * colorBytes) - (m_width * colorBytes);
	index = 0;
	for (y = 0; y < m_height; y++)
	{
		for (x = 0; x < m_width; x++)
		{
			m_imageData[index + 0] = image[offset + 2];		// Red
			m_imageData[index + 1] = image[offset + 1];		// Green
			m_imageData[index + 2] = image[offset + 0];		// Blue
			m_imageData[index + 3] = image[offset + 3];		// Alpha

			offset += colorBytes;
			index += 4;
		}

		// Move back  to new line of pixels
		offset -= (m_width * colorBytes * 2);
	}

	// Release the image data
	delete[] image;
	image = nullptr;

	return true;
}
