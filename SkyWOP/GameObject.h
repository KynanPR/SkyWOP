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
/// Public Methods ///
public:
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
	
	/// <summary>
	/// Get global position of GameObject
	/// </summary>
	/// <returns>Position of GameObject factoring in position of parent</returns>
	const DirectX::SimpleMath::Vector3 GetPositionGlobal() const;

	/// <summary>
	/// Get local position of GameObject
	/// </summary>
	/// <returns>Position of GameObject relative to its parent</returns>
	const DirectX::SimpleMath::Vector3& GetPosition() const;

	/// <summary>
	/// Get global scale of GameObject
	/// </summary>
	/// <returns>Scale of GameObject factoring in scale of parent</returns>
	const DirectX::SimpleMath::Vector3 GetScaleGlobal() const;

	/// <summary>
	/// Get local scale of GameObject
	/// </summary>
	/// <returns>Scale of GameObject relative to its parent</returns>
	const DirectX::SimpleMath::Vector3& GetScale() const;

	/// <summary>
	/// Get global rotation of GameObject
	/// </summary>
	/// <returns>Rotation of GameObject factoring in rotation of parent</returns>
	const DirectX::SimpleMath::Vector3 GetRotationGlobal() const;

	/// <summary>
	/// Get local rotation of GameObject
	/// </summary>
	/// <returns>Rotation of GameObject relative to its parent</returns>
	const DirectX::SimpleMath::Vector3& GetRotation() const;

	//get the mesh this GameObject is using
	const Mesh& GetMesh() const;

	//build a matrix from the euler angles, pos and scale
	//note that this hard codes a concatenation order that you might not want
	
	/// <summary>
	/// Build matrix from position, scale and rotation. Multiplication is in that order
	/// </summary>
	/// <returns>Global world matrix</returns>
	DirectX::SimpleMath::Matrix GetWorldMatrix() const;

	// Get's whether the override material should be used
	const bool GetUseOverrideMat() const { return mUseOverrideMat; }

	// Get's the override material on GameObject. Errors if no override set
	const Material& GetOverrideMat() const;
	

	
	// Setters

	/// <summary>
	/// Set local position of GameObject
	/// </summary>
	/// <param name="newPosition">New position of GameObject relative to its parent</param>
	void SetPosition(const DirectX::SimpleMath::Vector3& newPosition) { mPosition = newPosition; }

	/// <summary>
	/// Set local scale of GameObject
	/// </summary>
	/// <param name="newScale">New scale of GameObject relative to its parent</param>
	void SetScale(const DirectX::SimpleMath::Vector3& newScale) { mScale = newScale; }

	/// <summary>
	/// Set local rotation of GameObject
	/// </summary>
	/// <param name="newRotation">New rotation of GameObject relative to its parent</param>
	void SetRotation(const DirectX::SimpleMath::Vector3& newRotation) { mRotation = newRotation; }

	/// <summary>
	/// Sets whether the override material should be used. Errors if setting to true with no override material set
	/// </summary>
	/// <param name="newOverrideMat">Reference to the new override material</param>
	/// <param name="useOverrideMat">Whether this new material should be used. Default true</param>
	void SetUseOverrideMat(bool newUseState);

	void SetOverrideMat(const Material& newOverrideMat, const bool useOverrideMat = true);

/// Private Methods ///
private:

/// Public Data ///
public:

/// Private Data ///
private:
	/// <summary>
	/// Parent GameObject. position, scale and rotation are relative to parent
	/// </summary>
	const GameObject* mpParent = nullptr;
	bool mHasParent = false;
	const Mesh* mpMesh = nullptr;		//the mesh we are using
	DirectX::SimpleMath::Vector3 mPosition, mScale, mRotation;	//positon, scale and orientation
	bool mhasOverrideMaterial = false; // flag for whether we have a non-default override material
	Material mOverrideMaterial;		//an alternate material to the one in the Mesh
	bool mUseOverrideMat = false;	//should we actually be using it?
};

#endif

