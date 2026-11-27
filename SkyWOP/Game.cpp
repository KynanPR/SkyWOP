#include "WindowUtils.h"
#include "D3D.h"
#include "Game.h"
#include "GeometryBuilder.h"
#include "Player.hpp"

/*quick way to configure a GameObject
* gameObject - IN/OUT GameObject
* source - IN mesh to use
* scale/pos/rot - IN
*/

void Game::Load()
{
	using namespace DirectX::SimpleMath;
	MyD3D& d3d = WinUtil::Get().GetD3D();

	//get a quad and cube mesh ready
	Mesh& quadMesh = BuildQuad(d3d.GetMeshMgr());
	Mesh& cubeMesh = BuildCube(d3d.GetMeshMgr());

	// Player Load. Ensure player is at first
	mGameObjects.emplace(mGameObjects.begin(), Player());

	// Setup some test walls for spacial reference
	//back wall
	GameObject& backWall = mGameObjects.emplace_back();
	backWall.SetUp(quadMesh, Vector3(0, 0.5f, 3), Vector3(3, 1, 1.5f), Vector3(-PI / 2, 0, 0));
	Material mat = backWall.GetMesh().GetSubMesh(0).material;
	mat.gfxData.Set(Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 1));
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "wall.dds");
	mat.texture = "wall.dds";
	backWall.SetOverrideMat(mat);

	//left wall
	GameObject& leftWall = mGameObjects.emplace_back();
	leftWall.SetUp(quadMesh, Vector3(-2, 0.5f, 0), Vector3(3, 1, 1.5f), Vector3(-PI / 2, -PI / 2, 0));
	leftWall.SetOverrideMat(mat);

	//the sun
	d3d.GetFX().SetupDirectionalLight(0, true, Vector3(-0.7f, -0.7f, 0.7f), Vector3(0.47f, 0.47f, 0.47f), Vector3(0.15f, 0.15f, 0.15f), Vector3(0.25f, 0.25f, 0.25f));
}

void Game::Initialise()
{
	//create objects for font rendering
	MyD3D& d3d = WinUtil::Get().GetD3D();
	mpFontBatch = new DirectX::SpriteBatch(&d3d.GetDeviceCtx());
	assert(mpFontBatch);
	mpFont = new DirectX::SpriteFont(&d3d.GetDevice(), L"../bin/data/fonts/algerian.spritefont");
	assert(mpFont);

	Load();
}

void Game::Release()
{
	//tidy up
	delete mpFontBatch;
	mpFontBatch = nullptr;
	delete mpFont;
	mpFont = nullptr;
}

void Game::Update(float dTime)
{
	//spin things around
	mAngle += dTime * 0.5f;
}

void Game::Render(float dTime)
{
	RenderGame(dTime); //separate it out? assuming other things will be added
}

void Game::RenderGame(float dTime)
{
	using namespace DirectX::SimpleMath;
	MyD3D& d3d = WinUtil::Get().GetD3D();
	d3d.BeginRender(Colours::Black);

	SetUpCamera(d3d);

	//render all GameObjects
	for (auto& gameObject : mGameObjects)
		d3d.GetFX().Render(gameObject);

	d3d.EndRender();
}

inline void Game::SetUpCamera(MyD3D& d3d)
{
	//setup camera and projections
	// View
	d3d.GetFX().SetPerFrameConsts(d3d.GetDeviceCtx(), mCamPos);
	constexpr DirectX::SimpleMath::Vector3 camTarget = { 0, 0, 0 }; // Looking at origin
	constexpr DirectX::SimpleMath::Vector3 camUp = { 0, 0, 1 }; // +X to the right. +Z up. Relitive to screen
	CreateViewMatrix(d3d.GetFX().GetViewMatrix(), mCamPos, camTarget, camUp);

	// Projection
	constexpr float fov = 0.25f * PI;
	const float aspectRatio = WinUtil::Get().GetAspectRatio();
	constexpr float nearClip = 1.f;
	constexpr float farClip = 1000.f;
	CreateProjectionMatrix(d3d.GetFX().GetProjectionMatrix(), fov, aspectRatio, nearClip, farClip);

}

//push the camera around and check for exit
LRESULT Game::WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	const float camInc = 200.f * GetElapsedSec();
	//do something game specific here
	switch (msg)
	{
		// Respond to a keyboard event.
	case WM_CHAR:
		switch (wParam)
		{
		case 27:
		case 'q':
		case 'Q':
			PostQuitMessage(0);
			return 0;
		case 'a':
			mCamPos.y += camInc;
			break;
		case 'z':
			mCamPos.y -= camInc;
			break;
		case 'd':
			mCamPos.x -= camInc;
			break;
		case 'f':
			mCamPos.x += camInc;
			break;
		case 'w':
			mCamPos.z += camInc;
			break;
		case 's':
			mCamPos.z -= camInc;
			break;
		case ' ':
			mCamPos = mDefCamPos;
			break;
		}
	}
	//default message handling (resize window, full screen, etc)
	return WinUtil::Get().DefaultMssgHandler(hwnd, msg, wParam, lParam);
}

