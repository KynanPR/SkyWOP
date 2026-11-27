#pragma once
#include "GameObject.h"
#include "Player.hpp"

class PlayerLimb :
    public GameObject
{
/// Public Methods ///
public:
    // No default constructor as we need the parent Player
    PlayerLimb() = delete;

    PlayerLimb(const Player& parentPlayer);

/// Private Methods ///
private:
/// Public Data ///
public:
/// Private Data ///
private:


};

