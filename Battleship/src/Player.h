#ifndef _PLAYER_H
#define _PLAYER_H

#include <SFML/Graphics.hpp>
#include "PlayerField.h"
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
    PlacingFinished,
    Wait,
};

enum class EBuildingDirection
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
    EBuildingDirection mDirection;

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
    //
    PlayerField mField; // карта игрока
    //
    ShipTypes mAvailableShips; // массив кол-ва доступных кораблей каждого типа
    PlacingShip mCurrentPlacingShip; // корабль, который сейчас строится игроком
    std::vector<sf::Vector2i> mBuildingDecksPositions; // вектор позиций палуб строящегося корабля
    int mAliveDecksCounts = 0; // кол-во целых палуб
    //
    bool mIsAlive = false; // mAliveDecksCounts != 0
    bool mIsAI = false;

public:
    unsigned int mId;
    sf::View mCamera;
    sf::Text mName;

protected:
    bool CheckGranted(sf::Vector2i pos, EBuildingDirection& direction) const;
    void BuildDeck(const sf::Vector2i pos);
    void FinishShipBuilding();
    void TryBlockCell(sf::Vector2i pos);

public:
    Player(const unsigned int id, const sf::FloatRect& viewport, const sf::FloatRect& size, std::string name, EPlayerState state, bool isAi);
    //
    virtual void AddDeck(const sf::Vector2i& pos) = 0;
    virtual void Fire(const sf::Vector2i pos) = 0;
    //
    void Update();
    //
    bool GetNextShipTemplate(PlacingShip& currentShip);
    //
    inline sf::RectangleShape GetCell(unsigned row, unsigned col) const
    {
        return mField.mCells[row][col].mShape;
    }
    //
    inline EPlayerState GetCurrentState() const { return mState; }
    inline void SetCurrentState(const EPlayerState state) { mState = state; }
    //
    void HideShips();
};

typedef Player* PlayerPtr;

#endif // !_PLAYER_H