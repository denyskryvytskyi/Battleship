#include <iostream>
#include "Player.h"
#include "Map.h"
#include "ComputerPlayer.h"
#include "Game.h"

using namespace std;

int main()
{
	short menuItem;

	cout << "----------------------------- HELP -----------------------------\n";
	cout << "The ships are placed in the next order:\n";
	cout << "one 4-deck - the one first" << endl;
	cout << "two 3-deck" << endl;
	cout << "three 2-deck" << endl;
	cout << "four 1-deck" << endl;
	cout << "R - hide ships, when they was placed" << endl;
	cout << "Green deck - wounded" << endl;
	cout << "Red deck - destroyed" << endl;
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
