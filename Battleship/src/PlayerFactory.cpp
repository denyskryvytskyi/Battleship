#include "PlayerFactory.h"
#include "RealPlayer.h"

PlayerPtr PlayerFactory::Create(const unsigned int id, const sf::FloatRect& viewport, const sf::FloatRect& size, const std::string name, const EPlayerState state /*EState_Wait*/, const bool isAi /*false*/)
{
    /*if (isAi)
    {
        return new AiPlayer();
    }*/
   
    return new RealPlayer(id, viewport, size, name, state, isAi);
}
