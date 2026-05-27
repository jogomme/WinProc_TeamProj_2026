#include <iostream>
#include "Rock.h"
#include "GameObject.h"
#include "GameMap.h"
#include <time.h>

int Rock::gid{ 0 }; // 암석 고유 번호를 위한 변수 초기화

Rock::Rock()
	: m_price(100), m_RockType(0), id{gid++}
{
	m_size = 1;
}

Rock::~Rock()
{
	srand(time(NULL)); // 매번 다른 난수가 나오도록 시드값 초기화
	// 암석이 부숴질 때, 해당 암석의 종류에 맞는 갯수를 스테이지에 맞게 증가시킴
	GameMap::m_rock[m_RockType].Num = rand() % 3 + GetStage() / 2;
}

int Rock::GetID() const
{
	return id;
}