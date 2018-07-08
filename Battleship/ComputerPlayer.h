#pragma once
#include "Player.h"

//Класс игрок-компьютер
class ComputerPlayer : public Player {
private:
	enum fireOnEnemy { defaultRand = 0, hitLastShot, thereAreWoundDeck };//defaultRand-случайная, hitLastShot-попал последний выстрел, thereAreWoundDeck-есть подьитые палубы
	fireOnEnemy fireEnemy;//состояние стрельбы
public:
	ComputerPlayer()
	{
		playerMap.mapGenerate();//генерация карты
		setMapColor();//скрываем корабли
		maxNumberOfDecks = 20;//максимальное кол-во палуб - 20

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				ShipsOnMap[i][j] = None;
				canPut[i][j] = true;
				canShotOnMap[i][j] = true;
				isItComp = true;
				fireEnemy = defaultRand;
			}
		}

		numOfEnemyShips = 1;
		numberOfTheDeck = 1;//кол-во палуб на состояние расстановки одного корабля
		deckNums = 0;//кол-во не подбитых палуб
		deckWoundNums = 0;//кол-во подбитых палуб
	}
	virtual void addShipsOnMap();//расставляем корабли
	virtual void shotInEnemy(RenderWindow& win, Vector2i p, bool& canEnemyShot, Map& enemy);//стрельба по врагу
	void randXY();//случайная координаты для стрельбы
	int getEnemyX() { return enemyX; }
	int getEnemyY() { return enemyY; }
};