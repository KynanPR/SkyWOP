#pragma once
#include "GameObject.h"

//constexpr char* playerMeshFile = "player";

/// <summary>
/// Player class that handles the in game player
/// </summary>
class Player :
    public GameObject
{
/// Public Methods ///
public:
    Player();

    // Should only ever have one player so delete copy constructors
    Player (const Player&) = delete;
    Player& operator=(const Player&) = delete;
/// Private Methods ///
private:

/// Public Data ///
public:

/// Private Data ///
private:
};

