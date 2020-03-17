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

    mField.Initialize();

    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        // font wasn't find
    }

    mName.setFont(font);
    mName.setFillColor(CellColor::Water);
    mName.setCharacterSize(24);

    for (int i = 0; i < EShipType_Count; i++)
    {
        mAvailableShips[(EShipType)i] = (EShipType) i + 1;
    }
}

bool Player::CheckGranted(sf::Vector2i pos, EBuildingDirection& direction) const
{
    EBuildingDirection currentDir = mCurrentPlacingShip.mDirection;

    if (pos.x - 1 >= 0 && mField.mCells[pos.x - 1][pos.y].mState == ECellState::DeckBuilding && currentDir != EBuildingDirection::Vertical)
    {
        direction = EBuildingDirection::Horizontal;
        return true;
    }
    if (pos.x + 1 < cfg::field_cols && mField.mCells[pos.x + 1][pos.y].mState == ECellState::DeckBuilding && currentDir != EBuildingDirection::Vertical)
    {
        direction = EBuildingDirection::Horizontal;
        return true;
    }
    if (pos.y - 1 >= 0 && mField.mCells[pos.x][pos.y - 1].mState == ECellState::DeckBuilding && currentDir != EBuildingDirection::Horizontal)
    {
        direction = EBuildingDirection::Vertical;
        return true;
    }
    if (pos.y + 1 < cfg::field_rows && mField.mCells[pos.x][pos.y + 1].mState == ECellState::DeckBuilding && currentDir != EBuildingDirection::Horizontal)
    {
        direction = EBuildingDirection::Vertical;
        return true;
    }


    return false;
}

void Player::BuildDeck(const sf::Vector2i pos)
{
    Cell* cell = &mField.mCells[pos.x][pos.y];
    cell->mState = ECellState::DeckBuilding;
    cell->mShape.setFillColor(CellColor::Building);

    mCurrentPlacingShip.AddDeck();
    mBuildingDecksPositions.push_back(pos);
    ++mAliveDecksCounts;
}

void Player::FinishShipBuilding()
{
    for (sf::Vector2i pos : mBuildingDecksPositions)
    {
        Cell* cell = &mField.mCells[pos.x][pos.y];
        cell->mState = ECellState::Deck;
        cell->mShape.setFillColor(CellColor::Ready);

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
        if (mField.mCells[pos.x][pos.y].mState != ECellState::DeckBuilding && mField.mCells[pos.x][pos.y].mState != ECellState::Deck)
        {
            mField.mCells[pos.x][pos.y].mState = ECellState::Blocked;
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
    mField.ResetColor();
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
