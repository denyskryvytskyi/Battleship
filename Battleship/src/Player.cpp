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

bool Player::CheckGranted(sf::Vector2i pos, EDirection& direction) const
{
    EDirection currentDir = mCurrentPlacingShip.mDirection;

    if (pos.x - 1 >= 0 && mMap.mCells->at(pos.x - 1, pos.y).mState == ECellState::DeckBuilding && currentDir != EDirection::Vertical)
    {
        direction = EDirection::Horizontal;
        return true;
    }
    if (pos.x + 1 < cfg::field_cols && mMap.mCells->at(pos.x + 1, pos.y).mState == ECellState::DeckBuilding && currentDir != EDirection::Vertical)
    {
        direction = EDirection::Horizontal;
        return true;
    }
    if (pos.y - 1 >= 0 && mMap.mCells->at(pos.x, pos.y - 1).mState == ECellState::DeckBuilding && currentDir != EDirection::Horizontal)
    {
        direction = EDirection::Vertical;
        return true;
    }
    if (pos.y + 1 < cfg::field_rows && mMap.mCells->at(pos.x, pos.y + 1).mState == ECellState::DeckBuilding && currentDir != EDirection::Horizontal)
    {
        direction = EDirection::Vertical;
        return true;
    }


    return false;
}

void Player::BuildDeck(const sf::Vector2i pos)
{
    SetDeckState(pos, ECellState::DeckBuilding);

    mCurrentPlacingShip.AddDeck();
    mCurrentShipDecksPositions.push_back(pos);
    ++mAliveDecksCounts;
}

void Player::FinishShipBuilding()
{
    for (sf::Vector2i pos : mCurrentShipDecksPositions)
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
    mCurrentShipDecksPositions.clear();
}

void Player::TryBlockCell(sf::Vector2i pos)
{
    if (pos.x >= 0 && pos.x < cfg::field_cols && pos.y >= 0 && pos.y < cfg::field_rows)
    {
        if (mMap.mCells->at(pos.x, pos.y).mState != ECellState::DeckBuilding && mMap.mCells->at(pos.x, pos.y).mState != ECellState::Deck)
        {
            mMap.mCells->at(pos.x, pos.y).mState = ECellState::Blocked;
        }
    }
}

bool Player::TryFire(sf::Vector2i pos)
{
    ECellState state = mMap.mCells->at(pos.x, pos.y).mState;

    if (state == ECellState::Deck)
    {
        --mAliveDecksCounts;

        if (TryDestroy(pos))
        {
            mCurrentShipDecksPositions.push_back(pos);
            DestroyShip();
        }
        else
        {
            mCurrentShipDecksPositions.clear();
            SetDeckState(pos, ECellState::DeckHitted);
        }
    }
    else if (state == ECellState::Free)
    {
        SetDeckState(pos, ECellState::Missed);
        return false;
    }

    return true;

}

bool Player::TryDestroy(const sf::Vector2i& pos)
{
    if (!CheckCell(pos, sf::Vector2i(1, 0)))
    {
        return false;
    }
    if (!CheckCell(pos, sf::Vector2i(-1, 0)))
    {
        return false;
    }
    if (!CheckCell(pos, sf::Vector2i(0, 1)))
    {
        return false;
    }
    if (!CheckCell(pos, sf::Vector2i(0, -1)))
    {
        return false;
    }

    return true;
}

bool Player::CheckCell(const sf::Vector2i& pos, sf::Vector2i offset)
{
    sf::Vector2i checkPos = pos + offset;

    if (checkPos.x < 0 || checkPos.x >= cfg::field_cols || checkPos.y < 0 || checkPos.y >= cfg::field_rows)
        return true;

    Cell* cell = &mMap.mCells->at(checkPos.x, checkPos.y);

    if (cell->mState == ECellState::Free || cell->mState == ECellState::Missed)
    {
        return true;
    }
    if (cell->mState == ECellState::DeckHitted)
    {
        mCurrentShipDecksPositions.push_back(checkPos);
        return CheckCell(checkPos, offset);
    }

    return false;
}

void Player::SetDeckState(const sf::Vector2i& pos, const ECellState& cellState)
{
    Cell* cell = &mMap.mCells->at(pos.x, pos.y);
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
    case ECellState::DeckDestroyed:
        deckColor = color::Destroyed;
        break;
    case ECellState::Missed:
        deckColor = color::Missed;
        break;
    }
    cell->mShape.setFillColor(deckColor);
}

void Player::DestroyShip()
{
    for (sf::Vector2i pos : mCurrentShipDecksPositions)
    {
        SetDeckState(pos, ECellState::DeckDestroyed);
    }

    mCurrentShipDecksPositions.clear();
}

bool Player::Fire(const sf::Vector2i pos)
{
    for (int i = 0; i < cfg::field_rows; i++)
    {
        for (int j = 0; j < cfg::field_cols; j++)
        {
            if (mMap.mCells->at(i, j).mShape.getGlobalBounds().contains(pos.x, pos.y))
            {
                if (TryFire(sf::Vector2i(i, j)))
                {
                    return true;
                }
                mState = EPlayerState::Wait;
            }
        }
    }
}

void Player::Update()
{
    if (mAliveDecksCounts == 0 && mState != EPlayerState::Placing)
    {
        mState = EPlayerState::Dead;
    }
    else if (mAliveDecksCounts == cfg::max_ship_decks_count && mState == EPlayerState::Placing)
    {
        HideShips();
        mState = EPlayerState::Ready;
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
    mMap.Reset();
}

PlacingShip::PlacingShip() :
    mAvailableDecksCount(0),
    mShipType(),
    mDirection(EDirection::None)
{
}

PlacingShip::PlacingShip(const EShipType shipType) :
    mShipType(shipType),
    mDirection(EDirection::None)
{
    mAvailableDecksCount = EShipType_Count - mShipType;
}

void PlacingShip::Reset()
{
    ResetDecksCount();
    mDirection = EDirection::None;
}
