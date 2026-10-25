#pragma once
#include <DirectXMath.h>

// Data types
using DXFLOAT2 = DirectX::XMFLOAT2;
using DXFLOAT3 = DirectX::XMFLOAT3;
using DXFLOAT4 = DirectX::XMFLOAT4;
using DXMATRIX = DirectX::XMMATRIX;
using DXFLOAT4X4 = DirectX::XMFLOAT4X4;
using DXVECTOR = DirectX::XMVECTOR;

// Functions
using DirectX::XMMatrixIdentity;
using DirectX::XMMatrixScaling;
using DirectX::XMMatrixRotationRollPitchYaw;
using DirectX::XMMatrixTranslation;
using DirectX::XMMatrixLookAtLH;
using DirectX::XMMatrixPerspectiveFovLH;
using DirectX::XMMatrixOrthographicLH;
using DirectX::XMMatrixOrthographicOffCenterLH;
using DirectX::XMMatrixTranspose;
using DirectX::XMVectorAdd;