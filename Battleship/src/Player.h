#pragma once
#include "Map.h"

class Player {
protected:

	// Ships count
	unsigned short fourDeckShip = 1;
	unsigned short threeDeckShip = 2;
	unsigned short twoDeckShip = 3;
	unsigned short oneDeckShip = 4;

	int numberOfTheDeck; // number of the decks of current exposing ship [1;4]
	short numOfEnemyShips; // count of the remains enemy ships

	bool canPut[10][10]; // can put on the cell
	bool canShotOnMap[10][10]; // can shoot on the cell

	enum CellConditions
	{ 
		Deck = 1, Wound, None, Kill 
	}; 

	bool isItComp;

	int enemyX, enemyY; // coordinates of the last shooted enemy ship deck
	int deckNums; // number of existed decks
	int deckWoundNums; //  number of wounded decks
public:
	Font font;
	Text playerName;
	Text finishTextMessage; // Game Over Label

	Map playerMap;
	CellConditions playerShipsOnMap[10][10];
	CellConditions enemyShipsOnMap[10][10];

	int maxNumberOfDecks = 20;
	bool canShot;

	View view; // Camera
	
	Player():
		numberOfTheDeck(1),
		numOfEnemyShips(1),
		isItComp(false),
		enemyX(0),
		enemyY(0),
		deckNums(0),
		deckWoundNums(0)
	{
		playerMap.mapGenerate();

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				playerShipsOnMap[i][j] = None;
				canPut[i][j] = true;
				canShotOnMap[i][j] = true;
			}
		}

		font.loadFromFile("arial.ttf");

		playerName.setFont(font);
		playerName.setFillColor(Color::Red);
		playerName.setCharacterSize(24);

		finishTextMessage.setFont(font);
		finishTextMessage.setFillColor(Color::Red);
		finishTextMessage.setCharacterSize(30);
	}

	void setMapColor();

	void initShipOnMap(int s, int f, int decs);
	void addShipsOnMap(RenderWindow& win, Vector2i p, int movePos);
	void deckIsReady(int i, int j);

	virtual void shotInEnemy(RenderWindow& win, Vector2i p, int movePos, bool& canEnemyShot, Map& enemy);
	void deckIsDestroyed(int a, int b, Map& enemy);

	bool getIsItComp() { return isItComp; }
	short getNumOfEnemyShips() { return numOfEnemyShips; }
};