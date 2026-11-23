#include "FX.h"
#include "Gameobject.h"
#include "WindowUtils.h"
#include "Mesh.h"

void GameObject::Initialise(const std::string& meshFileName)
{
	MyD3D& d3d = WinUtil::Get().GetD3D();

}



void GameObject::Initialise(Mesh& mesh)
{
	using namespace DirectX::SimpleMath;
	mpMesh = &mesh;
	mPosition = Vector3(0, 0, 0);
	mScale = Vector3(1, 1, 1);
	mRotation = Vector3(0, 0, 0);
}

void GameObject::GetWorldMatrix(DirectX::SimpleMath::Matrix& w)
{
	using namespace DirectX::SimpleMath;
	w = Matrix::CreateScale(mScale) * Matrix::CreateRotationX(mRotation.x) *
		Matrix::CreateRotationY(mRotation.y) * Matrix::CreateRotationZ(mRotation.z) *
		Matrix::CreateTranslation(mPosition);
}

