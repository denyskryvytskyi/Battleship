#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

//Класс карта
class Map {
public:
	void mapGenerate();// генерация карты
	sf::RectangleShape map[10][10]; //Карта, матрица квадратов
};