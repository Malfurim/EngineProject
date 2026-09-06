#include "Camera.h"

// --- ADDITIONAL INCLUDES ---


// --- MACROS & DEFINES ---


// --- FORWARD DECLARATIONS ---


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC GLOBAL STATES & DATA								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CONSTRUCTORS & DESTRUCTOR								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
Camera::Camera()
{
	m_position = {0.0f, 0.0f, 0.0f};
	m_rotation = {0.0f, 0.0f, 0.0f};
}

Camera::~Camera()
{
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	VIRTUAL FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//bool Camera::Initialize()
//{
//	return true;
//}

//void Camera::Update()
//{
//}

void Camera::Render()
{
	XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };
	XMFLOAT3 position = m_position;
	XMFLOAT3 lookAt = { 0.0f, 0.0f, 1.0f };

	XMVECTOR upVector = XMLoadFloat3(&up);
	XMVECTOR positionVector = XMLoadFloat3(&position);
	XMVECTOR lookAtVector = XMLoadFloat3(&lookAt);

	// PI / 180.0f
	float oneDegree = 0.0174532925f;
	float yaw = m_rotation.x * oneDegree;
	float pitch = m_rotation.y * oneDegree;
	float roll = m_rotation.z * oneDegree;

	// Create the rotation matrix from the yaw, pitch and roll values
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CLASS API												//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	GETTERS & SETTERS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void Camera::GetViewMatrix(XMMATRIX& viewMatrix)
{
	XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };
	XMFLOAT3 position = m_position;
	XMFLOAT3 lookAt = { 0.0f, 0.0f, 1.0f };

	XMVECTOR upVector = XMLoadFloat3(&up);
	XMVECTOR positionVector = XMLoadFloat3(&position);
	XMVECTOR lookAtVector = XMLoadFloat3(&lookAt);

	// PI / 180.0f
	float oneDegree = 0.0174532925f;
	float yaw = m_rotation.x * oneDegree;
	float pitch = m_rotation.y * oneDegree;
	float roll = m_rotation.z * oneDegree;

	// Create the rotation matrix from the yaw, pitch and roll values
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	viewMatrix = m_viewMatrix;
}

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

