#include "WorldObject.h"

// --- ADDITIONAL INCLUDES ---
#ifdef _DEBUG
#include <Windows.h>
#endif

#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "DirectXManager.h"

// --- MACROS & DEFINES ---


// --- FORWARD DECLARATIONS ---


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC GLOBAL STATES & DATA								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
DXMATRIX WorldObject::ms_viewMatrix = XMMatrixIdentity(); //XMMatrixMultiply(XMMatrixIdentity(), XMMatrixTranslation(0.0f, 0.0f, 1.0f));
DXMATRIX WorldObject::ms_projectionMatrix = XMMatrixIdentity(); //XMMatrixOrthographicOffCenterLH(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, SCREEN_NEAR, SCREEN_DEPTH);

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CONSTRUCTORS & DESTRUCTOR								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
WorldObject::WorldObject(Mesh* mesh, Material* material)
{
	m_worldMatrix = XMMatrixIdentity();
	m_mesh = mesh;
	m_material = material;
}

WorldObject::~WorldObject()
{
	MATERIALRELEASE(m_material->GetName());
	MESHRELEASE(m_mesh->GetName());
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CORE FUNCTIONS											//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//bool WorldObject::Initialize()
//{
//	return true;
//}

void WorldObject::Update()
{
	if (IsInvalidated(UpdateFlag::UPDATE_POSITION))
	{
		float oneDegree = 0.0174532925f;
		float pitch = m_rotation.x * oneDegree;
		float yaw = m_rotation.y * oneDegree;
		float roll = m_rotation.z * oneDegree;
		DXFLOAT3 pivotPoint = m_mesh->GetPivotPoint();
		DXMATRIX offset = XMMatrixTranslation(-pivotPoint.x, -pivotPoint.y, -pivotPoint.z);
		DXMATRIX offsetBack = XMMatrixTranslation(pivotPoint.x, pivotPoint.y, pivotPoint.z);

		DXMATRIX scale = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
		DXMATRIX rotation = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
		DXMATRIX position = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

		DXMATRIX worldMatrix = XMMatrixMultiply(scale, offset);
		worldMatrix = XMMatrixMultiply(worldMatrix, rotation);
		worldMatrix = XMMatrixMultiply(worldMatrix, offsetBack);
		worldMatrix = XMMatrixMultiply(worldMatrix, position);
		m_worldMatrix = worldMatrix;
		Validate(UpdateFlag::UPDATE_POSITION);
	}
}

void WorldObject::Render()
{
	if (!Visibility)
		return;

	unsigned int stride = sizeof(SPDNTVertex);
	unsigned int offset = 0;

	ID3D11Buffer* vertexBuffer = m_mesh->GetMeshVertexBuffer();

	DXDEVICECONTEXT->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DXDEVICECONTEXT->IASetIndexBuffer(m_mesh->GetMeshIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	DXDEVICECONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_material->GetShader()->SetShaderParameters(m_material->GetTexture()->GetShaderResource());
	m_material->GetShader()->SetShaderParameters(m_worldMatrix, ms_viewMatrix, ms_projectionMatrix);
	m_material->GetShader()->Render(m_mesh->GetIndexCount());

	vertexBuffer = nullptr;
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PUBLIC VIRTUAL FUNCTIONS								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CLASS API												//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	GETTERS & SETTERS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC CLASS API										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
WorldObject* WorldObject::Create(Mesh* mesh, Material* material)
{
	if (mesh == nullptr || material == nullptr)
	{
		#ifdef _DEBUG
		MessageBox(NULL, L"Cannot create new WorldObject: Mesh or Material is nullptr", L"ERROR", MB_OK);
		#endif
		return nullptr;
	}

	return new WorldObject(mesh, material);
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED VIRTUAL FUNCTIONS								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
bool WorldObject::IsMouseOver() const
{
	return false;
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PRIVATE FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //

