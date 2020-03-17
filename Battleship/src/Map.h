#ifndef _MAP_H
#define _MAP_H

#include <SFML/Graphics.hpp>

enum class ECellState
{
    Deck,
    DeckBuilding,
    DeckHitted,
    DeckKilled,
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
};

class Map {
private:
    const sf::Vector2f mCellSize;

public:
    Cell** mCells;

public:
    void Initialize();
    //
    Map();
    ~Map();
    //
    void ResetColor();
};

#endif // !_MAP_H