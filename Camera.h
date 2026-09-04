#pragma once

#include <DirectXMath.h>
using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(XMMATRIX& viewMatrix);

	int GetSizeOfElement();

private:
	XMFLOAT3 m_position{};
	XMFLOAT3 m_rotation{};

	XMMATRIX m_viewMatrix{};
};

