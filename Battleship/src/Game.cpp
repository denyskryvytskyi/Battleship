#include "Game.h"

using namespace std;
void Game::run()
{
	while (m_Window.isOpen())
	{
		if (m_GameCondition == e_GameCondition::gameover)
			break;
		render();
		processEvents();
	}
}

void Game::render()
{
	m_Window.clear(Color(260, 220, 180));

	renderPlayer(m_Player1, 1);

	if (m_GameMode == e_GameMode::twoPlayer)
		renderPlayer(m_Player2, 2);
	else if (m_GameMode == e_GameMode::onePlayer)
		renderPlayer(m_PlayerAi, 3);

	m_Window.display();
}

void Game::processEvents()
{
	Event event;
	while (m_Window.pollEvent(event))
	{
		if (event.type == Event::Closed || m_GameMode == e_GameMode::exitGame)
			m_Window.close();

		if ((m_Player1 && m_Player1->getNumOfEnemyShips() == 0) || (m_Player2 && m_Player2->getNumOfEnemyShips() == 0) || (m_PlayerAi && m_PlayerAi->getNumOfEnemyShips() == 0))
		{
			m_GameCondition = e_GameCondition::gameover;
			if (!m_Player1->getNumOfEnemyShips())
			{
				renderFinish(1);
			}
			else if (!m_Player2->getNumOfEnemyShips() && m_GameMode == e_GameMode::twoPlayer)
				renderFinish(2);
			else if (!m_PlayerAi->getNumOfEnemyShips())
				renderFinish(3);
			break;
		}
		// All ships are ready
		if ((m_Player1 && m_Player1->m_MaxNumberOfDecks == 0) && ((m_Player2 && m_Player2->m_MaxNumberOfDecks == 0) || (m_PlayerAi && m_PlayerAi->m_MaxNumberOfDecks == 0)) 
			&& m_GameCondition == e_GameCondition::placingShips)
		{
			m_ShipsAreReady = true;
			m_GameCondition = e_GameCondition::shot;

			for (int i = 0; i < 10; i++)
				for (int j = 0; j < 10; j++)
				{
					if (m_GameMode == e_GameMode::twoPlayer)
					{
						m_Player1->m_EnemyShipsOnMap[i][j] = m_Player2->m_PlayerShipsOnMap[i][j];
						m_Player2->m_EnemyShipsOnMap[i][j] = m_Player1->m_PlayerShipsOnMap[i][j];
					}
					else if (m_GameMode == e_GameMode::onePlayer)
					{
						m_Player1->m_EnemyShipsOnMap[i][j] = m_PlayerAi->m_PlayerShipsOnMap[i][j];
						m_PlayerAi->m_EnemyShipsOnMap[i][j] = m_Player1->m_PlayerShipsOnMap[i][j];
					}
				}
		}
		// Placing ships
		if ((event.type == Event::MouseButtonPressed) && (m_ShipsAreReady == false))
			if (event.key.code == Mouse::Left)
			{
				if (m_Player1 && m_Player1->m_MaxNumberOfDecks > 0)
					m_Player1->addShipsOnMap(m_Window, m_MousePos, 0);
				else if (m_Player2 && (m_GameMode == e_GameMode::twoPlayer || m_Player2->m_MaxNumberOfDecks > 0)) // for the second
					m_Player2->addShipsOnMap(m_Window, m_MousePos, 400);
			}

		// Hide player's ships
		if (event.type == Event::KeyPressed)
			if (event.key.code == Keyboard::R)
			{
				if (m_Player1 && m_Player1->m_MaxNumberOfDecks == 0)
					m_Player1->setMapPlayerAiColor();
				if (m_Player2 && m_Player2->m_MaxNumberOfDecks == 0)
					m_Player2->setMapPlayerAiColor();
			}

		// player shooting
		if ((event.type == Event::MouseButtonPressed) && (m_ShipsAreReady == true))
			if (event.key.code == Mouse::Left)
			{
				if (m_Player1 && m_Player1->m_CanShot)
				{
					if (m_GameMode == e_GameMode::twoPlayer && m_Player2)
						m_Player1->shotInEnemy(m_Window, m_MousePos, 400, m_Player2->m_CanShot, m_Player2->m_PlayerMap);
					else if (m_GameMode == e_GameMode::onePlayer && m_PlayerAi)
						m_Player1->shotInEnemy(m_Window, m_MousePos, 400, m_PlayerAi->m_CanShot, m_PlayerAi->m_PlayerMap);
				}
				else if (m_Player1 && m_Player2 && (m_GameMode == e_GameMode::twoPlayer || m_Player2->m_CanShot))
					m_Player2->shotInEnemy(m_Window, m_MousePos, 0, m_Player1->m_CanShot, m_Player1->m_PlayerMap);
			}

		// AI shooting
		if (m_PlayerAi && m_PlayerAi->m_CanShot)
		{
			if (m_Player1)
			{
				m_PlayerAi->shotInEnemy(m_Window, m_MousePos, m_Player1->m_CanShot, m_Player1->m_PlayerMap);
			}
		}
	}
}
void Game::renderPlayer(Player* p, int numPlayer)
{
	m_Window.setView(p->m_Camera);

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
		{
			m_Window.draw(p->m_PlayerMap.m_Map[i][j]); // draw one cell
		}

	if (numPlayer == 1)
		p->m_PlayerName.setString("Player 1");
	else if (numPlayer == 2)
		p->m_PlayerName.setString("Player 2");

	if (p->getIsItComp())
		p->m_PlayerName.setString("Computer");

	p->m_PlayerName.setPosition(p->m_Camera.getCenter().x - 25, 0);
	m_Window.draw(p->m_PlayerName);
}

void Game::renderFinish(short winner)
{
	RenderWindow win(VideoMode(250, 60), "Battleship winner");

	while (win.isOpen())
	{
		Event event;
		while (win.pollEvent(event))
		{
			if (event.type == Event::Closed || m_GameMode == e_GameMode::exitGame)
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