#include <iostream>
#include "GameManager.h"

int main()
{
	int gameMode = 0;

	std::cout << "----------------------------- HELP -----------------------------" << std::endl;
	std::cout << "The ships are placed in the next order:" << std::endl;
	std::cout << "1. one 4-deck" << std::endl;
	std::cout << "2. two 3-deck" << std::endl;
	std::cout << "3. three 2-deck" << std::endl;
	std::cout << "4. four 1-deck" << std::endl;
	std::cout << "H - hide ships when they was placed" << std::endl;
	std::cout << "Green deck - ship is wounded" << std::endl;
	std::cout << "Red deck - ship is destroyed" << std::endl;
	std::cout << "Purple deck - missed" << std::endl;

	do {
		std::cout << "----------------------------- MENU --------------------------------" << std::endl;
		std::cout << "1. Single player" << std::endl;
		std::cout << "2. Two players" << std::endl;
		std::cout << "3. Exit" << std::endl;
		std::cout << "Your choice:";

		std::cin >> gameMode;
		
		if (!std::cin.fail() && (gameMode == 1 || gameMode == 2))
		{
			GameManager gameManager(gameMode - 1);
			gameManager.Start();
		}
		
	} while (gameMode != 3);

	return 0;
}