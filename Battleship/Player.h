#include "Map.h"
#pragma once
#include "Map.h"

//Класс Игрока
class Player {
protected:
	unsigned int fourDeckShip = 1;//кол-во 4-х палубных кораблей
	unsigned int threeDeckShip = 2;//кол-во 3-х палубных кораблей
	unsigned int twoDeckShip = 3;//кол-во 2-х палубных кораблей
	unsigned int oneDeckShip = 4;//кол-во 1-о палубных кораблей
	int numberOfTheDeck;//кол-во палуб на состояние расстановки одного корабля (от 1 до 4)
	short numOfEnemyShips;//кол-во оставшихся кораблей противника, используется для окончании игры
	bool canPut[10][10];//можно ставить палубу или нет
	bool canShotOnMap[10][10];//можно стрелять или нет
	enum decksConditions { Deck = 1, Wound, None, Kill };//Состояние палубы: deck - стоит, wound - ранена, none - мимо(пусто), kill - уничтожена 
	bool isItComp;//игрок - комп. или нет
	int enemyX, enemyY;//координаты вражеской палубы, в которую попали
	int deckNums;//кол-во не подбитых палуб
	int deckWoundNums;//кол-во подбитых палуб
public:
	Font font;//шрифт
	Text text;//подпись игрока
	Text finishTextMessage;//сообщение об окончании игры
	decksConditions ShipsOnMap[10][10];//палубы кораблей
	decksConditions enemysShipsOnMap[10][10];//палубы кораблей противника
	int maxNumberOfDecks;//макс. кол-во палуб
	bool canShot;//разрешение на стрельбу
	View view;//камера
	Map playerMap;//карта квадратиков

	Player() {
		playerMap.mapGenerate();//генерация карты
		maxNumberOfDecks = 20;//максимальное кол-во палуб - 20
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				ShipsOnMap[i][j] = None;
				canPut[i][j] = true;
				canShotOnMap[i][j] = true;
				isItComp = false;
			}
		}

		numberOfTheDeck = 1;//кол-во палуб на состояние расстановки одного корабля
		numOfEnemyShips = 1;
		font.loadFromFile("arial.ttf");

		//название игрока
		text.setFont(font);
		text.setFillColor(Color::Red);
		text.setCharacterSize(24);

		//сообщение в конце игры
		finishTextMessage.setFont(font);
		finishTextMessage.setFillColor(Color::Red);
		finishTextMessage.setCharacterSize(30);
	}

	void setMapColor();//цвет карты
	void addShipsOnMap(RenderWindow& win, Vector2i p, int movePos);//Взаимодействие с картой, например добавление кораблей
	virtual void shotInEnemy(RenderWindow& win, Vector2i p, int movePos, bool& canEnemyShot, Map& enemy);//стрельба по врагу
	void initShipOnMap(int s, int f, int decs);
	void deckIsReady(int i, int j);//палуба готова
	void deckIsDestroyed(int a, int b, Map& enemy);//уничтожаем палубу
	bool getIsItComp() { return isItComp; }
	short getNumOfEnemyShips() { return numOfEnemyShips; }
};