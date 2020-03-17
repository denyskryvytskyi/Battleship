//#include "PlayerAI.h"
//
//void PlayerAI::addShipsOnMap()
//{
//	srand(time(NULL));
//
//	while (m_MaxNumberOfDecks > 0)
//	{
//		m_EnemyX = rand() % 9;
//		m_EnemyY = rand() % 9;
//
//		if (m_PlayerShipsOnMap[m_EnemyX][m_EnemyY] == e_CellConditions::None && m_CanPut[m_EnemyX][m_EnemyY] == true)
//		{
//			// Placed ships
//			if (m_FourDeckShip != 0)
//			{
//				initShipOnMap(m_EnemyX, m_EnemyY, 4);
//			}
//			else if (m_ThreeDeckShip != 0 && m_FourDeckShip == 0)
//			{
//				if (m_NumberOfTheDeck == 4) m_NumberOfTheDeck = 1;
//				initShipOnMap(m_EnemyX, m_EnemyY, 3);
//			}
//			else if (m_TwoDeckShip != 0 && m_ThreeDeckShip == 0)
//			{
//				if (m_NumberOfTheDeck == 3) m_NumberOfTheDeck = 1;
//				initShipOnMap(m_EnemyX, m_EnemyY, 2);
//			}
//			else if (m_OneDeckShip != 0 && m_TwoDeckShip == 0)
//			{
//				deckIsReady(m_EnemyX, m_EnemyY);
//				m_PlayerShipsOnMap[m_EnemyX][m_EnemyY] = e_CellConditions::Deck;
//
//				if ((m_EnemyX - 1) >= 0) m_CanPut[m_EnemyX - 1][m_EnemyY] = false;
//				if ((m_EnemyX + 1) <= 9) m_CanPut[m_EnemyX + 1][m_EnemyY] = false;
//				if ((m_EnemyY - 1) >= 0) m_CanPut[m_EnemyX][m_EnemyY - 1] = false;
//				if ((m_EnemyY + 1) <= 9) m_CanPut[m_EnemyX][m_EnemyY + 1] = false;
//			}
//		}
//	}
//}
//void PlayerAI::randXY()
//{
//	srand(time(NULL));
//	m_EnemyX = rand() % 9;
//	m_EnemyY = rand() % 9;
//}
//
//void PlayerAI::shotInEnemy(RenderWindow& win, Vector2i p, bool& canEnemyShot, Map& enemy)
//{
//
//	if (m_FireState == e_FireOnEnemy::defaultRand) // last shoot - missing and there is not wounded decks
//	{
//		do {
//			randXY();
//		} while (m_CanShotOnMap[m_EnemyX][m_EnemyY] != true);
//
//	}
//
//	// if there is wounded decks
//	if (m_FireState == e_FireOnEnemy::thereAreWoundDeck || m_FireState == e_FireOnEnemy::hitLastShot)
//	{
//		for (int i = 0; i < 10; i++)
//			for (int j = 0; j < 10; j++)
//			{
//				if (m_EnemyShipsOnMap[i][j] == e_CellConditions::Wound && (m_CanShotOnMap[i - 1][j] || m_CanShotOnMap[i + 1][j]
//					|| m_CanShotOnMap[i][j - 1] || m_CanShotOnMap[i][j + 1]))
//				{
//
//					if (m_EnemyShipsOnMap[i - 1][j] != e_CellConditions::Wound && m_CanShotOnMap[i - 1][j])
//					{
//						m_EnemyX = i - 1;
//						m_EnemyY = j;
//						break;
//					}
//					else if (m_EnemyShipsOnMap[i + 1][j] != e_CellConditions::Wound && m_CanShotOnMap[i + 1][j])
//					{
//						m_EnemyX = i + 1;
//						m_EnemyY = j;
//						break;
//					}
//					else if (m_EnemyShipsOnMap[i][j - 1] != e_CellConditions::Wound && m_CanShotOnMap[i][j - 1])
//					{
//						m_EnemyY = j - 1;
//						m_EnemyX = i;
//						break;
//					}
//					else if (m_EnemyShipsOnMap[i][j + 1] != e_CellConditions::Wound && m_CanShotOnMap[i][j + 1])
//					{
//						m_EnemyY = j + 1;
//						m_EnemyX = i;
//						break;
//					}
//				}
//			}
//	}
//
//	// if hit the deck
//	if (m_EnemyShipsOnMap[m_EnemyX][m_EnemyY] == e_CellConditions::Deck)
//	{
//		m_CanShotOnMap[m_EnemyX][m_EnemyY] = false;
//
//		// if there is not any deck nearby
//		if ((((m_EnemyX - 1) >= 0 && m_EnemyShipsOnMap[m_EnemyX - 1][m_EnemyY] == e_CellConditions::None) || (m_EnemyX - 1) < 0) 
//			&& (((m_EnemyX + 1) <= 9 && m_EnemyShipsOnMap[m_EnemyX + 1][m_EnemyY] == e_CellConditions::None) || (m_EnemyX + 1) > 9) 
//			&& (((m_EnemyY - 1) >= 0 && m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - 1] == e_CellConditions::None) || (m_EnemyY - 1) < 0)
//			&& (((m_EnemyY + 1) <= 9 && m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + 1] == e_CellConditions::None) || (m_EnemyY + 1) > 9))
//		{
//			deckIsDestroyed(m_EnemyX, m_EnemyY, enemy);
//			m_OneDeckShip--;
//			m_FireState = e_FireOnEnemy::defaultRand;
//		}
//		// if there is a deck nearby
//		// hit and deck is wounded now 
//		else if (((m_EnemyX - 1) >= 0 && m_EnemyShipsOnMap[m_EnemyX - 1][m_EnemyY] == e_CellConditions::Deck) || ((m_EnemyX + 1) <= 9 
//			&& m_EnemyShipsOnMap[m_EnemyX + 1][m_EnemyY] == e_CellConditions::Deck) || ((m_EnemyY - 1) >= 0
//				&& m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - 1] == e_CellConditions::Deck) || ((m_EnemyY + 1) <= 9 && m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + 1] == e_CellConditions::Deck))
//		{
//			enemy.m_Map[m_EnemyX][m_EnemyY].setFillColor(Color::Green);
//			m_FireState = e_FireOnEnemy::hitLastShot;
//			m_EnemyShipsOnMap[m_EnemyX][m_EnemyY] = e_CellConditions::Wound;
//		}
//		// if there is a wounded deck nearby
//		else if (m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - 1] == e_CellConditions::Wound || m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + 1] == e_CellConditions::Wound)
//		{
//			m_DeckNums = 0;
//			m_DeckWoundNums = 0;
//
//			// if there is an existed decks above or below
//			if (m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - 1] == e_CellConditions::Wound)
//			{
//				for (int k = 1; k <= 3; k++)
//					if ((m_EnemyY - k) >= 0 && m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - k] != e_CellConditions::None)
//					{
//						if (m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - k] == e_CellConditions::Deck)
//							m_DeckNums++;
//					}
//					else break;
//			}
//			if (m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + 1] == e_CellConditions::Wound)
//			{
//				for (int k = 1; k <= 3; k++)
//					if ((m_EnemyY + k) <= 9 && m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + k] != e_CellConditions::None)
//					{
//						if (m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + k] == e_CellConditions::Deck)
//							m_DeckNums++;
//					}
//					else break;
//			}
//
//			// if there is existed decks
//			// then just wound the deck
//			if (m_DeckNums != 0)
//			{
//				enemy.m_Map[m_EnemyX][m_EnemyY].setFillColor(Color::Green);
//				m_FireState = e_FireOnEnemy::hitLastShot;
//				m_CanShotOnMap[m_EnemyX][m_EnemyY] = false;
//				m_EnemyShipsOnMap[m_EnemyX][m_EnemyY] = e_CellConditions::Wound;
//			}
//			// otherwise - destroy the ship
//			else if (m_DeckNums == 0)
//			{
//				deckIsDestroyed(m_EnemyX, m_EnemyY, enemy);
//				m_DeckWoundNums = 1;
//
//				// calculate wounded decks by vertical
//				for (int a = 1; a <= 3; a++)
//				{
//					if ((m_EnemyY - a) >= 0 && m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - a] != e_CellConditions::None)
//					{
//						if (m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - a] == e_CellConditions::Wound)
//						{
//							deckIsDestroyed(m_EnemyX, m_EnemyY - a, enemy);
//							m_DeckWoundNums++;
//						}
//					}
//					else break;
//				}
//				for (int a = 1; a <= 3; a++)
//				{
//					if ((m_EnemyY + a) >= 0 && m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + a] != e_CellConditions::None)
//					{
//						if (m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + a] == e_CellConditions::Wound)
//						{
//							deckIsDestroyed(m_EnemyX, m_EnemyY + a, enemy);
//							m_DeckWoundNums++;
//						}
//					}
//					else break;
//				}
//			}
//
//			// if the ship was destroyed
//			if (m_DeckNums == 0)
//				m_FireState = e_FireOnEnemy::defaultRand;
//		}
//		else if (m_EnemyShipsOnMap[m_EnemyX - 1][m_EnemyY] == e_CellConditions::Wound || m_EnemyShipsOnMap[m_EnemyX + 1][m_EnemyY] == e_CellConditions::Wound)
//		{
//			m_DeckNums = 0;
//			m_DeckWoundNums = 0;
//
//			if (m_EnemyShipsOnMap[m_EnemyX - 1][m_EnemyY] == e_CellConditions::Wound)
//			{
//				for (int k = 1; k <= 3; k++)
//					if ((m_EnemyX - k) >= 0 && m_EnemyShipsOnMap[m_EnemyX - k][m_EnemyY] != e_CellConditions::None)
//					{
//						if (m_EnemyShipsOnMap[m_EnemyX - k][m_EnemyY] == e_CellConditions::Deck)
//							m_DeckNums++;
//					}
//					else break;
//			}
//			if (m_EnemyShipsOnMap[m_EnemyX + 1][m_EnemyY] == e_CellConditions::Wound)
//			{
//				for (int k = 1; k <= 3; k++)
//					if ((m_EnemyX + k) <= 9 && m_EnemyShipsOnMap[m_EnemyX + k][m_EnemyY] != e_CellConditions::None)
//					{
//						if (m_EnemyShipsOnMap[m_EnemyX + k][m_EnemyY] == e_CellConditions::Deck)
//							m_DeckNums++;
//					}
//					else break;
//			}
//
//			if (m_DeckNums != 0)
//			{
//				enemy.m_Map[m_EnemyX][m_EnemyY].setFillColor(Color::Green);
//				m_FireState = e_FireOnEnemy::hitLastShot;
//				m_CanShotOnMap[m_EnemyX][m_EnemyY] = false;
//				m_EnemyShipsOnMap[m_EnemyX][m_EnemyY] = e_CellConditions::Wound;
//			}
//			else if (m_DeckNums == 0)
//			{
//				deckIsDestroyed(m_EnemyX, m_EnemyY, enemy);
//				m_DeckWoundNums = 1;
//
//				for (int a = 1; a <= 3; a++)
//				{
//					if ((m_EnemyX - a) >= 0 && m_EnemyShipsOnMap[m_EnemyX - a][m_EnemyY] != e_CellConditions::None)
//					{
//						if (m_EnemyShipsOnMap[m_EnemyX - a][m_EnemyY] == e_CellConditions::Wound)
//						{
//							deckIsDestroyed(m_EnemyX - a, m_EnemyY, enemy);
//							m_DeckWoundNums++;
//						}
//					}
//					else break;
//				}
//				for (int a = 1; a <= 3; a++)
//				{
//					if ((m_EnemyX + a) >= 0 && m_EnemyShipsOnMap[m_EnemyX + a][m_EnemyY] != e_CellConditions::None)
//					{
//						if (m_EnemyShipsOnMap[m_EnemyX + a][m_EnemyY] == e_CellConditions::Wound)
//						{
//							deckIsDestroyed(m_EnemyX + a, m_EnemyY, enemy);
//							m_DeckWoundNums++;
//						}
//					}
//					else break;
//				}
//			}
//
//			if (m_DeckNums == 0)
//				m_FireState = e_FireOnEnemy::defaultRand;
//		}
//		m_NumOfEnemyShips = 0;
//
//		// check if the enemy has ships
//		for (int i = 0; i < 10; i++)
//			for (int j = 0; j < 10; j++)
//				if (m_EnemyShipsOnMap[i][j] == e_CellConditions::Deck)
//					m_NumOfEnemyShips++;
//
//	}
//	// if missing
//	else if (m_EnemyShipsOnMap[m_EnemyX][m_EnemyY] == e_CellConditions::None)
//	{
//		enemy.m_Map[m_EnemyX][m_EnemyY].setFillColor(Color::Magenta);
//		m_CanShotOnMap[m_EnemyX][m_EnemyY] = false;
//		m_CanShot = false;
//		canEnemyShot = true;
//		if (m_FireState == e_FireOnEnemy::hitLastShot)
//			m_FireState = e_FireOnEnemy::thereAreWoundDeck;
//	}
//}