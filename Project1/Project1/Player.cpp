#include "GameObject.h"
#include "Player.h"
#include "Rock.h"
#include <iostream>
#include <math.h>

Player::Player()
{
	m_attackPower = 1;
	m_hp = 1;
	m_speed = 10;
	m_x = 0;
	m_y = 0;
	attackType = 0;
	for (int i = 0; i < MAX_ROCKS; i++)
	{
		m_length[i] = INF;
	}
}	

Player::~Player()
{

}

// 플레이어와 암석 사이의 거리 계산 함수
void Player::SetLength(Rock r)
{
	m_length[r.GetID()] = sqrt(pow(m_x - r.GetX(), 2) + pow(m_y - r.GetY(), 2));
}

// 최소거리에 있는 암석의 ID 반환 함수
int Player::GetMinLengthID()
{
	double copy[MAX_ROCKS];

	memcpy(copy, m_length, sizeof(m_length));

	qsort(copy, MAX_ROCKS, sizeof(double), [](const void* a, const void* b) {
		double da = *(double*)a;
		double db = *(double*)b;
		if (da < db) return -1;
		else if (da > db) return 1;
		else return 0;
		}
	);

	for (int i = 0; i < MAX_ROCKS; i++)
	{
		if (copy[0] == m_length[i])
		{
			return i;
		}
	}

	return -1; // 에러 처리
}

void Player::attack(Rock r)
{
	// 암석의 ID
	int rID = r.GetID();
	
	// 암석의 최소거리 ID
	int rMinID = GetMinLengthID();

	if (m_length[rID] <= m_size )
	{
		GetDemege(1);
		return;
	}

	// attckType이 0이면 기본 공격
	if (attackType == 0)
	{
		// 최소거리에 있는 암석이 공격받는 암석이면 데미지를 받는다.
		if (rID == rMinID && !m_length[rID] == -1)
		{
			r.GetDemege(m_attackPower);
		}
	}
	else if (attackType == 1) {

	}
}