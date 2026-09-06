#pragma once

// --- INCLUDES ---
#include "UIElement.h"

// --- CONSTANTS ---


// --- MACROS & DEFINES ---


// --- ENUMS & STRUCTS ---


// --- FORWARD DECLARATIONS ---
class Texture;
class TextureShader;


class UIImage : public UIElement
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	UIImage();
	UIImage(Position position, Size size);
	UIImage(Position position, Size size, Color backgroundColor);
	~UIImage();

	// --- CORE FUNCTIONS ---
	// bool Initialize();
	void Update();
	void Render();
	static void Shutdown();

	// --- VIRTUAL FUNCTIONS ---


	// --- CLASS API ---


	// --- GETTERS & SETTERS ---
	void SetSize(Size size) override;
	void SetSize(float width, float height) override;
	void SetImage(Texture* texture);
	void SetBorder(float size);

	// --- STATIC CLASS API ---


protected:
	// --- CONSTRUCTORS ---


	// --- VIRTUAL FUNCTIONS ---


	// --- PROTECTED FUNCTIONS ---


private:
	// --- CONSTRUCTORS ---


	// --- PRIVATE FUNCTIONS ---
	void CheckMinimalSize();
	void ConfirmShader();
	void CalculateGeometry();

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


	// --- PRIVATE COMPONENT STATES ---


	// --- PRIVATE COMPONENT DATA ---
	Texture* m_texture{ nullptr };
	float m_borderSize{ 0.0f };

	// --- STATIC GLOBAL STATES ---
	static TextureShader* ms_imageShader;
};

