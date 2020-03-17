/*
/ Для ИИ сделаем следующий алогритм
/ 1. Стройка кораблся происходит рандомно, но, рандомно выбирается только начальная точка
/ То-есть в цикле (пока не выбрали удачно) выбираем случайную ячеку карты, которая доступна
/ Дальше строим корабль от этой точки. То-есть выбираем рандомно следующую ячейку - направление доступно любое из 4
/ Остальные палубы (начиная с 3) строим только в выбранном направлении

*/





//#pragma once
//#include "Player.h"
//
//enum class e_FireOnEnemy
//{
//	defaultRand = 0, hitLastShot, thereAreWoundDeck
//};
//
//class PlayerAI : public Player {
//private:
//	e_FireOnEnemy m_FireState;
//
//public:
//	PlayerAI()
//	{
//		m_PlayerMap.mapGenerate();
//		setMapPlayerAiColor();
//		m_MaxNumberOfDecks = 20;
//
//		for (int i = 0; i < 10; i++)
//		{
//			for (int j = 0; j < 10; j++)
//			{
//				m_PlayerShipsOnMap[i][j] = e_CellConditions::None;
//				m_CanPut[i][j] = true;
//				m_CanShotOnMap[i][j] = true;
//				m_IsAI = true;
//				
//			}
//		}
//
//		m_FireState = e_FireOnEnemy::defaultRand;
//
//		m_NumOfEnemyShips = 1;
//		m_NumberOfTheDeck = 1;
//		
//		m_DeckNums = 0;
//		m_DeckWoundNums = 0;
//	}
//
//	virtual void addShipsOnMap();
//	virtual void shotInEnemy(RenderWindow& win, Vector2i p, bool& canEnemyShot, Map& enemy);
//
//	void randXY();
//};