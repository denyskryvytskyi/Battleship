#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Map {
private:
	short m_Rows = 10;
	short m_Cols = 10;
public:
	void mapGenerate();
	RectangleShape** m_Map;

	Map();
	~Map();
};