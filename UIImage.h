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
	void Update() override;
	void Render() override;
	static void Shutdown();

	// --- VIRTUAL FUNCTIONS ---


	// --- CLASS API ---
	void EnableAnimation(unsigned int framesPerRow, unsigned int framesPerColumn, unsigned int framesPerSecond);
	void DisableAnimation();

	// --- GETTERS & SETTERS ---
	void SetSize(Size size) override;
	void SetSize(float width, float height) override;
	void SetImage(Texture* texture);

	// --- STATIC CLASS API ---


protected:
	// --- CONSTRUCTORS ---


	// --- VIRTUAL FUNCTIONS ---
	void CalculateGeometry() override;

	// --- PROTECTED FUNCTIONS ---


private:
	// --- CONSTRUCTORS ---


	// --- PRIVATE FUNCTIONS ---
	void CheckMinimalSize();
	void ConfirmShader();

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
	bool m_animated{ false };
	int m_currentFrame{ 0 };

	// --- PRIVATE COMPONENT DATA ---
	Texture* m_texture{ nullptr };
	unsigned int m_textureFramesPerRow{ 1 };
	unsigned int m_textureFramesPerColumn{ 1 };
	unsigned int m_frames{ 1 };
	double m_frameDelta{ 0.0 };
	double m_frameInterval{ 0.0 };

	// --- STATIC GLOBAL STATES ---
	static TextureShader* ms_imageShader;
};

