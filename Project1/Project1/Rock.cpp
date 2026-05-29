#include <iostream>
#include "Rock.h"
#include "GameObject.h"
#include "GameMap.h"
#include <time.h>
#include<random>

#define width 1280
#define height 800

std::default_random_engine generate(time(NULL)); // 난수 생성기 초기화
std::uniform_int_distribution<int> minRockDist(0, 3); // 0, 1, 2 중에서 랜덤한 정수를 생성하는 분포

// 암석의 종류는 암석의 종류가 n 개이면 마지막을 n-1로 초기화
std::uniform_int_distribution<int> rockTypeDist(0, 3);

// 암석의 속도를 랜덤하게 설정하기 위한 분포
std::uniform_int_distribution<int> speedDist(1, 5);

// 암석의 위치 생성
std::uniform_int_distribution<int> rockPos(1, 4);

// x 좌표에서 무작위 뽑기
std::uniform_int_distribution<int> xPosDist(0, 1280);

// y 좌표에서 무작위 뽑기
std::uniform_int_distribution<int> yPosDist(0, 800);

// 운석 생성시 생기는 공간 텀
std::uniform_int_distribution<int> PlusPosDist(0, 30);

int Rock::gid{ 0 }; // 암석 고유 번호를 위한 변수 초기화

Rock::Rock()
	: id{ gid++ }
{
	m_size = 20;
}

Rock::~Rock()
{
	// 암석이 부숴질 때, 해당 암석의 종류에 맞는 갯수를 스테이지에 맞게 증가시킴
	GameMap::m_rock[m_RockType].Num = minRockDist(generate) + GetStage() / 2;
	GameMap::m_rockNum++;
}

void Rock::Spawn()
{
	// 지금 해금된 암석 종류에 맞춰서 랜덤값으로 암석의 종류 선정
	while (true) {
		m_RockType = rockTypeDist(generate);

		if (m_unlockType[m_RockType] == 1) {
			break;
		}
	}

	// 암석의 종류에 맞춰서 체력과 가격 설정
	if (m_RockType == 0) {
		m_hp = 1 + GetStage() * 1.2;
		m_price = 1;
	}
	else if (m_RockType == 1) {
		m_hp = 2 + GetStage() * 1.2;
		m_price = 5;
	}
	else if (m_RockType == 2) {
		m_hp = 3 + GetStage() * 1.2;
		m_price = 15;
	}
	else if (m_RockType == 3) {
		m_hp = 4 + GetStage() * 1.2;
		m_price = 50;
	}

	SetRockPos();
	setDirection(xPosDist(generate), yPosDist(generate));

	// 암석의 속도 랜덤으로 설정
	m_speed = speedDist(generate);
}

int Rock::GetID() const
{
	return id;
}

void Rock::setDirection(int X, int Y)
{
	// 랜덤한 값 2개를 잡고 그것을 방향 벡터로 잡는 함수.

	double dx = X - m_x;
	double dy = Y - m_y;
	double distance = sqrt(dx * dx + dy * dy);

	// (0으로 나누는 에러 방지)
	if (distance == 0) return;

	direction[0] = dx / distance;
	direction[1] = dy / distance;

}

// 암석이 이동하는 함수. setDirection으로 설정한 방향벡터에 속도를 곱해서 이동.
void Rock::Move(double x, double y)
{
	// 암석이 이동하는 함수. setDirection으로 설정한 방향벡터에 속도를 곱해서 이동.
	m_x += direction[0] * m_speed;
	m_y += direction[1] * m_speed;
}

void Rock::UnlockRockType(int rockType)
{
	if (rockType >= 0 && rockType < m_MaxRockType) {
		m_unlockType[rockType] = 1;
	}
}

void Rock::SetRockPos()
{
	int pos = rockPos(generate);

	// 상
	if (pos == 1) {
		m_y = 0 - PlusPosDist(generate);
		m_x = xPosDist(generate);
	}
	// 하
	else if (pos == 2) {
		m_y = height + PlusPosDist(generate);
		m_x = xPosDist(generate);
	}
	//좌
	else if (pos == 3) {
		m_y = yPosDist(generate);
		m_x = 0 - PlusPosDist(generate);
	}
	//우
	else if (pos == 4) {
		m_y = yPosDist(generate);
		m_x = width + PlusPosDist(generate);
	}
}