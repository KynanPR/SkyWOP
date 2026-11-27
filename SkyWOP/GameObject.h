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
	/// Public Methods ///
	// Copy a GameObject
	GameObject& operator=(const GameObject& gameObject)
	{
		mpMesh = gameObject.mpMesh;
		mPosition = gameObject.mPosition;
		mScale = gameObject.mScale;
		mRotation = gameObject.mRotation;
		return *this;
	}
	
	//go find the mesh with this name and setup
	void Initialise(const std::string& meshFileName);
	// Init and set position, rotation and scale
	void SetUp(const std::string& meshFileName, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation);
	// Set up with constant scale
	void SetUp(const std::string& meshFileName, const DirectX::SimpleMath::Vector3 position, const float scale, const DirectX::SimpleMath::Vector3& rotation);

	//setup using the given mesh
	void Initialise(const Mesh& mesh);
	// Init and set position, rotation and scale
	void SetUp(const Mesh& mesh, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation);
	// Set up with constant scale
	void SetUp(const Mesh& mesh, const DirectX::SimpleMath::Vector3& position, const float scale, const DirectX::SimpleMath::Vector3& rotation);

	// Getters
	const DirectX::SimpleMath::Vector3& GetPosition() const { return mPosition; }
	const DirectX::SimpleMath::Vector3& GetScale() const { return mScale; }
	const DirectX::SimpleMath::Vector3& GetRotation() const { return mRotation; }
	//get the mesh this GameObject is using
	const Mesh& GetMesh() const;
	//build a matrix from the euler angles, pos and scale
	//note that this hard codes a concatenation order that you might not want
	DirectX::SimpleMath::Matrix GetWorldMatrix() const;
	// Get's whether the override material should be used
	const bool GetUseOverrideMat() const { return mUseOverrideMat; }
	// Get's the override material on GameObject. Errors if no override set
	const Material& GetOverrideMat() const;

	
	// Setters
	void SetPosition(const DirectX::SimpleMath::Vector3& newPosition) { mPosition = newPosition; }
	void SetScale(const DirectX::SimpleMath::Vector3& newScale) { mScale = newScale; }
	void SetRotation(const DirectX::SimpleMath::Vector3& newRotation) { mRotation = newRotation; }
	/// <summary>
	/// Set the world matrix with individual position, scale and rotation values
	/// </summary>
	void SetWorldMatrix(const DirectX::SimpleMath::Vector3& newPosition, const DirectX::SimpleMath::Vector3& newScale, const DirectX::SimpleMath::Vector3& newRotation);
	/// <summary>
	/// Sets whether the override material should be used. Errors if setting to true with no override material set
	/// </summary>
	/// <param name="newOverrideMat">Reference to the new override material</param>
	/// <param name="useOverrideMat">Whether this new material should be used. Default true</param>
	void SetUseOverrideMat(bool newUseState);
	void SetOverrideMat(const Material& newOverrideMat, const bool useOverrideMat = true);

private:
	/// Private Methods ///


public:
	/// Public Data ///

private:
	/// Private Data ///

	const Mesh* mpMesh = nullptr;		//the mesh we are using
	DirectX::SimpleMath::Vector3 mPosition, mScale, mRotation;	//positon, scale and orientation
	bool mhasOverrideMaterial = false; // flag for whether we have a non-default override material
	Material mOverrideMaterial;		//an alternate material to the one in the Mesh
	bool mUseOverrideMat = false;	//should we actually be using it?
};

#endif

