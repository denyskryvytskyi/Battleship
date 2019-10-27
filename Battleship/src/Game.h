#pragma once
#include "Player.h"
#include "PlayerAI.h"

enum class e_GameCondition
{
	placingShips = 0, shot, gameover
};

enum class e_GameMode
{
	onePlayer = 1, twoPlayer, exitGame
};

class Game {
private:
	bool m_ShipsAreReady;
	RenderWindow m_Window;

	Player* m_Player1;
	Player* m_Player2;
	PlayerAI* m_PlayerAi;

	Vector2i m_MousePos; // mouse cursor position

	e_GameCondition m_GameCondition;
	e_GameMode m_GameMode;
public:
	Game(int playerMode) : m_Window(VideoMode(800, 400), "Battleship")
	{
		m_Player1 = new Player();
		m_Player2 = new Player();

		m_GameMode = (e_GameMode) playerMode;

		m_Player1->m_Camera.setViewport(FloatRect(0, 0, 0.5f, 1)); // left part of the screen
		m_Player1->m_Camera.reset(FloatRect(0, 0, 400, 400));
		m_Player1->m_CanShot = true;

		if (m_GameMode == e_GameMode::twoPlayer)
		{
			m_Player2 = new Player();
			m_Player2->m_Camera.setViewport(FloatRect(0.5f, 0, 0.5f, 1)); // right part of the screen
			m_Player2->m_Camera.reset(FloatRect(0, 0, 400, 400));
			m_Player2->m_CanShot = false;
		}
		else if (m_GameMode == e_GameMode::onePlayer)
		{
			m_PlayerAi = new PlayerAI();
			m_PlayerAi->m_Camera.setViewport(FloatRect(0.5f, 0, 0.5f, 1)); // right part of the screen
			m_PlayerAi->m_Camera.reset(FloatRect(0, 0, 400, 400));
			m_PlayerAi->m_CanShot = false;
			m_PlayerAi->addShipsOnMap();
			m_PlayerAi->setMapPlayerAiColor();
		}

		m_GameCondition = e_GameCondition::placingShips; 
		m_ShipsAreReady = false;
	}

	~Game()
	{
		delete m_Player1;

		if (m_Player2 != nullptr)
			delete m_Player2;
		if (m_PlayerAi != nullptr)
			delete m_PlayerAi;
	}

	void run(); // run the game
	void processEvents(); // Events handler
	void render();
	void renderPlayer(Player* p, int numPlayer);
	void renderFinish(short winner);
};