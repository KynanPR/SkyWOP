#include "PlayerLimb.hpp"

PlayerLimb::PlayerLimb(const Player& parentPlayer) :
	GameObject()	
{
	GameObject::SetParent(parentPlayer);
}
