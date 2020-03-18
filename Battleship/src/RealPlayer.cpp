#include "Player.h"
#include "RealPlayer.h"

RealPlayer::RealPlayer(const unsigned int id, const sf::FloatRect& viewport, const sf::FloatRect& size, const std::string name, const EPlayerState state, const bool isAi) :
    Player(id, viewport, size, name, state, isAi)
{
}

void RealPlayer::AddDeck(const sf::Vector2i& pos)
{
    for (int i = 0; i < cfg::field_rows; i++)
    {
        for (int j = 0; j < cfg::field_cols; j++)
        {
            Cell* cell = &mMap.mCells[i][j];
            if (cell->mShape.getGlobalBounds().contains(pos.x, pos.y))
            {
                if (cell->mState == ECellState::Free)
                {
                    TryAddDeck(sf::Vector2i(i, j));
                }
                return;
            }
        }
    }
}

bool RealPlayer::TryAddDeck(sf::Vector2i pos)
{
    bool isNewShip = mCurrentPlacingShip.IsReady();

    if (isNewShip)
    {
        if (!GetNextShipTemplate(mCurrentPlacingShip))
        {
            return false;
        }
    }

    if (!isNewShip)
    {
        EDirection direction;

        if (!CheckGranted(pos, direction))
        {
            return false;
        }
        if (mCurrentPlacingShip.mDirection == EDirection::None)
        {
            mCurrentPlacingShip.mDirection = direction;
        }
    }

    BuildDeck(pos);

    if (mCurrentPlacingShip.IsReady())
        FinishShipBuilding();

    return true;
}