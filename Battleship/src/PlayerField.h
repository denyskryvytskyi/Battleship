#ifndef _PLAYER_FIELD_H
#define _PLAYER_FIELD_H

#include <SFML/Graphics.hpp>
#include "Math.h"

enum class ECellState
{
    Deck,
    DeckBuilding,
    DeckHitted,
    DeckKilled,
    Free,
    Blocked,
};

struct Cell
{
    sf::RectangleShape mShape;
    ECellState mState;

    Cell() :
        mShape(),
        mState(ECellState::Free)
    {}

    Cell(const sf::RectangleShape shape, ECellState state) :
        mShape(shape),
        mState(state)
    {}
};

class PlayerField {
private:
    const sf::Vector2f mCellSize;

public:
    Cell** mCells;

public:
    void Initialize();
    //
    PlayerField();
    ~PlayerField();
    //
    void ResetColor();
};

#endif // !_PLAYER_FIELD_H