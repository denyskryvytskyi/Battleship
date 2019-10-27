#include "Map.h"

Map::Map()
{
	m_Map = new RectangleShape * [m_Rows];
	for (int i = 0; i < m_Rows; i++)
	{
		m_Map[i] = new RectangleShape[m_Cols];
	}
}

Map::~Map()
{
	for (int i = 0; i < m_Rows; i++)
	{
		delete[] m_Map[i];
	}

	delete[] m_Map;
}

void Map::mapGenerate()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			m_Map[i][j].setSize(Vector2f(30, 30)); // cell size
			m_Map[i][j].setFillColor(Color(16, 16, 68));
			m_Map[i][j].setOutlineThickness(2);
			m_Map[i][j].setOutlineColor(Color(92, 94, 154));
			m_Map[i][j].setPosition((i + 2) * 30, (j + 2) * 30);
		}
	}
}


