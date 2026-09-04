#pragma once

#include "UIElement.h"
#include "TextureShader.h"

class UIImage : public UIElement
{
public:
	UIImage();
	UIImage(Position position, Size size);
	UIImage(Position position, Size size, Color backgroundColor);
	~UIImage();
	static void Shutdown();

	void Render();
	void Update();

	virtual void SetSize(Size size);
	virtual void SetSize(float width, float height);
	void CheckMinimalSize();

	void SetImage(Texture* texture);
	void SetBorder(float size);

private:
	void ConfirmShader();
	void CalculateGeometry();

private:
	Texture* m_texture{ nullptr };
	static TextureShader* ms_imageShader;

	float m_borderSize{ 0.0f };
};

