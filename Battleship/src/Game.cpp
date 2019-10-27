#include "Game.h"

using namespace std;
void Game::run()
{
	while (mWindow.isOpen())
	{
		if (gameCondition == GameCondition::gameover)
			break;
		render();
		processEvents();
	}
}

void Game::render()
{
	mWindow.clear(Color::Yellow);

	renderPlayer(p1, 1);

	if (gameMode == GameMode::twoPlayer)
		renderPlayer(p2, 2);
	else if (gameMode == GameMode::onePlayer)
		renderPlayer(pC, 3);

	mWindow.display();
}

void Game::processEvents()
{
	Event event;
	while (mWindow.pollEvent(event))
	{
		if (event.type == Event::Closed || gameMode == GameMode::exitGame)
			mWindow.close();

		if (p1.getNumOfEnemyShips() == 0 || p2.getNumOfEnemyShips() == 0 || pC.getNumOfEnemyShips() == 0)
		{
			gameCondition = GameCondition::gameover;
			if (!p1.getNumOfEnemyShips())
			{
				renderFinish(1);
			}
			else if (!p2.getNumOfEnemyShips() && gameMode == GameMode::twoPlayer)
				renderFinish(2);
			else if (!pC.getNumOfEnemyShips())
				renderFinish(3);
			break;
		}
		// All ships are ready
		if (p1.maxNumberOfDecks == 0 && (p2.maxNumberOfDecks == 0 || pC.maxNumberOfDecks == 0) && gameCondition == GameCondition::placingShips)
		{
			shipsAreReady = true;
			gameCondition = GameCondition::shot;

			for (int i = 0; i < 10; i++)
				for (int j = 0; j < 10; j++)
				{
					if (gameMode == GameMode::twoPlayer)
					{
						p1.enemyShipsOnMap[i][j] = p2.playerShipsOnMap[i][j];
						p2.enemyShipsOnMap[i][j] = p1.playerShipsOnMap[i][j];
					}
					else if (gameMode == GameMode::onePlayer)
					{
						p1.enemyShipsOnMap[i][j] = pC.playerShipsOnMap[i][j];
						pC.enemyShipsOnMap[i][j] = p1.playerShipsOnMap[i][j];
					}
				}
		}
		// Placing ships
		if ((event.type == Event::MouseButtonPressed) && (shipsAreReady == false))
			if (event.key.code == Mouse::Left)
			{
				if (p1.maxNumberOfDecks > 0)
					p1.addShipsOnMap(mWindow, mousePos, 0);
				else if (gameMode == GameMode::twoPlayer || p2.maxNumberOfDecks > 0) //Для второго.
					p2.addShipsOnMap(mWindow, mousePos, 400);
			}

		// Hide player's ships
		if (event.type == Event::KeyPressed)
			if (event.key.code == Keyboard::R)
			{
				if (p1.maxNumberOfDecks == 0)
					p1.setMapColor();
				if (p2.maxNumberOfDecks == 0)
					p2.setMapColor();
			}

		// player shooting
		if ((event.type == Event::MouseButtonPressed) && (shipsAreReady == true))
			if (event.key.code == Mouse::Left)
			{
				if (p1.canShot)
				{
					if (gameMode == GameMode::twoPlayer)
						p1.shotInEnemy(mWindow, mousePos, 400, p2.canShot, p2.playerMap);
					else if (gameMode == GameMode::onePlayer)
						p1.shotInEnemy(mWindow, mousePos, 400, pC.canShot, pC.playerMap);
				}
				else if (gameMode == GameMode::twoPlayer || p2.canShot)
					p2.shotInEnemy(mWindow, mousePos, 0, p1.canShot, p1.playerMap);
			}

		// AI shooting
		if (pC.canShot)
		{
			pC.shotInEnemy(mWindow, mousePos, p1.canShot, p1.playerMap);
		}
	}
}
void Game::renderPlayer(Player &p, int numPlayer)
{
	mWindow.setView(p.view);

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
		{
			mWindow.draw(p.playerMap.map[i][j]); // draw one cell
		}

	if (numPlayer == 1)
		p.playerName.setString("Player 1");
	else if (numPlayer == 2)
		p.playerName.setString("Player 2");

	if (p.getIsItComp())
		p.playerName.setString("Computer");

	p.playerName.setPosition(p.view.getCenter().x - 25, 0);
	mWindow.draw(p.playerName);
}

void Game::renderFinish(short winner)
{
	RenderWindow win(VideoMode(250, 60), "Battleship winner");

	while (win.isOpen())
	{
		Event event;
		while (win.pollEvent(event))
		{
			if (event.type == Event::Closed || gameMode == GameMode::exitGame)
				win.close();
		}
		Font font;
		font.loadFromFile("arial.ttf");
		Text t;
		t.setFont(font);
		t.setFont(font);
		t.setFillColor(Color::Red);

		t.setCharacterSize(20);
		string str;

		switch (winner)
		{
		case 1: str = "Player 1";
			break;
		case 2:	str = "Player 2";
			break;
		case 3: str = "Computer";
			break;
		}
		str += " is winner!";
		t.setString(str);

		win.clear();
		win.draw(t);
		win.display();
	}
}