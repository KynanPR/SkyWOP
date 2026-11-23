#ifndef GameObject_H
#define GameObject_H

#include <string>
#include <cassert>
#include "d3d.h"

//forward declaration, only using a reference/pointer
class Mesh;

/*
* A gameobject is a rendered instance of a mesh, which is geometry
*/
class GameObject
{
public:
	//go find the mesh with this name and setup
	void Initialise(const std::string& meshFileName);
	//setup using the given mesh
	void Initialise(Mesh& mesh);

	//get and optionally set pos/scale/rotation
	DirectX::SimpleMath::Vector3& GetPosition() { return mPosition; }
	DirectX::SimpleMath::Vector3& GetScale() { return mScale; }
	DirectX::SimpleMath::Vector3& GetRotation() { return mRotation; }
	//build a matrix from the euler angles, pos and scale
	//note that this hard codes a concatenation order that you might not want
	void GetWorldMatrix(DirectX::SimpleMath::Matrix& w);
	//get the mesh this GameObject is using
	Mesh& GetMesh() {
		assert(mpMesh);
		return *mpMesh;
	}
	//has this GameObject been configured to use a custom material
	Material* HasOverrideMat() {
		if (mUseOverrideMat)
			return &mOverrideMaterial;
		return nullptr;
	}
	//tell the GameObject to use a custom material (take a copy of it) or stop using one
	void SetOverrideMat(Material* pMat = nullptr) {
		if (!pMat) {
			mUseOverrideMat = false;
			return;
		}
		mUseOverrideMat = true;
		mOverrideMaterial = *pMat;
	}
	//copy a GameObject
	GameObject& operator=(const GameObject& gameObject)
	{
		mpMesh = gameObject.mpMesh;
		mPosition = gameObject.mPosition;
		mScale = gameObject.mScale;
		mRotation = gameObject.mRotation;
		return *this;
	}
private:

	Mesh* mpMesh = nullptr;		//the mesh we are using
	DirectX::SimpleMath::Vector3 mPosition, mScale, mRotation;	//positon, scale and orientation
	Material mOverrideMaterial;		//an alternate material to the one in the Mesh
	bool mUseOverrideMat = false;	//should we actually be using it?
};

#endif

