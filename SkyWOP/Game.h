#ifndef GAME_H
#define GAME_H

#include <future>
#include "SpriteBatch.h"
#include "SpriteFont.h"

#include "Mesh.h"
#include "GameObject.h"
#include "singleton.h"


class Game : public Singleton<Game>
{
public:
	//standard stuff
	Game() {}
	~Game() {
		Release();
	}
	void Update(float dTime);
	void Render(float dTime);
	void Initialise();
	void Release();
	LRESULT WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//a camera we can push around the scene
	const DirectX::SimpleMath::Vector3 mDefCamPos = { 0, 10, 0 };
	DirectX::SimpleMath::Vector3 mCamPos = mDefCamPos;

	//stick all GameObjects in a vector
	std::vector<GameObject> mGameObjects;

private:

	//spin some GameObjects around
	float mAngle = 0;
	//printing text
	DirectX::SpriteBatch* mpFontBatch = nullptr;
	DirectX::SpriteFont* mpFont = nullptr;

	//load all GameObjects
	void Load();
	//separate out in-game rendering
	void RenderGame(float dTime);

	// Set camera matricies
	inline void SetUpCamera(MyD3D& d3d);

};

#endif
