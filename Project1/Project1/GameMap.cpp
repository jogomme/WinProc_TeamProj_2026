#include<iostream>
#include "GameMap.h"

GameMap::GameMap()
	: m_stage(1)
{

}

GameMap::~GameMap()
{

}

int GameMap::GetStage() const
{
	return m_stage;
}