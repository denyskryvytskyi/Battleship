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
            if (mField.mCells[i][j].mShape.getGlobalBounds().contains(pos.x, pos.y))
            {
                if (mField.mCells[i][j].mState == ECellState::Free)
                {
                    if (TryAddDeck(sf::Vector2i(i, j)))
                    {
                        return;
                    }
                }
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
        EBuildingDirection direction;

        if (!CheckGranted(pos, direction))
        {
            return false;
        }
        if (mCurrentPlacingShip.mDirection == EBuildingDirection::None)
        {
            mCurrentPlacingShip.mDirection = direction;
        }
    }

    BuildDeck(pos);

    if (mCurrentPlacingShip.IsReady())
        FinishShipBuilding();

    return true;
}

void RealPlayer::Fire(const sf::Vector2i pos)
{

}

//
//void Player::setMapPlayerAiColor()
//{
//	for (int i = 0; i < 10; i++)
//		for (int j = 0; j < 10; j++)
//		{
//			m_PlayerMap.m_Map[i][j].setFillColor(Color(16, 16, 68));
//		}
//}

//
//void Player::deckIsDestroyed(int a, int b, Map& enemy)
//{
//	m_EnemyShipsOnMap[a][b] = e_CellConditions::Kill;
//	enemy.m_Map[a][b].setFillColor(Color::Red);
//}
//
//void Player::shotInEnemy(RenderWindow& win, Vector2i p, int movePos, bool& canEnemyShot, Map& enemy)
//{
//	p = Mouse::getPosition(win);
//	p.x -= movePos;
//
//	for (int i = 0; i < 10; i++)
//		for (int j = 0; j < 10; j++)
//		{
//			if (enemy.m_Map[i][j].getGlobalBounds().contains(p.x, p.y)) // if the mouse cursor on the enemy map
//			{
//				m_EnemyX = i; m_EnemyY = j;
//				if (m_EnemyShipsOnMap[i][j] == e_CellConditions::Deck)// if hit the deck
//				{
//					// if there isn't any other decks nearby
//					// then destroy the deck
//					if ((((m_EnemyX - 1) >= 0 && m_EnemyShipsOnMap[m_EnemyX - 1][m_EnemyY] == e_CellConditions::None) || (m_EnemyX - 1) < 0)
//						&& (((m_EnemyX + 1) <= 9 && m_EnemyShipsOnMap[m_EnemyX + 1][m_EnemyY] == e_CellConditions::None) || (m_EnemyX + 1) > 9)
//						&& (((m_EnemyY - 1) >= 0 && m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - 1] == e_CellConditions::None) || (m_EnemyY - 1) < 0) 
//						&& (((m_EnemyY + 1) <= 9 && m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + 1] == e_CellConditions::None) || (m_EnemyY + 1) > 9))
//					{
//						deckIsDestroyed(m_EnemyX, m_EnemyY, enemy);
//						m_OneDeckShip--;
//					}
//					// if there is a deck nearby
//					// then wound the deck
//					else if (((m_EnemyX - 1) >= 0 && m_EnemyShipsOnMap[m_EnemyX - 1][m_EnemyY] == e_CellConditions::Deck) || ((m_EnemyX + 1) <= 9
//						&& m_EnemyShipsOnMap[m_EnemyX + 1][m_EnemyY] == e_CellConditions::Deck) || ((m_EnemyY - 1) >= 0
//							&& m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - 1] == e_CellConditions::Deck) || ((m_EnemyY + 1) <= 9
//								&& m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + 1] == e_CellConditions::Deck))
//					{
//						enemy.m_Map[m_EnemyX][m_EnemyY].setFillColor(Color::Green);
//						m_EnemyShipsOnMap[m_EnemyX][m_EnemyY] = e_CellConditions::Wound;
//					}
//					// if there is a wound deck nearby
//					else if (m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - 1] == e_CellConditions::Wound || m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + 1] == e_CellConditions::Wound)
//					{
//						m_DeckNums = 0;
//						m_DeckWoundNums = 0;
//
//						// if there is existed decks up or down
//						if (m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - 1] == e_CellConditions::Wound)
//						{
//							for (int k = 1; k <= 3; k++)
//								if ((m_EnemyY - k) >= 0 && m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - k] != e_CellConditions::None)
//								{
//									if (m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - k] == e_CellConditions::Deck)
//										m_DeckNums++;
//								}
//								else break;
//						}
//						if (m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + 1] == e_CellConditions::Wound)
//						{
//							for (int k = 1; k <= 3; k++)
//								if ((m_EnemyY + k) <= 9 && m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + k] != e_CellConditions::None)
//								{
//									if (m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + k] == e_CellConditions::Deck)
//										m_DeckNums++;
//								}
//								else break;
//						}
//
//						// if there is existed deck
//						if (m_DeckNums != 0)
//						{
//							enemy.m_Map[m_EnemyX][m_EnemyY].setFillColor(Color::Green);
//							m_EnemyShipsOnMap[m_EnemyX][m_EnemyY] = e_CellConditions::Wound;
//						}
//						// otherwise destroy the ship
//						else if (m_DeckNums == 0)
//						{
//							deckIsDestroyed(m_EnemyX, m_EnemyY, enemy);
//							m_DeckWoundNums = 1;
//
//							// calculate wounded decks by vertical
//							for (int a = 1; a <= 3; a++)
//							{
//								if ((m_EnemyY - a) >= 0 && m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - a] != e_CellConditions::None)
//								{
//									if (m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - a] == e_CellConditions::Wound)
//									{
//										deckIsDestroyed(m_EnemyX, m_EnemyY - a, enemy);
//										m_DeckWoundNums++;
//									}
//								}
//								else break;
//							}
//							for (int a = 1; a <= 3; a++)
//							{
//								if ((m_EnemyY + a) >= 0 && m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + a] != e_CellConditions::None)
//								{
//									if (m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + a] == e_CellConditions::Wound)
//									{
//										deckIsDestroyed(m_EnemyX, m_EnemyY + a, enemy);
//										m_DeckWoundNums++;
//									}
//								}
//								else break;
//							}
//						}
//					}
//					else if (m_EnemyShipsOnMap[m_EnemyX - 1][m_EnemyY] == e_CellConditions::Wound || m_EnemyShipsOnMap[m_EnemyX + 1][m_EnemyY] == e_CellConditions::Wound)
//					{
//						m_DeckNums = 0;
//						m_DeckWoundNums = 0;
//
//						// if there is existed decks above or below
//						if (m_EnemyShipsOnMap[m_EnemyX - 1][m_EnemyY] == e_CellConditions::Wound)
//						{
//							for (int k = 1; k <= 3; k++)
//								if ((m_EnemyX - k) >= 0 && m_EnemyShipsOnMap[m_EnemyX - k][m_EnemyY] != e_CellConditions::None)
//								{
//									if (m_EnemyShipsOnMap[m_EnemyX - k][m_EnemyY] == e_CellConditions::Deck)
//										m_DeckNums++;
//								}
//								else break;
//						}
//						if (m_EnemyShipsOnMap[m_EnemyX + 1][m_EnemyY] == e_CellConditions::Wound)
//						{
//							for (int k = 1; k <= 3; k++)
//								if ((m_EnemyX + k) <= 9 && m_EnemyShipsOnMap[m_EnemyX + k][m_EnemyY] != e_CellConditions::None)
//								{
//									if (m_EnemyShipsOnMap[m_EnemyX + k][m_EnemyY] == e_CellConditions::Deck)
//										m_DeckNums++;
//								}
//								else break;
//						}
//
//						// if there is a existed deck
//						// then hit the deck
//						if (m_DeckNums != 0)
//						{
//							enemy.m_Map[m_EnemyX][m_EnemyY].setFillColor(Color::Green);
//							m_EnemyShipsOnMap[m_EnemyX][m_EnemyY] = e_CellConditions::Wound;
//						}
//						// otherwise - destroy the ship
//						else if (m_DeckNums == 0)
//						{
//							deckIsDestroyed(m_EnemyX, m_EnemyY, enemy);
//							m_DeckWoundNums = 1;
//
//							// calculate number of the wounded decks by vertical
//							for (int a = 1; a <= 3; a++)
//							{
//								if ((m_EnemyX - a) >= 0 && m_EnemyShipsOnMap[m_EnemyX - a][m_EnemyY] != e_CellConditions::None)
//								{
//									if (m_EnemyShipsOnMap[m_EnemyX - a][m_EnemyY] == e_CellConditions::Wound)
//									{
//										deckIsDestroyed(m_EnemyX - a, m_EnemyY, enemy);
//										m_DeckWoundNums++;
//									}
//								}
//								else break;
//							}
//							for (int a = 1; a <= 3; a++)
//							{
//								if ((m_EnemyX + a) >= 0 && m_EnemyShipsOnMap[m_EnemyX + a][m_EnemyY] != e_CellConditions::None)
//								{
//									if (m_EnemyShipsOnMap[m_EnemyX + a][m_EnemyY] == e_CellConditions::Wound)
//									{
//										deckIsDestroyed(m_EnemyX + a, m_EnemyY, enemy);
//										m_DeckWoundNums++;
//									}
//								}
//								else break;
//							}
//						}
//					}
//					m_NumOfEnemyShips = 0;
//					// check if the enemy has ships
//					for (int i = 0; i < 10; i++)
//						for (int j = 0; j < 10; j++)
//							if (m_EnemyShipsOnMap[i][j] == e_CellConditions::Deck)
//								m_NumOfEnemyShips++;
//				}
//				// missing
//				else if (m_EnemyShipsOnMap[i][j] == e_CellConditions::None)
//				{
//					enemy.m_Map[i][j].setFillColor(Color::Magenta);
//					m_CanShot = false;
//					canEnemyShot = true;
//				}
//			}
//		}
//}
