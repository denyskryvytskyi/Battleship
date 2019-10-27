#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Map {
private:
	short rows = 10;
	short cols = 10;
public:
	void mapGenerate();
	sf::RectangleShape** map;

	Map();
	~Map();
};