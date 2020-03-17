#ifndef _PLAYER_FACTORY_H
#define _PLAYER_FACTORY_H

#include "Player.h"

class PlayerFactory
{
public:
    static PlayerPtr Create(const unsigned int id, const sf::FloatRect& viewport, const sf::FloatRect& size, const std::string name, const EPlayerState state = EPlayerState::Wait, const bool isAi = false);
};


#endif // !_PLAYER_FACTORY_H
