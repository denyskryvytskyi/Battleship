#include <iostream>
#include "Player.h"
#include "Map.h"
#include "PlayerAI.h"
#include "Game.h"

using namespace std;

int main()
{
	short menuItem;

	cout << "----------------------------- HELP -----------------------------" << endl;
	cout << "The ships are placed in the next order:" << endl;
	cout << "1. one 4-deck" << endl;
	cout << "2. two 3-deck" << endl;
	cout << "3. three 2-deck" << endl;
	cout << "4. four 1-deck" << endl;
	cout << "R - hide ships when they was placed" << endl;
	cout << "Green deck - ship is wounded" << endl;
	cout << "Red deck - ship is destroyed" << endl;
	cout << "Purple deck - miss" << endl;

	do {
		cout << "----------------------------- MENU --------------------------------" << endl;
		cout << "1. Single player" << endl;
		cout << "2. Two players" << endl;
		cout << "3. Exit" << endl;
		cout << "Your choice:";

		cin >> menuItem;
		
		if (!cin.fail() && (menuItem == 1 || menuItem == 2))
		{
			Game game(menuItem);
			game.run();
		}
		
	} while (menuItem != 3);

	return 0;
}