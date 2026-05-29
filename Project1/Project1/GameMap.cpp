#include<iostream>
#include "GameMap.h"

GameMap::GameMap()
	: m_stage(1), TickRate(16) // 60 프레임으로 설정
{
	m_goal = 100;
	m_rockNum = 0;
}

GameMap::~GameMap()
{

}

int GameMap::GetStage() const
{
	return m_stage;
}

int GameMap::GetTickRate() const
{
	return TickRate;
}

void GameMap::SetStage(int stage)
{
	m_stage = stage;
}

void GameMap::NextStage()
{
	if (m_goal <= m_rockNum)
	{
		m_stage++;
		m_rockNum = 0;
	}
}
