#ifndef _PLAYER_H
#define _PLAYER_H

#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Config.h"

enum EShipType
{
    EShipType_FourDecks,
    EShipType_ThreeDecks,
    EShipType_TwoDecks,
    EShipType_OneDeck,
    EShipType_Count,
};

enum class EPlayerState
{
    Fire,
    Placing,
    Ready,
    Wait,
    Dead
};

enum class EDirection
{
    None, // for single deck ship
    Horizontal,
    Vertical,
};

struct PlacingShip
{
private:
    int mAvailableDecksCount;

public:
    EShipType mShipType;
    EDirection mDirection;

public:
    PlacingShip();
    PlacingShip(const EShipType shipType);

    inline void AddDeck() { --mAvailableDecksCount; }
    inline bool IsReady() { return mAvailableDecksCount == 0 ? true : false; }

    inline void ResetDecksCount() { mAvailableDecksCount = EShipType_Count - mShipType; }
    void Reset();
};

class Player
{
    typedef std::map<EShipType, int> ShipTypes;

protected:
    EPlayerState mState;
    Map mMap;
    //
    ShipTypes mAvailableShips;
    PlacingShip mCurrentPlacingShip;
    //
    // Decks positions of current processed ship (players)
    std::vector<sf::Vector2i> mCurrentShipDecksPositions;
    //
    int mAliveDecksCounts = 0;
    //
    bool mIsAI = false;
    sf::Font font;

public:
    unsigned int mId;
    sf::View mCamera;
    sf::Text mName;

protected:
    bool CheckGranted(sf::Vector2i pos, EDirection& direction) const;
    void BuildDeck(const sf::Vector2i pos);
    void FinishShipBuilding();
    void TryBlockCell(sf::Vector2i pos);
    bool TryFire(sf::Vector2i pos);
    bool TryDestroy(const sf::Vector2i& pos);
    bool CheckCell(const sf::Vector2i& pos, sf::Vector2i offset);
    void SetDeckState(const sf::Vector2i& pos, const ECellState& cellState);
    void DestroyShip();

public:
    Player(const unsigned int id, const sf::FloatRect& viewport, const sf::FloatRect& size, std::string name, EPlayerState state, bool isAi);
    //
    void Update();
    virtual void AddDeck(const sf::Vector2i& pos) = 0;
    bool Fire(const sf::Vector2i pos);
    bool GetNextShipTemplate(PlacingShip& currentShip);
    //
    inline sf::RectangleShape GetCell(unsigned row, unsigned col) const
    {
        return mMap.mCells[row][col].mShape;
    }
    inline EPlayerState GetCurrentState() const { return mState; }
    inline void SetCurrentState(const EPlayerState state) { mState = state; }
    //
    void HideShips();
};

typedef Player* PlayerPtr;

#endif // !_PLAYER_H