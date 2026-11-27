#include "FX.h"
#include "Gameobject.h"
#include "WindowUtils.h"
#include "Mesh.h"

void GameObject::Initialise(const std::string& meshFileName)
{
	MyD3D& d3d = WinUtil::Get().GetD3D();

}



void GameObject::SetUp(const std::string& meshFileName, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation)
{
	Initialise(meshFileName);
	SetPosition(position);
	SetScale(scale);
	SetRotation(rotation);
}

void GameObject::SetUp(const std::string& meshFileName, const DirectX::SimpleMath::Vector3 position, const float scale, const DirectX::SimpleMath::Vector3& rotation)
{
	SetUp(meshFileName, position, DirectX::SimpleMath::Vector3(scale, scale, scale), rotation);
}

void GameObject::Initialise(const Mesh& mesh)
{
	using namespace DirectX::SimpleMath;
	mpMesh = &mesh;
	mPosition = Vector3(0, 0, 0);
	mScale = Vector3(1, 1, 1);
	mRotation = Vector3(0, 0, 0);
}

void GameObject::SetUp(const Mesh& mesh, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation)
{
	Initialise(mesh);
	SetPosition(position);
	SetScale(scale);
	SetRotation(rotation);
}

void GameObject::SetUp(const Mesh& mesh, const DirectX::SimpleMath::Vector3& position, const float scale, const DirectX::SimpleMath::Vector3& rotation)
{
	SetUp(mesh, position, DirectX::SimpleMath::Vector3(scale, scale, scale), rotation);
}

const Mesh& GameObject::GetMesh() const
{
	assert(mpMesh);
	return *mpMesh;
}

DirectX::SimpleMath::Matrix GameObject::GetWorldMatrix() const
{
	using namespace DirectX::SimpleMath;
	Matrix worldMatrix = 
		Matrix::CreateScale(mScale)
		* Matrix::CreateRotationX(mRotation.x)
		* Matrix::CreateRotationY(mRotation.y)
		* Matrix::CreateRotationZ(mRotation.z)
		* Matrix::CreateTranslation(mPosition);
	return worldMatrix;
}



const Material& GameObject::GetOverrideMat() const
{

	return mOverrideMaterial;
}


void GameObject::SetWorldMatrix(const DirectX::SimpleMath::Vector3& newPosition, const DirectX::SimpleMath::Vector3& newScale, const DirectX::SimpleMath::Vector3& newRotation)
{
	SetPosition(newPosition);
	SetScale(newScale);
	SetRotation(newRotation);
}

void GameObject::SetUseOverrideMat(bool newUseState)
{
	assert(mhasOverrideMaterial);
	mUseOverrideMat = newUseState;
	
}

void GameObject::SetOverrideMat(const Material& newOverrideMat, const bool useOverrideMat)
{
	mOverrideMaterial = newOverrideMat;
	mhasOverrideMaterial = true;
	mUseOverrideMat = useOverrideMat;
}

