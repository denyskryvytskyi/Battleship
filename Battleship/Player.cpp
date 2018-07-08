#include "Player.h"

void Player::addShipsOnMap(RenderWindow& win, Vector2i p, int movePos)
{
	p = Mouse::getPosition(win); //Считываем координаты курсора мыши
	p.x -= movePos; //Если расставляем для второго игрока, то отнимаем от координаты х курсора 400

	for (int k = 0; k < 10; k++)
	{
		for (int l = 0; l < 10; l++)
		{
			if (playerMap.map[k][l].getGlobalBounds().contains(p.x, p.y) && ShipsOnMap[k][l] == None && canPut[k][l] == true) // если квадратик содержит координаты курсора
			{
				//расставляем палубы
				if (fourDeckShip != 0)
				{
					initShipOnMap(k, l, 4);//передаем кол-во палуб
				}
				else if (threeDeckShip != 0 && fourDeckShip == 0)
				{
					if (numberOfTheDeck == 4) numberOfTheDeck = 1;
					initShipOnMap(k, l, 3);//передаем кол-во палуб
				}
				else if (twoDeckShip != 0 && threeDeckShip == 0)
				{
					if (numberOfTheDeck == 3) numberOfTheDeck = 1;
					initShipOnMap(k, l, 2);//передаем кол-во палуб
				}
				else if (oneDeckShip != 0 && twoDeckShip == 0)
				{
					deckIsReady(k, l);
					ShipsOnMap[k][l] = Deck;
					//вокруг корябля ставить нельзя
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
			playerMap.map[i][j].setFillColor(sf::Color(16, 16, 68));//цвет
		}
}
void Player::deckIsReady(int i, int j)
{
	playerMap.map[i][j].setFillColor(sf::Color(100, 100, 100)); //ставим корабль(красим квадратик)
	ShipsOnMap[i][j] = Deck; //ПАЛУБА ГОТОВА
	maxNumberOfDecks--;//минус 1 возможная палуба

					   //по диагонали теперь ставить нельзя
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
	enemysShipsOnMap[a][b] = Kill;//уничтожаем
	enemy.map[a][b].setFillColor(sf::Color::Red);//цвет Red если уничтожен
}
void Player::initShipOnMap(int f, int s, int decs)
{
	if (numberOfTheDeck == 1)
	{
		numberOfTheDeck++;
		deckIsReady(f, s);
	}
	else if ((numberOfTheDeck > 1) && (numberOfTheDeck < decs) &&
		((ShipsOnMap[f - 1][s] == Deck) ||
		(ShipsOnMap[f + 1][s] == Deck) ||
			(ShipsOnMap[f][s - 1] == Deck) ||
			(ShipsOnMap[f][s + 1] == Deck)))
	{
		deckIsReady(f, s);
		numberOfTheDeck++;
	}

	else if (numberOfTheDeck == decs && (ShipsOnMap[f - 1][s] == Deck || ShipsOnMap[f + 1][s] == Deck || ShipsOnMap[f][s - 1] == Deck || ShipsOnMap[f][s + 1] == Deck))
	{
		deckIsReady(f, s);
		numberOfTheDeck = 1;
		switch (decs) {
		case 2: twoDeckShip--;//минус 2-х палубный корабль
			break;
		case 3: threeDeckShip--;//минус 3-х палубный корабль
			break;
		case 4: fourDeckShip--;//минус 4-х палубный корабль
			break;
		}

		//обосабливаем корабль
		if (ShipsOnMap[f - 1][s] == Deck)
		{
			if ((f + 1) <= 9)
				canPut[f + 1][s] = false;
			if ((f - decs) >= 0)
				canPut[f - decs][s] = false;
		}
		else if (ShipsOnMap[f + 1][s] == Deck)
		{
			if ((f - 1) >= 0)
				canPut[f - 1][s] = false;
			if ((f + decs) <= 9)
				canPut[f + decs][s] = false;
		}
		else if (ShipsOnMap[f][s - 1] == Deck)
		{
			if ((s + 1) <= 9)
				canPut[f][s + 1] = false;
			if ((s - decs) >= 0)
				canPut[f][s - decs] = false;
		}
		else if (ShipsOnMap[f][s + 1] == Deck)
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
	p = Mouse::getPosition(win); //Считываем координаты курсора мыши
	p.x -= movePos; //Если для второго игрока, то отнимаем от координаты х курсора 400

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
		{
			if (enemy.map[i][j].getGlobalBounds().contains(p.x, p.y))//если курсор попадает на карту кораблей
			{
				enemyX = i; enemyY = j;
				if (enemysShipsOnMap[i][j] == Deck)//если попали в палубу
				{
					//если рядом нет палуб
					if ((((enemyX - 1) >= 0 && enemysShipsOnMap[enemyX - 1][enemyY] == None) || (enemyX - 1) < 0) && (((enemyX + 1) <= 9 && enemysShipsOnMap[enemyX + 1][enemyY] == None) || (enemyX + 1) > 9) && (((enemyY - 1) >= 0 && enemysShipsOnMap[enemyX][enemyY - 1] == None) || (enemyY - 1) < 0) && (((enemyY + 1) <= 9 && enemysShipsOnMap[enemyX][enemyY + 1] == None) || (enemyY + 1) > 9))
					{
						deckIsDestroyed(enemyX, enemyY, enemy);//если рядом нету палуб - уничтожаем корабль
						oneDeckShip--;//минус один однопалубный корабль
					}
					//если рядом палуба
					else if (((enemyX - 1) >= 0 && enemysShipsOnMap[enemyX - 1][enemyY] == Deck) || ((enemyX + 1) <= 9 && enemysShipsOnMap[enemyX + 1][enemyY] == Deck) || ((enemyY - 1) >= 0 && enemysShipsOnMap[enemyX][enemyY - 1] == Deck) || ((enemyY + 1) <= 9 && enemysShipsOnMap[enemyX][enemyY + 1] == Deck))
					{
						enemy.map[enemyX][enemyY].setFillColor(sf::Color::Green);//цвет Green если ранен
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
					}
					numOfEnemyShips = 0;
					//проверяем остались ли еще корабли у противника
					for (int i = 0; i < 10; i++)
						for (int j = 0; j < 10; j++)
							if (enemysShipsOnMap[i][j] == Deck)
								numOfEnemyShips++;
				}
				//если промах
				else if (enemysShipsOnMap[i][j] == None)
				{
					enemy.map[i][j].setFillColor(sf::Color::Magenta);//цвет Magenta если промах
					canShot = false;
					canEnemyShot = true;
				}
			}
		}
}