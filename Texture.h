#pragma once

// --- INCLUDES ---
#include <d3d11.h>
#include <string>

// --- CONSTANTS ---


// --- MACROS & DEFINES ---


// --- ENUMS & STRUCTS ---


// --- FORWARD DECLARATIONS ---

class Texture
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	Texture();
	~Texture();
	
	// --- CORE FUNCTIONS ---
	bool Initialize(const std::wstring& fileName, bool enableMipmap);
	// void Update();
	// void Render();

	// --- VIRTUAL FUNCTIONS ---


	// --- CLASS API ---


	// --- GETTERS & SETTERS ---
	ID3D11ShaderResourceView* GetTexture() const { return m_texture; }
	std::wstring GetName() const { return m_name; }
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	// --- STATIC CLASS API ---


protected:
	// --- CONSTRUCTORS ---


	// --- VIRTUAL FUNCTIONS ---


	// --- PROTECTED FUNCTIONS ---


private:
	// --- CONSTRUCTORS ---


	// --- PRIVATE FUNCTIONS ---
	bool LoadImageData(const std::wstring& fileName);
	bool LoadBMPImage(const std::wstring& fileName);
	bool LoadPNGImage(const std::wstring& fileName);
	bool LoadJPGImage(const std::wstring& fileName);
	bool LoadTGAImage(const std::wstring& fileName);

// - PROPERTIES -
public:
	// --- PUBLIC COMPONENT STATES ---


	// --- PUBLIC COMPONENT DATA ---


protected:
	// --- INTERNAL CONSTANTS ---


	// --- INTERNAL STRUCTS & ENUMS ---


	// --- INTERNAL COMPONENT STATES ---


	// --- INTERNAL COMPONENT DATA ---


private:
	// --- PRIVATE CONSTANTS ---


	// --- PRIVATE STRUCTS & ENUMS ---
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

	// --- PRIVATE COMPONENT STATES ---


	// --- PRIVATE COMPONENT DATA ---
	unsigned char* m_imageData{ nullptr };
	ID3D11Texture2D* m_textureData{ nullptr };
	ID3D11ShaderResourceView* m_texture{ nullptr };
	std::wstring m_name;
	std::wstring m_path;
	int m_width{ 0 };
	int m_height{ 0 };
	short m_bitsPerPixel{ 0 };

	// --- STATIC GLOBAL STATES ---

};
