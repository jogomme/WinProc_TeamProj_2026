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
	m_size = 50;
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

void Player::Move(double targetX, double targetY)
{
	// 1. 현재 위치에서 목표 위치를 바라보는 '방향 벡터(거리 차이)' 계산
	double dx = targetX - m_x;
	double dy = targetY - m_y;

	// 2. 피타고라스의 정리로 목표까지의 '실제 직선 거리' 계산
	double distance = sqrt(dx * dx + dy * dy);

	// 3. 도착 판정 (진동 현상 방지)
	// 거리가 내 이동 속도보다 가깝다면, 덜덜 떨리는 현상을 막기 위해 목표 위치에 고정시키고 종료
	if (distance <= m_speed)
	{
		m_x = targetX;
		m_y = targetY;
		return;
	}

	// 4. 단위 벡터(정규화) 만들기
	// X, Y 거리 차이를 전체 거리로 나누면, 크기가 딱 1인 '순수한 방향'만 남게 됩니다.
	double dirX = dx / distance;
	double dirY = dy / distance;

	// 5. 순수한 방향(크기 1)에 나의 이동 속도(m_speed)를 곱해서 최종 이동
	m_x += dirX * m_speed;
	m_y += dirY * m_speed;
}