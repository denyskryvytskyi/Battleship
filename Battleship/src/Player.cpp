#include "Player.h"
#include "CommonTypes.h"
#include "Config.h"

Player::Player(const unsigned int id, const sf::FloatRect& viewport, const sf::FloatRect& size, const std::string name, const EPlayerState state, const bool isAi) :
    mId(id),
    mState(state),
    mIsAI(isAi)
{
    mCamera.setViewport(viewport);
    mCamera.reset(size);

    mName.setString(name);

    mMap.Initialize();

    font.loadFromFile("arial.ttf");

    mName.setFont(font);
    mName.setFillColor(color::Water);
    mName.setCharacterSize(24);

    for (int i = 0; i < EShipType_Count; i++)
    {
        mAvailableShips[(EShipType)i] = (EShipType)i + 1;
    }
}

bool Player::CheckGranted(sf::Vector2i pos, EBuildingDirection& direction) const
{
    EBuildingDirection currentDir = mCurrentPlacingShip.mDirection;

    if (pos.x - 1 >= 0 && mMap.mCells[pos.x - 1][pos.y].mState == ECellState::DeckBuilding && currentDir != EBuildingDirection::Vertical)
    {
        direction = EBuildingDirection::Horizontal;
        return true;
    }
    if (pos.x + 1 < cfg::field_cols && mMap.mCells[pos.x + 1][pos.y].mState == ECellState::DeckBuilding && currentDir != EBuildingDirection::Vertical)
    {
        direction = EBuildingDirection::Horizontal;
        return true;
    }
    if (pos.y - 1 >= 0 && mMap.mCells[pos.x][pos.y - 1].mState == ECellState::DeckBuilding && currentDir != EBuildingDirection::Horizontal)
    {
        direction = EBuildingDirection::Vertical;
        return true;
    }
    if (pos.y + 1 < cfg::field_rows && mMap.mCells[pos.x][pos.y + 1].mState == ECellState::DeckBuilding && currentDir != EBuildingDirection::Horizontal)
    {
        direction = EBuildingDirection::Vertical;
        return true;
    }


    return false;
}

void Player::BuildDeck(const sf::Vector2i pos)
{
    SetDeckState(pos, ECellState::DeckBuilding);

    mCurrentPlacingShip.AddDeck();
    mBuildingDecksPositions.push_back(pos);
    ++mAliveDecksCounts;
}

void Player::FinishShipBuilding()
{
    for (sf::Vector2i pos : mBuildingDecksPositions)
    {
        SetDeckState(pos, ECellState::Deck);

        // block cells around the ship
        TryBlockCell(sf::Vector2i(pos.x - 1, pos.y));
        TryBlockCell(sf::Vector2i(pos.x + 1, pos.y));
        TryBlockCell(sf::Vector2i(pos.x, pos.y - 1));
        TryBlockCell(sf::Vector2i(pos.x, pos.y + 1));
        TryBlockCell(sf::Vector2i(pos.x - 1, pos.y - 1));
        TryBlockCell(sf::Vector2i(pos.x - 1, pos.y + 1));
        TryBlockCell(sf::Vector2i(pos.x + 1, pos.y - 1));
        TryBlockCell(sf::Vector2i(pos.x + 1, pos.y + 1));
    }

    --mAvailableShips[mCurrentPlacingShip.mShipType];
    mBuildingDecksPositions.clear();
}

void Player::TryBlockCell(sf::Vector2i pos)
{
    if (pos.x >= 0 && pos.x < cfg::field_cols && pos.y >= 0 && pos.y < cfg::field_rows)
    {
        if (mMap.mCells[pos.x][pos.y].mState != ECellState::DeckBuilding && mMap.mCells[pos.x][pos.y].mState != ECellState::Deck)
        {
            mMap.mCells[pos.x][pos.y].mState = ECellState::Blocked;
        }
    }
}

bool Player::TryFire(sf::Vector2i pos)
{
    /*
    1. ѕроверка "если палуба"
    2. ≈сли палуба, нужно посмотреть соседние клетки (если с какой-то стороны есть - ставим также направление)
    3. ≈сли р€дом есть палубы не подбитые - ставим состо€ние палубы по позиции pos - DeckHitted
    4. ≈сли состо€ние соседних примыкающих палуб все DeckHitted - корабль уничтожен

    __ сделать проверку окружающих палуб в рекурсивной функции, также использовать направление, чтобы сократить колличество проверок,
    __ если уже найдена соседн€€ палуба

    ƒалее:
    * ”меньшить кол-во живых палуб у противника
    * ≈сли промах - изменить состо€ние игрока на Wait
    */
    if (mMap.mCells[pos.x][pos.y].mState == ECellState::Deck)// если попали в палубу
    {
        // проверить все соседние: если true - корабль уничтожен, false - подбит
        if (CheckKilled(pos))
        {
            SetDeckState(pos, ECellState::DeckKilled);
        }
        else
        {
            SetDeckState(pos, ECellState::DeckHitted);
        }
    }
    else // мимо
    {
        SetDeckState(pos, ECellState::Missed);
        return false;
    }

    return true;

}

bool Player::CheckKilled(const sf::Vector2i& pos)
{
    // проверить все соседние палубы

    

    return false;
}

void Player::SetDeckState(const sf::Vector2i& pos, const ECellState& cellState)
{
    Cell* cell = &mMap.mCells[pos.x][pos.y];
    cell->mState = cellState;
    
    sf::Color deckColor;
    switch (cellState)
    {
    case ECellState::DeckBuilding:
        deckColor = color::Building;
        break;
    case ECellState::Deck:
        deckColor = color::Ready;
        break;
    case ECellState::DeckHitted:
        deckColor = color::Hitted;
        break;
    case ECellState::DeckKilled:
        deckColor = color::Killed;
        break;
    }
    cell->mShape.setFillColor(deckColor);
}

bool Player::Fire(const sf::Vector2i pos)
{
    for (int i = 0; i < cfg::field_rows; i++)
    {
        for (int j = 0; j < cfg::field_cols; j++)
        {
            if (mMap.mCells[i][j].mShape.getGlobalBounds().contains(pos.x, pos.y))
            {
                if (TryFire(sf::Vector2i(i, j)))
                {
                    return true;
                }
                //если мимо
                mState = EPlayerState::Wait;
                // mEnemy->SetState(EPlayerState::Fire);
            }
        }
    }
}

void Player::Update()
{
    if (mAliveDecksCounts == 0 && mState != EPlayerState::Placing)
    {
        mIsAlive = false;
    }
    else if (mAliveDecksCounts == cfg::max_ship_decks_count && mState == EPlayerState::Placing)
    {
        mState = EPlayerState::PlacingFinished;
    }
}

bool Player::GetNextShipTemplate(PlacingShip& currentShip)
{
    for (int i = 0; i < EShipType_Count; i++)
    {
        if (mAvailableShips[(EShipType)i] != 0)
        {
            currentShip.mShipType = (EShipType)i;
            currentShip.Reset();
            return true;
        }
    }

    return false;
}

void Player::HideShips()
{
    mMap.ResetColor();
}

PlacingShip::PlacingShip() :
    mAvailableDecksCount(0),
    mShipType(),
    mDirection(EBuildingDirection::None)
{
}

PlacingShip::PlacingShip(const EShipType shipType) :
    mShipType(shipType),
    mDirection(EBuildingDirection::None)
{
    mAvailableDecksCount = EShipType_Count - mShipType;
}

void PlacingShip::Reset()
{
    ResetDecksCount();
    mDirection = EBuildingDirection::None;
}
