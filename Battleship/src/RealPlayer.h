#ifndef _REAL_PLAYER_H
#define _REAL_PLAYER_H

#include "Player.h"

class RealPlayer : public Player {

private:
    bool TryAddDeck(sf::Vector2i pos);

public:
    RealPlayer(const unsigned int id, const sf::FloatRect& viewport, const sf::FloatRect& size, const std::string name, const EPlayerState state, const bool isAi);
    //
    void AddDeck(const sf::Vector2i& pos) override;
    //
    void Fire(const sf::Vector2i pos) override;
    //
};

#endif // !_REAL_PLAYER_H