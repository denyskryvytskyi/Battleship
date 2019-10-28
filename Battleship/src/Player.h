#pragma once
#include "Map.h"

enum class e_CellConditions
{
	Deck = 1, Wound, None, Kill
};

class Player {
protected:
	// Ships count
	unsigned short m_FourDeckShip = 1;
	unsigned short m_ThreeDeckShip = 2;
	unsigned short m_TwoDeckShip = 3;
	unsigned short m_OneDeckShip = 4;

	int m_NumberOfTheDeck; // number of the decks of current exposing ship [1;4]
	short m_NumOfEnemyShips; // count of the remains enemy ships

	static const int m_Rows = 10;
	static const int m_Cols = 10;

	bool m_CanPut[m_Rows][m_Cols]; // can put on the cell
	bool m_CanShotOnMap[m_Rows][m_Cols]; // can shoot on the cell

	bool m_IsAI;

	int m_EnemyX, m_EnemyY; // coordinates of the last shooted enemy ship deck
	int m_DeckNums; // number of existed decks
	int m_DeckWoundNums; //  number of wounded decks

public:
	Font m_Font;
	Text m_PlayerName;
	Text m_FinishTextMessage; // Game Over Label

	Map m_PlayerMap;
	e_CellConditions m_PlayerShipsOnMap[m_Rows][m_Cols];
	e_CellConditions m_EnemyShipsOnMap[m_Rows][m_Cols];

	int m_MaxNumberOfDecks = 20;
	bool m_CanShot;

	View m_Camera; // Camera
	
	Player():
		m_NumberOfTheDeck(1),
		m_NumOfEnemyShips(1),
		m_IsAI(false),
		m_EnemyX(0),
		m_EnemyY(0),
		m_DeckNums(0),
		m_DeckWoundNums(0),
		m_CanShot(false),
		m_CanPut{ true },
		m_CanShotOnMap{true},
		m_PlayerShipsOnMap{ e_CellConditions::None },
		m_EnemyShipsOnMap()
	{
		m_PlayerMap.mapGenerate();

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				m_PlayerShipsOnMap[i][j] = e_CellConditions::None;
				m_CanPut[i][j] = true;
				m_CanShotOnMap[i][j] = true;
			}
		}

		m_Font.loadFromFile("arial.ttf");

		m_PlayerName.setFont(m_Font);
		m_PlayerName.setFillColor(Color::Blue);
		m_PlayerName.setCharacterSize(24);
		
		m_FinishTextMessage.setFont(m_Font);
		m_FinishTextMessage.setFillColor(Color::Red);
		m_FinishTextMessage.setCharacterSize(30);
	}

	void setMapPlayerAiColor();

	void initShipOnMap(int s, int f, int decs);
	void addShipsOnMap(RenderWindow& win, Vector2i p, int movePos);
	void deckIsReady(int i, int j);

	virtual void shotInEnemy(RenderWindow& win, Vector2i p, int movePos, bool& canEnemyShot, Map& enemy);
	void deckIsDestroyed(int a, int b, Map& enemy);

	bool getIsItComp() { return m_IsAI; }
	short getNumOfEnemyShips() { return m_NumOfEnemyShips; }
};