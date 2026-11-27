#include "Player.hpp"
#include "WindowUtils.h"
#include "D3D.h"
#include "GeometryBuilder.h"

Player::Player() :
	GameObject()
{
	using namespace DirectX::SimpleMath;
	MyD3D& d3d = WinUtil::Get().GetD3D();
	// Create the player mesh and init game object with it
	Mesh& playerMesh = BuildPlayerCube(d3d.GetMeshMgr()); // Player is cube for the moment
	GameObject::Initialise(playerMesh);

	// Set default values
	constexpr Vector3 defaultPosition = { 0.f, -0.5f, 1.f };
	constexpr Vector3 defaultRotation = { 0.f, 0.f, 0.f };
	constexpr Vector3 defaultScale = { 0.5f, 0.5f, 0.5f };
	GameObject::SetPosition(defaultPosition);
	GameObject::SetScale(defaultScale);
	GameObject::SetRotation(defaultRotation);

	// Set up material
	constexpr char* defaultTextureFile = "test.dds";
	Material mat = GameObject::GetMesh().GetSubMesh(0).material;
	mat.gfxData.Set(Vector4(0.5, 0.5, 0.5, 1), Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 1));
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), std::string(defaultTextureFile));
	mat.texture = std::string(defaultTextureFile);
	GameObject::SetOverrideMat(mat);
	
}
