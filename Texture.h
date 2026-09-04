#pragma once

#include <d3d11.h>
#include <fstream>
#include <string>

class Texture
{
public:
	Texture();
	~Texture();
	
	bool Initialize(const std::wstring& fileName, bool enableMipmap);

	ID3D11ShaderResourceView* GetTexture();
	std::wstring GetName();
	int GetWidth();
	int GetHeight();

private:
	bool LoadImageData(const std::wstring& fileName);
	bool LoadBMPImage(const std::wstring& fileName);
	bool LoadPNGImage(const std::wstring& fileName);
	bool LoadJPGImage(const std::wstring& fileName);
	bool LoadTGAImage(const std::wstring& fileName);

	unsigned char* m_imageData{ nullptr };
	ID3D11Texture2D* m_textureData{ nullptr };
	ID3D11ShaderResourceView* m_texture{ nullptr };
	std::wstring m_name;
	std::wstring m_path;
	int m_width{ 0 };
	int m_height{ 0 };
	short m_bitsPerPixel{ 0 };

private:
#pragma pack(1)
	struct BitmapHeader
	{
		char type[2];
		int size;
		short data1;
		short data2;
		int pixelOffset;
		int headerSize;
	};

	struct BitmapHeader2
	{
		int width;
		int height;
		short numPlanes;
		short bitsPerPixel;
		int compression;
		int imageSize;
		int horizontalResolution;
		int verticalResolution;
		int numColorsInPalette;
		int numImportantColors;
	};

	struct TargaHeader
	{
		unsigned char data[12];
		unsigned short width;
		unsigned short height;
		unsigned char bitsPerPixel;
		unsigned char data2;
	};
#pragma pack()
};
