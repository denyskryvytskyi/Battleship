#include "ComputerPlayer.h"


void ComputerPlayer::addShipsOnMap()
{
	srand(time(NULL));

	while (maxNumberOfDecks > 0)
	{
		enemyX = rand() % 9;
		enemyY = rand() % 9;

		if (ShipsOnMap[enemyX][enemyY] == None && canPut[enemyX][enemyY] == true)
		{
			//расставляем палубы
			if (fourDeckShip != 0)
			{
				initShipOnMap(enemyX, enemyY, 4);//передаем кол-во палуб
			}
			else if (threeDeckShip != 0 && fourDeckShip == 0)
			{
				if (numberOfTheDeck == 4) numberOfTheDeck = 1;
				initShipOnMap(enemyX, enemyY, 3);//передаем кол-во палуб
			}
			else if (twoDeckShip != 0 && threeDeckShip == 0)
			{
				if (numberOfTheDeck == 3) numberOfTheDeck = 1;
				initShipOnMap(enemyX, enemyY, 2);//передаем кол-во палуб
			}
			else if (oneDeckShip != 0 && twoDeckShip == 0)
			{
				deckIsReady(enemyX, enemyY);
				ShipsOnMap[enemyX][enemyY] = Deck;
				//вокруг корябля ставить нельзя
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

	if (fireEnemy == defaultRand)// если по умолчанию(то-есть прошлый выстрел - промах, и нет подбитых палуб
	{
		do {
			randXY();
		} while (canShotOnMap[enemyX][enemyY] != true);

	}

	if (fireEnemy == thereAreWoundDeck || fireEnemy == hitLastShot)//если есть подбитые палубы
	{
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
			{
				if (enemysShipsOnMap[i][j] == Wound && (canShotOnMap[i - 1][j] || canShotOnMap[i + 1][j] || canShotOnMap[i][j - 1] || canShotOnMap[i][j + 1]))
				{

					if (enemysShipsOnMap[i - 1][j] != Wound && canShotOnMap[i - 1][j])
					{
						enemyX = i - 1;
						enemyY = j;
						break;
					}
					else if (enemysShipsOnMap[i + 1][j] != Wound && canShotOnMap[i + 1][j])
					{
						enemyX = i + 1;
						enemyY = j;
						break;
					}
					else if (enemysShipsOnMap[i][j - 1] != Wound && canShotOnMap[i][j - 1])
					{
						enemyY = j - 1;
						enemyX = i;
						break;
					}
					else if (enemysShipsOnMap[i][j + 1] != Wound && canShotOnMap[i][j + 1])
					{
						enemyY = j + 1;
						enemyX = i;
						break;
					}
				}
			}
	}

	if (enemysShipsOnMap[enemyX][enemyY] == Deck)//если попали в палубу
	{
		canShotOnMap[enemyX][enemyY] = false;//нельзя стрелять
											 //если рядом нет палуб
		if ((((enemyX - 1) >= 0 && enemysShipsOnMap[enemyX - 1][enemyY] == None) || (enemyX - 1) < 0) && (((enemyX + 1) <= 9 && enemysShipsOnMap[enemyX + 1][enemyY] == None) || (enemyX + 1) > 9) && (((enemyY - 1) >= 0 && enemysShipsOnMap[enemyX][enemyY - 1] == None) || (enemyY - 1) < 0) && (((enemyY + 1) <= 9 && enemysShipsOnMap[enemyX][enemyY + 1] == None) || (enemyY + 1) > 9))
		{
			deckIsDestroyed(enemyX, enemyY, enemy);//если рядом нету палуб - уничтожаем корабль
			oneDeckShip--;//минус один однопалубный корабль
			fireEnemy = defaultRand;//по умолчанию
		}
		//если рядом палуба
		else if (((enemyX - 1) >= 0 && enemysShipsOnMap[enemyX - 1][enemyY] == Deck) || ((enemyX + 1) <= 9 && enemysShipsOnMap[enemyX + 1][enemyY] == Deck) || ((enemyY - 1) >= 0 && enemysShipsOnMap[enemyX][enemyY - 1] == Deck) || ((enemyY + 1) <= 9 && enemysShipsOnMap[enemyX][enemyY + 1] == Deck))
		{
			enemy.map[enemyX][enemyY].setFillColor(sf::Color::Green);//цвет Green если ранен
			fireEnemy = hitLastShot;//попадание
			enemysShipsOnMap[enemyX][enemyY] = Wound;//раним, если рядом Deck 	
		}
		//если рядом раненная палуба
		else if (enemysShipsOnMap[enemyX][enemyY - 1] == Wound || enemysShipsOnMap[enemyX][enemyY + 1] == Wound)
		{
			deckNums = 0;//кол-во не подбитых палуб
			deckWoundNums = 0;//кол-во подбитых палуб

							  //если сверху или снизу есть не подбитые палубы
			if (enemysShipsOnMap[enemyX][enemyY - 1] == Wound)
			{
				for (int k = 1; k <= 3; k++)
					if ((enemyY - k) >= 0 && enemysShipsOnMap[enemyX][enemyY - k] != None)
					{
						if (enemysShipsOnMap[enemyX][enemyY - k] == Deck)
							deckNums++;
					}
					else break;
			}
			if (enemysShipsOnMap[enemyX][enemyY + 1] == Wound)
			{
				for (int k = 1; k <= 3; k++)
					if ((enemyY + k) <= 9 && enemysShipsOnMap[enemyX][enemyY + k] != None)
					{
						if (enemysShipsOnMap[enemyX][enemyY + k] == Deck)
							deckNums++;
					}
					else break;
			}

			//если есть не подбитые палубы, то просто раним палубу
			if (deckNums != 0)
			{
				enemy.map[enemyX][enemyY].setFillColor(sf::Color::Green);
				fireEnemy = hitLastShot;//попадание
				canShotOnMap[enemyX][enemyY] = false;//нельзя стрелять
				enemysShipsOnMap[enemyX][enemyY] = Wound;
			}
			else if (deckNums == 0)//иначе уничтожаем корабль
			{
				deckIsDestroyed(enemyX, enemyY, enemy);
				deckWoundNums = 1;

				//подсчет подбитых палуб по вертикали
				for (int a = 1; a <= 3; a++)
				{
					if ((enemyY - a) >= 0 && enemysShipsOnMap[enemyX][enemyY - a] != None)
					{
						if (enemysShipsOnMap[enemyX][enemyY - a] == Wound)
						{
							deckIsDestroyed(enemyX, enemyY - a, enemy);
							deckWoundNums++;
						}
					}
					else break;
				}
				for (int a = 1; a <= 3; a++)
				{
					if ((enemyY + a) >= 0 && enemysShipsOnMap[enemyX][enemyY + a] != None)
					{
						if (enemysShipsOnMap[enemyX][enemyY + a] == Wound)
						{
							deckIsDestroyed(enemyX, enemyY + a, enemy);
							deckWoundNums++;
						}
					}
					else break;
				}
			}

			//если корабль уничтожен
			if (deckNums == 0)
				fireEnemy = defaultRand;
		}
		else if (enemysShipsOnMap[enemyX - 1][enemyY] == Wound || enemysShipsOnMap[enemyX + 1][enemyY] == Wound)
		{
			deckNums = 0;//кол-во не подбитых палуб
			deckWoundNums = 0;//кол-во подбитых палуб

							  //если сверху или снизу есть не подбитые палубы
			if (enemysShipsOnMap[enemyX - 1][enemyY] == Wound)
			{
				for (int k = 1; k <= 3; k++)
					if ((enemyX - k) >= 0 && enemysShipsOnMap[enemyX - k][enemyY] != None)
					{
						if (enemysShipsOnMap[enemyX - k][enemyY] == Deck)
							deckNums++;
					}
					else break;
			}
			if (enemysShipsOnMap[enemyX + 1][enemyY] == Wound)
			{
				for (int k = 1; k <= 3; k++)
					if ((enemyX + k) <= 9 && enemysShipsOnMap[enemyX + k][enemyY] != None)
					{
						if (enemysShipsOnMap[enemyX + k][enemyY] == Deck)
							deckNums++;
					}
					else break;
			}

			//если есть не подбитые палубы, то просто раним палубу
			if (deckNums != 0)
			{
				enemy.map[enemyX][enemyY].setFillColor(sf::Color::Green);
				fireEnemy = hitLastShot;//попадание
				canShotOnMap[enemyX][enemyY] = false;//нельзя стрелять
				enemysShipsOnMap[enemyX][enemyY] = Wound;
			}
			else if (deckNums == 0)//иначе уничтожаем корабль
			{
				deckIsDestroyed(enemyX, enemyY, enemy);
				deckWoundNums = 1;

				//подсчет подбитых палуб по вертикали
				for (int a = 1; a <= 3; a++)
				{
					if ((enemyX - a) >= 0 && enemysShipsOnMap[enemyX - a][enemyY] != None)
					{
						if (enemysShipsOnMap[enemyX - a][enemyY] == Wound)
						{
							deckIsDestroyed(enemyX - a, enemyY, enemy);
							deckWoundNums++;
						}
					}
					else break;
				}
				for (int a = 1; a <= 3; a++)
				{
					if ((enemyX + a) >= 0 && enemysShipsOnMap[enemyX + a][enemyY] != None)
					{
						if (enemysShipsOnMap[enemyX + a][enemyY] == Wound)
						{
							deckIsDestroyed(enemyX + a, enemyY, enemy);
							deckWoundNums++;
						}
					}
					else break;
				}
			}

			//если корабль уничтожен
			if (deckNums == 0)
				fireEnemy = defaultRand;
		}
		numOfEnemyShips = 0;

		//проверяем остались ли еще корабли у противника
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
				if (enemysShipsOnMap[i][j] == Deck)
					numOfEnemyShips++;

	}
	else if (enemysShipsOnMap[enemyX][enemyY] == None)//если промах
	{
		enemy.map[enemyX][enemyY].setFillColor(sf::Color::Magenta);//цвет Magenta если промах
		canShotOnMap[enemyX][enemyY] = false;
		canShot = false;
		canEnemyShot = true;
		if (fireEnemy == hitLastShot)
			fireEnemy = thereAreWoundDeck;//есть не подбитые палубы, в след раз стрелять рядом
	}
}