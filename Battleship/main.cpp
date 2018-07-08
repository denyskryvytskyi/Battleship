#include <iostream>
#include "Class.h"
#include "Player.h"
#include "Map.h"
#include "ComputerPlayer.h"
#include "Game.h"
using namespace std;
int main()
{
	setlocale(LC_ALL, "rus");

	short chooseMenu;//пункт меню

	cout << "----------------------------- СПРАВКА -----------------------------\n";
	cout << "Корабли расставляются в таком порядке:\n";
	cout << "- Один 4-х палубный\n";
	cout << "- Два 3-х палубных\n";
	cout << "- Три 2-х палубных\n";
	cout << "- Четыре 1-о палубных\n";
	cout << "R - скрыть корабли, когда расставили.\n";
	cout << "Зеленый квадратик - корабль подбит\n";
	cout << "Красный квадратик - корабль уничтожен\n";
	cout << "Фиолетовый квадратик - промах\n";

	do {
		cout << "\n----------------------------- МЕНЮ --------------------------------\n";
		cout << "1. Один игрок\n";
		cout << "2. Два игрока\n";
		cout << "3. Выход\n";
		cout << "Введите ваш выбор:";
		try {
			if (!(cin >> chooseMenu) || cin.fail() || chooseMenu < 1 || chooseMenu > 3)
				throw 1;

			if (chooseMenu == 3)//выход
				return 0;

			Game game(chooseMenu);
			game.run();
		}
		catch (int)
		{
			cout << "Ошибка ввода! Попробуйте снова.\n";
			exit(1);
		}
	} while (chooseMenu != 3);

	return 0;
}
