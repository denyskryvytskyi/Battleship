#include "ComputerPlayer.h"

void ComputerPlayer::addShipsOnMap()
{
	srand(time(NULL));

	while (maxNumberOfDecks > 0)
	{
		enemyX = rand() % 9;
		enemyY = rand() % 9;

		if (playerShipsOnMap[enemyX][enemyY] == None && canPut[enemyX][enemyY] == true)
		{
			// Placed ships
			if (fourDeckShip != 0)
			{
				initShipOnMap(enemyX, enemyY, 4);
			}
			else if (threeDeckShip != 0 && fourDeckShip == 0)
			{
				if (numberOfTheDeck == 4) numberOfTheDeck = 1;
				initShipOnMap(enemyX, enemyY, 3);
			}
			else if (twoDeckShip != 0 && threeDeckShip == 0)
			{
				if (numberOfTheDeck == 3) numberOfTheDeck = 1;
				initShipOnMap(enemyX, enemyY, 2);
			}
			else if (oneDeckShip != 0 && twoDeckShip == 0)
			{
				deckIsReady(enemyX, enemyY);
				playerShipsOnMap[enemyX][enemyY] = Deck;

				if ((enemyX - 1) >= 0) canPut[enemyX - 1][enemyY] = false;
				if ((enemyX + 1) <= 9) canPut[enemyX + 1][enemyY] = false;
				if ((enemyY - 1) >= 0) canPut[enemyX][enemyY - 1] = false;
				if ((enemyY + 1) <= 9) canPut[enemyX][enemyY + 1] = false;
			}
		}
	}
}
void ComputerPlayer::randXY()
{
	srand(time(NULL));
	enemyX = rand() % 9;
	enemyY = rand() % 9;
}

void ComputerPlayer::shotInEnemy(RenderWindow& win, Vector2i p, bool& canEnemyShot, Map& enemy)
{

	if (fireState == defaultRand)// last shoot - missing and there is not wounded decks
	{
		do {
			randXY();
		} while (canShotOnMap[enemyX][enemyY] != true);

	}

	// if there is wounded decks
	if (fireState == thereAreWoundDeck || fireState == hitLastShot)
	{
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
			{
				if (enemyShipsOnMap[i][j] == Wound && (canShotOnMap[i - 1][j] || canShotOnMap[i + 1][j] 
					|| canShotOnMap[i][j - 1] || canShotOnMap[i][j + 1]))
				{

					if (enemyShipsOnMap[i - 1][j] != Wound && canShotOnMap[i - 1][j])
					{
						enemyX = i - 1;
						enemyY = j;
						break;
					}
					else if (enemyShipsOnMap[i + 1][j] != Wound && canShotOnMap[i + 1][j])
					{
						enemyX = i + 1;
						enemyY = j;
						break;
					}
					else if (enemyShipsOnMap[i][j - 1] != Wound && canShotOnMap[i][j - 1])
					{
						enemyY = j - 1;
						enemyX = i;
						break;
					}
					else if (enemyShipsOnMap[i][j + 1] != Wound && canShotOnMap[i][j + 1])
					{
						enemyY = j + 1;
						enemyX = i;
						break;
					}
				}
			}
	}

	// if hit the deck
	if (enemyShipsOnMap[enemyX][enemyY] == Deck)
	{
		canShotOnMap[enemyX][enemyY] = false;

		// if there is not any deck nearby
		if ((((enemyX - 1) >= 0 && enemyShipsOnMap[enemyX - 1][enemyY] == None) || (enemyX - 1) < 0) && (((enemyX + 1) <= 9 && enemyShipsOnMap[enemyX + 1][enemyY] == None) || (enemyX + 1) > 9) && (((enemyY - 1) >= 0 && enemyShipsOnMap[enemyX][enemyY - 1] == None) || (enemyY - 1) < 0) && (((enemyY + 1) <= 9 && enemyShipsOnMap[enemyX][enemyY + 1] == None) || (enemyY + 1) > 9))
		{
			deckIsDestroyed(enemyX, enemyY, enemy);
			oneDeckShip--;
			fireState = defaultRand;
		}
		// if there is a deck nearby
		// hit and deck is wounded now 
		else if (((enemyX - 1) >= 0 && enemyShipsOnMap[enemyX - 1][enemyY] == Deck) || ((enemyX + 1) <= 9 && enemyShipsOnMap[enemyX + 1][enemyY] == Deck) || ((enemyY - 1) >= 0 && enemyShipsOnMap[enemyX][enemyY - 1] == Deck) || ((enemyY + 1) <= 9 && enemyShipsOnMap[enemyX][enemyY + 1] == Deck))
		{
			enemy.map[enemyX][enemyY].setFillColor(sf::Color::Green);
			fireState = hitLastShot;
			enemyShipsOnMap[enemyX][enemyY] = Wound;
		}
		// if there is a wounded deck nearby
		else if (enemyShipsOnMap[enemyX][enemyY - 1] == Wound || enemyShipsOnMap[enemyX][enemyY + 1] == Wound)
		{
			deckNums = 0;
			deckWoundNums = 0;

			// if there is an existed decks above or below
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

			// if there is existed decks
			// then just wound the deck
			if (deckNums != 0)
			{
				enemy.map[enemyX][enemyY].setFillColor(sf::Color::Green);
				fireState = hitLastShot;
				canShotOnMap[enemyX][enemyY] = false;
				enemyShipsOnMap[enemyX][enemyY] = Wound;
			}
			// otherwise - destroy the ship
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

			// if the ship was destroyed
			if (deckNums == 0)
				fireState = defaultRand;
		}
		else if (enemyShipsOnMap[enemyX - 1][enemyY] == Wound || enemyShipsOnMap[enemyX + 1][enemyY] == Wound)
		{
			deckNums = 0;
			deckWoundNums = 0;

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

			if (deckNums != 0)
			{
				enemy.map[enemyX][enemyY].setFillColor(sf::Color::Green);
				fireState = hitLastShot;
				canShotOnMap[enemyX][enemyY] = false;
				enemyShipsOnMap[enemyX][enemyY] = Wound;
			}
			else if (deckNums == 0)
			{
				deckIsDestroyed(enemyX, enemyY, enemy);
				deckWoundNums = 1;

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

			if (deckNums == 0)
				fireState = defaultRand;
		}
		numOfEnemyShips = 0;

		// check if the enemy has ships
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
				if (enemyShipsOnMap[i][j] == Deck)
					numOfEnemyShips++;

	}
	// if missing
	else if (enemyShipsOnMap[enemyX][enemyY] == None)
	{
		enemy.map[enemyX][enemyY].setFillColor(sf::Color::Magenta);
		canShotOnMap[enemyX][enemyY] = false;
		canShot = false;
		canEnemyShot = true;
		if (fireState == hitLastShot)
			fireState = thereAreWoundDeck;
	}
}