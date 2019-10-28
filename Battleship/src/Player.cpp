#include "Player.h"

void Player::addShipsOnMap(RenderWindow& win, Vector2i p, int movePos)
{
	p = Mouse::getPosition(win); // Get mouse cursor position
	p.x -= movePos; // For the second player

	for (int k = 0; k < 10; k++)
	{
		for (int l = 0; l < 10; l++)
		{
			if (m_PlayerMap.m_Map[k][l].getGlobalBounds().contains(p.x, p.y) && m_PlayerShipsOnMap[k][l] == e_CellConditions::None && m_CanPut[k][l] == true) // if the cursor on the cell=[k;l]
			{
				//  set deck
				if (m_FourDeckShip != 0)
				{
					initShipOnMap(k, l, 4); // pass the nu,ber of the decks
				}
				else if (m_ThreeDeckShip != 0 && m_FourDeckShip == 0)
				{
					if (m_NumberOfTheDeck == 4) m_NumberOfTheDeck = 1;
					initShipOnMap(k, l, 3); 
				}
				else if (m_TwoDeckShip != 0 && m_ThreeDeckShip == 0)
				{
					if (m_NumberOfTheDeck == 3) m_NumberOfTheDeck = 1;
					initShipOnMap(k, l, 2);
				}
				else if (m_OneDeckShip != 0 && m_TwoDeckShip == 0)
				{
					deckIsReady(k, l);
					m_PlayerShipsOnMap[k][l] = e_CellConditions::Deck;
					// can't set the deck around the ship
					if ((k - 1) >= 0) m_CanPut[k - 1][l] = false;
					if ((k + 1) <= 9) m_CanPut[k + 1][l] = false;
					if ((l - 1) >= 0) m_CanPut[k][l - 1] = false;
					if ((l + 1) <= 9) m_CanPut[k][l + 1] = false;
				}
			}
		}
	}
}
void Player::setMapPlayerAiColor()
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
		{
			m_PlayerMap.m_Map[i][j].setFillColor(Color(16, 16, 68));
		}
}

void Player::deckIsReady(int i, int j)
{
	m_PlayerMap.m_Map[i][j].setFillColor(Color(100, 100, 100));
	m_PlayerShipsOnMap[i][j] = e_CellConditions::Deck;
	m_MaxNumberOfDecks--;
	
	// A deck cann't be placed diagonally 
	if ((i + 1) <= 9 && (j - 1) >= 0)
		m_CanPut[i + 1][j - 1] = false;
	if ((i - 1) >= 0 && (j - 1) >= 0)
		m_CanPut[i - 1][j - 1] = false;
	if ((i - 1) >= 0 && (j + 1) <= 9)
		m_CanPut[i - 1][j + 1] = false;
	if ((i + 1) <= 9 && (j + 1) <= 9)
		m_CanPut[i + 1][j + 1] = false;
}

void Player::deckIsDestroyed(int a, int b, Map& enemy)
{
	m_EnemyShipsOnMap[a][b] = e_CellConditions::Kill;
	enemy.m_Map[a][b].setFillColor(Color::Red);
}

void Player::initShipOnMap(int f, int s, int decs)
{
	if (m_NumberOfTheDeck == 1)
	{
		m_NumberOfTheDeck++;
		deckIsReady(f, s);
	}
	else if ((m_NumberOfTheDeck > 1) && (m_NumberOfTheDeck < decs) &&
		((m_PlayerShipsOnMap[f - 1][s] == e_CellConditions::Deck) ||
		(m_PlayerShipsOnMap[f + 1][s] == e_CellConditions::Deck) ||
			(m_PlayerShipsOnMap[f][s - 1] == e_CellConditions::Deck) ||
			(m_PlayerShipsOnMap[f][s + 1] == e_CellConditions::Deck)))
	{
		deckIsReady(f, s);
		m_NumberOfTheDeck++;
	}

	else if (m_NumberOfTheDeck == decs && (m_PlayerShipsOnMap[f - 1][s] == e_CellConditions::Deck || m_PlayerShipsOnMap[f + 1][s] == e_CellConditions::Deck
		|| m_PlayerShipsOnMap[f][s - 1] == e_CellConditions::Deck || m_PlayerShipsOnMap[f][s + 1] == e_CellConditions::Deck))
	{
		deckIsReady(f, s);
		m_NumberOfTheDeck = 1;
		switch (decs) {
		case 2: m_TwoDeckShip--;
			break;
		case 3: m_ThreeDeckShip--;
			break;
		case 4: m_FourDeckShip--;
			break;
		}

		// Cells around the ship are no longer available 
		if (m_PlayerShipsOnMap[f - 1][s] == e_CellConditions::Deck)
		{
			if ((f + 1) <= 9)
				m_CanPut[f + 1][s] = false;
			if ((f - decs) >= 0)
				m_CanPut[f - decs][s] = false;
		}
		else if (m_PlayerShipsOnMap[f + 1][s] == e_CellConditions::Deck)
		{
			if ((f - 1) >= 0)
				m_CanPut[f - 1][s] = false;
			if ((f + decs) <= 9)
				m_CanPut[f + decs][s] = false;
		}
		else if (m_PlayerShipsOnMap[f][s - 1] == e_CellConditions::Deck)
		{
			if ((s + 1) <= 9)
				m_CanPut[f][s + 1] = false;
			if ((s - decs) >= 0)
				m_CanPut[f][s - decs] = false;
		}
		else if (m_PlayerShipsOnMap[f][s + 1] == e_CellConditions::Deck)
		{
			if ((s - 1) >= 0)
				m_CanPut[f][s - 1] = false;
			if ((s + decs) <= 9)
				m_CanPut[f][s + decs] = false;
		}
	}
}
void Player::shotInEnemy(RenderWindow& win, Vector2i p, int movePos, bool& canEnemyShot, Map& enemy)
{
	p = Mouse::getPosition(win);
	p.x -= movePos;

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
		{
			if (enemy.m_Map[i][j].getGlobalBounds().contains(p.x, p.y)) // if the mouse cursor on the enemy map
			{
				m_EnemyX = i; m_EnemyY = j;
				if (m_EnemyShipsOnMap[i][j] == e_CellConditions::Deck)// if hit the deck
				{
					// if there isn't any other decks nearby
					// then destroy the deck
					if ((((m_EnemyX - 1) >= 0 && m_EnemyShipsOnMap[m_EnemyX - 1][m_EnemyY] == e_CellConditions::None) || (m_EnemyX - 1) < 0)
						&& (((m_EnemyX + 1) <= 9 && m_EnemyShipsOnMap[m_EnemyX + 1][m_EnemyY] == e_CellConditions::None) || (m_EnemyX + 1) > 9)
						&& (((m_EnemyY - 1) >= 0 && m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - 1] == e_CellConditions::None) || (m_EnemyY - 1) < 0) 
						&& (((m_EnemyY + 1) <= 9 && m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + 1] == e_CellConditions::None) || (m_EnemyY + 1) > 9))
					{
						deckIsDestroyed(m_EnemyX, m_EnemyY, enemy);
						m_OneDeckShip--;
					}
					// if there is a deck nearby
					// then wound the deck
					else if (((m_EnemyX - 1) >= 0 && m_EnemyShipsOnMap[m_EnemyX - 1][m_EnemyY] == e_CellConditions::Deck) || ((m_EnemyX + 1) <= 9
						&& m_EnemyShipsOnMap[m_EnemyX + 1][m_EnemyY] == e_CellConditions::Deck) || ((m_EnemyY - 1) >= 0
							&& m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - 1] == e_CellConditions::Deck) || ((m_EnemyY + 1) <= 9
								&& m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + 1] == e_CellConditions::Deck))
					{
						enemy.m_Map[m_EnemyX][m_EnemyY].setFillColor(Color::Green);
						m_EnemyShipsOnMap[m_EnemyX][m_EnemyY] = e_CellConditions::Wound;
					}
					// if there is a wound deck nearby
					else if (m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - 1] == e_CellConditions::Wound || m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + 1] == e_CellConditions::Wound)
					{
						m_DeckNums = 0;
						m_DeckWoundNums = 0;

						// if there is existed decks up or down
						if (m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - 1] == e_CellConditions::Wound)
						{
							for (int k = 1; k <= 3; k++)
								if ((m_EnemyY - k) >= 0 && m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - k] != e_CellConditions::None)
								{
									if (m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - k] == e_CellConditions::Deck)
										m_DeckNums++;
								}
								else break;
						}
						if (m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + 1] == e_CellConditions::Wound)
						{
							for (int k = 1; k <= 3; k++)
								if ((m_EnemyY + k) <= 9 && m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + k] != e_CellConditions::None)
								{
									if (m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + k] == e_CellConditions::Deck)
										m_DeckNums++;
								}
								else break;
						}

						// if there is existed deck
						if (m_DeckNums != 0)
						{
							enemy.m_Map[m_EnemyX][m_EnemyY].setFillColor(Color::Green);
							m_EnemyShipsOnMap[m_EnemyX][m_EnemyY] = e_CellConditions::Wound;
						}
						// otherwise destroy the ship
						else if (m_DeckNums == 0)
						{
							deckIsDestroyed(m_EnemyX, m_EnemyY, enemy);
							m_DeckWoundNums = 1;

							// calculate wounded decks by vertical
							for (int a = 1; a <= 3; a++)
							{
								if ((m_EnemyY - a) >= 0 && m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - a] != e_CellConditions::None)
								{
									if (m_EnemyShipsOnMap[m_EnemyX][m_EnemyY - a] == e_CellConditions::Wound)
									{
										deckIsDestroyed(m_EnemyX, m_EnemyY - a, enemy);
										m_DeckWoundNums++;
									}
								}
								else break;
							}
							for (int a = 1; a <= 3; a++)
							{
								if ((m_EnemyY + a) >= 0 && m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + a] != e_CellConditions::None)
								{
									if (m_EnemyShipsOnMap[m_EnemyX][m_EnemyY + a] == e_CellConditions::Wound)
									{
										deckIsDestroyed(m_EnemyX, m_EnemyY + a, enemy);
										m_DeckWoundNums++;
									}
								}
								else break;
							}
						}
					}
					else if (m_EnemyShipsOnMap[m_EnemyX - 1][m_EnemyY] == e_CellConditions::Wound || m_EnemyShipsOnMap[m_EnemyX + 1][m_EnemyY] == e_CellConditions::Wound)
					{
						m_DeckNums = 0;
						m_DeckWoundNums = 0;

						// if there is existed decks above or below
						if (m_EnemyShipsOnMap[m_EnemyX - 1][m_EnemyY] == e_CellConditions::Wound)
						{
							for (int k = 1; k <= 3; k++)
								if ((m_EnemyX - k) >= 0 && m_EnemyShipsOnMap[m_EnemyX - k][m_EnemyY] != e_CellConditions::None)
								{
									if (m_EnemyShipsOnMap[m_EnemyX - k][m_EnemyY] == e_CellConditions::Deck)
										m_DeckNums++;
								}
								else break;
						}
						if (m_EnemyShipsOnMap[m_EnemyX + 1][m_EnemyY] == e_CellConditions::Wound)
						{
							for (int k = 1; k <= 3; k++)
								if ((m_EnemyX + k) <= 9 && m_EnemyShipsOnMap[m_EnemyX + k][m_EnemyY] != e_CellConditions::None)
								{
									if (m_EnemyShipsOnMap[m_EnemyX + k][m_EnemyY] == e_CellConditions::Deck)
										m_DeckNums++;
								}
								else break;
						}

						// if there is a existed deck
						// then hit the deck
						if (m_DeckNums != 0)
						{
							enemy.m_Map[m_EnemyX][m_EnemyY].setFillColor(Color::Green);
							m_EnemyShipsOnMap[m_EnemyX][m_EnemyY] = e_CellConditions::Wound;
						}
						// otherwise - destroy the ship
						else if (m_DeckNums == 0)
						{
							deckIsDestroyed(m_EnemyX, m_EnemyY, enemy);
							m_DeckWoundNums = 1;

							// calculate number of the wounded decks by vertical
							for (int a = 1; a <= 3; a++)
							{
								if ((m_EnemyX - a) >= 0 && m_EnemyShipsOnMap[m_EnemyX - a][m_EnemyY] != e_CellConditions::None)
								{
									if (m_EnemyShipsOnMap[m_EnemyX - a][m_EnemyY] == e_CellConditions::Wound)
									{
										deckIsDestroyed(m_EnemyX - a, m_EnemyY, enemy);
										m_DeckWoundNums++;
									}
								}
								else break;
							}
							for (int a = 1; a <= 3; a++)
							{
								if ((m_EnemyX + a) >= 0 && m_EnemyShipsOnMap[m_EnemyX + a][m_EnemyY] != e_CellConditions::None)
								{
									if (m_EnemyShipsOnMap[m_EnemyX + a][m_EnemyY] == e_CellConditions::Wound)
									{
										deckIsDestroyed(m_EnemyX + a, m_EnemyY, enemy);
										m_DeckWoundNums++;
									}
								}
								else break;
							}
						}
					}
					m_NumOfEnemyShips = 0;
					// check if the enemy has ships
					for (int i = 0; i < 10; i++)
						for (int j = 0; j < 10; j++)
							if (m_EnemyShipsOnMap[i][j] == e_CellConditions::Deck)
								m_NumOfEnemyShips++;
				}
				// missing
				else if (m_EnemyShipsOnMap[i][j] == e_CellConditions::None)
				{
					enemy.m_Map[i][j].setFillColor(Color::Magenta);
					m_CanShot = false;
					canEnemyShot = true;
				}
			}
		}
}