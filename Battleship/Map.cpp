#include "Map.h"

void Map::mapGenerate()
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
		{
			map[i][j].setSize(sf::Vector2f(30, 30)); //размер квадратика
			map[i][j].setFillColor(sf::Color(16, 16, 68));//цвет
			map[i][j].setOutlineThickness(2);//толщина контура
			map[i][j].setOutlineColor(sf::Color(92, 94, 154));//цвета контура
			map[i][j].setPosition((i + 2) * 30, (j + 2) * 30);//позиция квадратика относительно камеры View игрока
		}
}