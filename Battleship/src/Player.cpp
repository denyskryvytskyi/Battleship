#include "Player.h"

void Player::addShipsOnMap(RenderWindow& win, Vector2i p, int movePos)
{
	p = Mouse::getPosition(win); // Get mouse cursor position
	p.x -= movePos; // For the second player

	for (int k = 0; k < 10; k++)
	{
		for (int l = 0; l < 10; l++)
		{
			if (playerMap.map[k][l].getGlobalBounds().contains(p.x, p.y) && playerShipsOnMap[k][l] == None && canPut[k][l] == true) // if the cursor on the cell=[k;l]
			{
				//  set deck
				if (fourDeckShip != 0)
				{
					initShipOnMap(k, l, 4); // pass the nu,ber of the decks
				}
				else if (threeDeckShip != 0 && fourDeckShip == 0)
				{
					if (numberOfTheDeck == 4) numberOfTheDeck = 1;
					initShipOnMap(k, l, 3); 
				}
				else if (twoDeckShip != 0 && threeDeckShip == 0)
				{
					if (numberOfTheDeck == 3) numberOfTheDeck = 1;
					initShipOnMap(k, l, 2);
				}
				else if (oneDeckShip != 0 && twoDeckShip == 0)
				{
					deckIsReady(k, l);
					playerShipsOnMap[k][l] = Deck;
					// can't set the deck around the ship
					if ((k - 1) >= 0) canPut[k - 1][l] = false;
					if ((k + 1) <= 9) canPut[k + 1][l] = false;
					if ((l - 1) >= 0) canPut[k][l - 1] = false;
					if ((l + 1) <= 9) canPut[k][l + 1] = false;
				}
			}
		}
	}
}
void Player::setMapColor()
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
		{
			playerMap.map[i][j].setFillColor(sf::Color(16, 16, 68));
		}
}

void Player::deckIsReady(int i, int j)
{
	playerMap.map[i][j].setFillColor(sf::Color(100, 100, 100));
	playerShipsOnMap[i][j] = Deck; 
	maxNumberOfDecks--;
	
	// A deck cann't be placed diagonally 
	if ((i + 1) <= 9 && (j - 1) >= 0)
		canPut[i + 1][j - 1] = false;
	if ((i - 1) >= 0 && (j - 1) >= 0)
		canPut[i - 1][j - 1] = false;
	if ((i - 1) >= 0 && (j + 1) <= 9)
		canPut[i - 1][j + 1] = false;
	if ((i + 1) <= 9 && (j + 1) <= 9)
		canPut[i + 1][j + 1] = false;
}

void Player::deckIsDestroyed(int a, int b, Map& enemy)
{
	enemyShipsOnMap[a][b] = Kill;
	enemy.map[a][b].setFillColor(sf::Color::Red);
}

void Player::initShipOnMap(int f, int s, int decs)
{
	if (numberOfTheDeck == 1)
	{
		numberOfTheDeck++;
		deckIsReady(f, s);
	}
	else if ((numberOfTheDeck > 1) && (numberOfTheDeck < decs) &&
		((playerShipsOnMap[f - 1][s] == Deck) ||
		(playerShipsOnMap[f + 1][s] == Deck) ||
			(playerShipsOnMap[f][s - 1] == Deck) ||
			(playerShipsOnMap[f][s + 1] == Deck)))
	{
		deckIsReady(f, s);
		numberOfTheDeck++;
	}

	else if (numberOfTheDeck == decs && (playerShipsOnMap[f - 1][s] == Deck || playerShipsOnMap[f + 1][s] == Deck 
		|| playerShipsOnMap[f][s - 1] == Deck || playerShipsOnMap[f][s + 1] == Deck))
	{
		deckIsReady(f, s);
		numberOfTheDeck = 1;
		switch (decs) {
		case 2: twoDeckShip--;
			break;
		case 3: threeDeckShip--;
			break;
		case 4: fourDeckShip--;
			break;
		}

		// Cells around the ship are no longer available 
		if (playerShipsOnMap[f - 1][s] == Deck)
		{
			if ((f + 1) <= 9)
				canPut[f + 1][s] = false;
			if ((f - decs) >= 0)
				canPut[f - decs][s] = false;
		}
		else if (playerShipsOnMap[f + 1][s] == Deck)
		{
			if ((f - 1) >= 0)
				canPut[f - 1][s] = false;
			if ((f + decs) <= 9)
				canPut[f + decs][s] = false;
		}
		else if (playerShipsOnMap[f][s - 1] == Deck)
		{
			if ((s + 1) <= 9)
				canPut[f][s + 1] = false;
			if ((s - decs) >= 0)
				canPut[f][s - decs] = false;
		}
		else if (playerShipsOnMap[f][s + 1] == Deck)
		{
			if ((s - 1) >= 0)
				canPut[f][s - 1] = false;
			if ((s + decs) <= 9)
				canPut[f][s + decs] = false;
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
			if (enemy.map[i][j].getGlobalBounds().contains(p.x, p.y)) // if the mouse cursor on the enemy map
			{
				enemyX = i; enemyY = j;
				if (enemyShipsOnMap[i][j] == Deck)// if hit the deck
				{
					// if there isn't any other decks nearby
					// then destroy the deck
					if ((((enemyX - 1) >= 0 && enemyShipsOnMap[enemyX - 1][enemyY] == None) || (enemyX - 1) < 0) 
						&& (((enemyX + 1) <= 9 && enemyShipsOnMap[enemyX + 1][enemyY] == None) || (enemyX + 1) > 9)
						&& (((enemyY - 1) >= 0 && enemyShipsOnMap[enemyX][enemyY - 1] == None) || (enemyY - 1) < 0) 
						&& (((enemyY + 1) <= 9 && enemyShipsOnMap[enemyX][enemyY + 1] == None) || (enemyY + 1) > 9))
					{
						deckIsDestroyed(enemyX, enemyY, enemy);
						oneDeckShip--;
					}
					// if there is a deck nearby
					// then wound the deck
					else if (((enemyX - 1) >= 0 && enemyShipsOnMap[enemyX - 1][enemyY] == Deck) || ((enemyX + 1) <= 9 
						&& enemyShipsOnMap[enemyX + 1][enemyY] == Deck) || ((enemyY - 1) >= 0 
							&& enemyShipsOnMap[enemyX][enemyY - 1] == Deck) || ((enemyY + 1) <= 9 
								&& enemyShipsOnMap[enemyX][enemyY + 1] == Deck))
					{
						enemy.map[enemyX][enemyY].setFillColor(sf::Color::Green);
						enemyShipsOnMap[enemyX][enemyY] = Wound;
					}
					// if there is a wound deck nearby
					else if (enemyShipsOnMap[enemyX][enemyY - 1] == Wound || enemyShipsOnMap[enemyX][enemyY + 1] == Wound)
					{
						deckNums = 0;
						deckWoundNums = 0;

						// if there is existed decks up or down
						if (enemyShipsOnMap[enemyX][enemyY - 1] == Wound)
						{
							for (int k = 1; k <= 3; k++)
								if ((enemyY - k) >= 0 && enemyShipsOnMap[enemyX][enemyY - k] != None)
								{
									if (enemyShipsOnMap[enemyX][enemyY - k] == Deck)
										deckNums++;
								}
								else break;
						}
						if (enemyShipsOnMap[enemyX][enemyY + 1] == Wound)
						{
							for (int k = 1; k <= 3; k++)
								if ((enemyY + k) <= 9 && enemyShipsOnMap[enemyX][enemyY + k] != None)
								{
									if (enemyShipsOnMap[enemyX][enemyY + k] == Deck)
										deckNums++;
								}
								else break;
						}

						// if there is existed deck
						if (deckNums != 0)
						{
							enemy.map[enemyX][enemyY].setFillColor(sf::Color::Green);
							enemyShipsOnMap[enemyX][enemyY] = Wound;
						}
						// otherwise destroy the ship
						else if (deckNums == 0)
						{
							deckIsDestroyed(enemyX, enemyY, enemy);
							deckWoundNums = 1;

							// calculate wounded decks by vertical
							for (int a = 1; a <= 3; a++)
							{
								if ((enemyY - a) >= 0 && enemyShipsOnMap[enemyX][enemyY - a] != None)
								{
									if (enemyShipsOnMap[enemyX][enemyY - a] == Wound)
									{
										deckIsDestroyed(enemyX, enemyY - a, enemy);
										deckWoundNums++;
									}
								}
								else break;
							}
							for (int a = 1; a <= 3; a++)
							{
								if ((enemyY + a) >= 0 && enemyShipsOnMap[enemyX][enemyY + a] != None)
								{
									if (enemyShipsOnMap[enemyX][enemyY + a] == Wound)
									{
										deckIsDestroyed(enemyX, enemyY + a, enemy);
										deckWoundNums++;
									}
								}
								else break;
							}
						}
					}
					else if (enemyShipsOnMap[enemyX - 1][enemyY] == Wound || enemyShipsOnMap[enemyX + 1][enemyY] == Wound)
					{
						deckNums = 0;
						deckWoundNums = 0;

						// if there is existed decks above or below
						if (enemyShipsOnMap[enemyX - 1][enemyY] == Wound)
						{
							for (int k = 1; k <= 3; k++)
								if ((enemyX - k) >= 0 && enemyShipsOnMap[enemyX - k][enemyY] != None)
								{
									if (enemyShipsOnMap[enemyX - k][enemyY] == Deck)
										deckNums++;
								}
								else break;
						}
						if (enemyShipsOnMap[enemyX + 1][enemyY] == Wound)
						{
							for (int k = 1; k <= 3; k++)
								if ((enemyX + k) <= 9 && enemyShipsOnMap[enemyX + k][enemyY] != None)
								{
									if (enemyShipsOnMap[enemyX + k][enemyY] == Deck)
										deckNums++;
								}
								else break;
						}

						// if there is a existed deck
						// then hit the deck
						if (deckNums != 0)
						{
							enemy.map[enemyX][enemyY].setFillColor(sf::Color::Green);
							enemyShipsOnMap[enemyX][enemyY] = Wound;
						}
						// otherwise - destroy the ship
						else if (deckNums == 0)
						{
							deckIsDestroyed(enemyX, enemyY, enemy);
							deckWoundNums = 1;

							// calculate number of the wounded decks by vertical
							for (int a = 1; a <= 3; a++)
							{
								if ((enemyX - a) >= 0 && enemyShipsOnMap[enemyX - a][enemyY] != None)
								{
									if (enemyShipsOnMap[enemyX - a][enemyY] == Wound)
									{
										deckIsDestroyed(enemyX - a, enemyY, enemy);
										deckWoundNums++;
									}
								}
								else break;
							}
							for (int a = 1; a <= 3; a++)
							{
								if ((enemyX + a) >= 0 && enemyShipsOnMap[enemyX + a][enemyY] != None)
								{
									if (enemyShipsOnMap[enemyX + a][enemyY] == Wound)
									{
										deckIsDestroyed(enemyX + a, enemyY, enemy);
										deckWoundNums++;
									}
								}
								else break;
							}
						}
					}
					numOfEnemyShips = 0;
					// check if the enemy has ships
					for (int i = 0; i < 10; i++)
						for (int j = 0; j < 10; j++)
							if (enemyShipsOnMap[i][j] == Deck)
								numOfEnemyShips++;
				}
				// missing
				else if (enemyShipsOnMap[i][j] == None)
				{
					enemy.map[i][j].setFillColor(sf::Color::Magenta);
					canShot = false;
					canEnemyShot = true;
				}
			}
		}
}