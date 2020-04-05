#ifndef _MAP_H
#define _MAP_H

#include <SFML/Graphics.hpp>
#include "Math.h"

enum class ECellState
{
    Deck,
    DeckBuilding,
    DeckHitted,
    DeckDestroyed,
    Free,
    Blocked,
    Missed
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
    
    Cell(const sf::RectangleShape shape) :
        mShape(shape),
        mState(ECellState::Free)
    {}
};

class Map {
private:
    const sf::Vector2f mCellSize;

public:
    math::Matrix<Cell>* mCells;

public:
    Map();
    //
    void Initialize();
    void Reset();
};

#endif // !_MAP_H