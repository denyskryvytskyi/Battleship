#pragma once
#include "Player.h"
#include "ComputerPlayer.h"

enum class GameCondition
{
	placingShips = 0, shot, gameover
};

enum class GameMode
{
	onePlayer = 1, twoPlayer, exitGame
};

class Game {
private:
	bool shipsAreReady;
	RenderWindow mWindow;

	Player p1;
	Player p2;
	ComputerPlayer pC;

	Vector2i mousePos; // mouse cursor position

	GameCondition gameCondition;
	GameMode gameMode;
public:
	Game(int playerMode) : mWindow(sf::VideoMode(800, 400), "Battleship")
	{
		gameMode = (GameMode) playerMode;

		p1.view.setViewport(FloatRect(0, 0, 0.5f, 1)); // left part of the screen
		p1.view.reset(FloatRect(0, 0, 400, 400));
		p1.canShot = true;

		if (gameMode == GameMode::twoPlayer)
		{
			p2.view.setViewport(FloatRect(0.5f, 0, 0.5f, 1)); // right part of the screen
			p2.view.reset(FloatRect(0, 0, 400, 400));
			p2.canShot = false;
		}
		else if (gameMode == GameMode::onePlayer)
		{
			pC.view.setViewport(FloatRect(0.5f, 0, 0.5f, 1)); // right part of the screen
			pC.view.reset(FloatRect(0, 0, 400, 400));
			pC.canShot = false;
			pC.addShipsOnMap();
			pC.setMapColor();
		}

		gameCondition = GameCondition::placingShips; 
		shipsAreReady = false;
	}

	void run(); // run the game
	void processEvents(); // Events handler
	void render();
	void renderPlayer(Player &p, int numPlayer);
	void renderFinish(short winner);
};