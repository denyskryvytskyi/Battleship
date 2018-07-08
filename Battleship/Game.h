#pragma once
#include "Player.h"
#include "ComputerPlayer.h"

//Класс Игра
class Game {
private:
	bool shipsAreReady;//корабли расставлены
	RenderWindow mWindow; //Окно
	Player p1;//игрок один
	Player p2;//игрок два
	ComputerPlayer pC;//игрок-компьютер
	Vector2i pos;//координаты курсора мыши
	enum gameConditions { putShip = 0, shot, gameover };//состояние игры: putShip- расставляются корабли; shot - идет стрельба
	gameConditions gC;
	enum chooseGameMenu { onePlayer = 1, twoPlayer, exitGame };//выбор пункта меню
	chooseGameMenu menuSelection;
public:
	Game(short choiceMenu) : mWindow(sf::VideoMode(800, 400), "Battleship")
	{
		switch (choiceMenu)
		{
		case 1: menuSelection = onePlayer;
			break;
		case 2: menuSelection = twoPlayer;
			break;
		case 3: menuSelection = exitGame;
			break;
		}
		p1.view.setViewport(FloatRect(0, 0, 0.5f, 1));//левая часть окна
		p1.view.reset(FloatRect(0, 0, 400, 400));//задаем размер
		p1.canShot = true;//первый игрок может стрелять,когда выставит корабли

		if (menuSelection == twoPlayer)
		{
			p2.view.setViewport(FloatRect(0.5f, 0, 0.5f, 1));//правая часть окна
			p2.view.reset(FloatRect(0, 0, 400, 400));//задаем размер
			p2.canShot = false;//второй игрок не может стрелять
		}
		else if (menuSelection == onePlayer)//если на одного игрока, то компьютер автоматически расставляет корабли
		{
			pC.view.setViewport(FloatRect(0.5f, 0, 0.5f, 1));//правая часть окна
			pC.view.reset(FloatRect(0, 0, 400, 400));//задаем размер
			pC.canShot = false;//второй игрок не может стрелять
			pC.addShipsOnMap();//расставляем корабли
			pC.setMapColor();//скрываем корабли
		}

		gC = putShip; //состояние игры расставнока кораблей
		shipsAreReady = false;
	}
	void run();//Запуск игры
	void processEvents(); //Обработка действий
	void render(); //Отрисовка ALL
	void renderPlayer(Player &p, int numPlayer);//Отрисовка карт игроков
	void renderFinish(short winner);//вывод сообщение о победителе
};