#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>

#include "Shader.h"
#include "Event.h"
#include "InputManager.h"

using namespace DirectX;

#define VERTEXSTRUCTSIZE sizeof(SPDNT2Vertex)

typedef XMFLOAT3 TPosition;
typedef XMFLOAT4 TColor;
typedef XMFLOAT3 TNormal;
typedef XMFLOAT3 TDiffuse;
typedef XMFLOAT2 TTextureCoordinate;

enum BackgroundType
{
	BACKGROUND_NONE,
	BACKGROUND_SOLID,
	BACKGROUND_TRANSPARENT
};

struct Color
{
	float r;
	float g;
	float b;
	float a;

	bool operator==(const Color& other) const
	{
		return (r == other.r && g == other.g && b == other.b && a == other.a);
	}

	bool operator!=(const Color& other) const
	{
		return !(*this == other);
	}
};

struct SPDNT2Vertex
{
	TPosition position{ 0.0f, 0.0f, 0.0f };
	TDiffuse diffuse{ 0.0f, 0.0f, 0.0f };
	TNormal normal{ 0.0f, 0.0f, 0.0f };
	TTextureCoordinate texCoord{ 0.0f, 0.0f };
	TTextureCoordinate texCoord2{ 0.0f, 0.0f };
};

class GraphicBase
{
private:
	struct SPCVertex
	{
		TPosition position;
		TColor color;
	};

public:
	virtual ~GraphicBase();
	static void Shutdown();

	void SetBackgroundColor(Color color);
	Color GetBackgroundColor();

	void Focus();
	static void Unfocus();
	static bool IsFocused() { return ms_focus != nullptr; }
	static GraphicBase* GetFocus() { return ms_focus; }
	virtual void OnTextInput(wchar_t character) {}
	virtual bool CapturesKeyboard() const { return false; }

	InputState GetRequiredInputState() const { return m_requiredInputState; }

	Event<> OnMouseEnter;
	Event<> OnMouseLeave;
	Event<> OnLeftClick;
	Event<> OnRightClick;
	Event<> OnMiddleClick;
	Event<> OnRightRelease;
	Event<> OnLeftRelease;
	Event<> OnMiddleRelease;
	Event<int, int> OnMouseMove;
	Event<GraphicBase*> OnFocus;
	Event<GraphicBase*> OnFocusLost;

	bool IsHover() const { return m_isHover; }

	bool Visibility{ true };

private:
	bool SetVertexBuffer();
	bool SetIndexBuffer();

protected:
	GraphicBase();
	virtual void Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	virtual void Update();

	bool LoadBuffers();

	virtual bool CheckMouseOver() const = 0;
	void UpdateMouseEvents();

protected:
	BackgroundType m_background{ BACKGROUND_NONE };
	Color m_backgroundColor{ 0.0f, 0.0f, 0.0f, 0.0f };

	ID3D11Buffer* m_vertexBuffer{ nullptr };
	ID3D11Buffer* m_indexBuffer{ nullptr };
	SPDNT2Vertex* m_vertices{ nullptr };
	unsigned long* m_indices{ nullptr };

	int m_vertexCount{ 0 };
	int m_indexCount{ 0 };
	int m_index{ 0 };

	bool m_dynamic{ false };
	bool m_isHover{ false };
	InputState m_requiredInputState{ InputState::INPUT_STATE_GAMEPLAY };

	static GraphicBase* ms_focus;

private:
	ID3D11Buffer* m_vertexBufferBackground{ nullptr };

	static Shader* ms_shader;
};

