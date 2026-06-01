#include "GameObject.h"
#include "Player.h"
#include "Rock.h"
#include <iostream>
#include <math.h>

#define INF 999

Player::Player()
{
	m_attackPower = 1;
	max_hp = 1;
	m_speed = 10;
	m_x = 690;
	m_y = 400;
	attackType = 0;
	m_size = 20;
	m_MaxFual = 100;
	for (int i = 0; i < MAX_ROCKS; i++)
	{
		m_length[i] = INF;
	}

	//2026,05,31
	// 1초에 1발은 쏜다는 것을 목표로 설정
	m_AttackSpeed = 1000;
}	

Player::~Player()
{

}

// 플레이어와 암석 사이의 거리 계산 함수
void Player::SetLength(const Rock& r)
{
	if (r.GetActive()) {
		double dx = m_x - r.GetX();
		double dy = m_y - r.GetY();
		m_length[r.GetID()] = sqrt(dx * dx + dy * dy);

		// 충돌 판정 - 암석의 크기를 고려해서, 
		// 플레이어와 암석 사이의 거리가 플레이어의 크기보다 작아지는 경우 데미지를 받는다.
		if (m_length[r.GetID()] - r.GetSize() < m_size)
		{
			GetDemege(r.GetAttackPower());
		}

	}
	else {
		m_length[r.GetID()] = INF;
	}
}

// Getter
int Player::GetAttackSpeed()
{
	return m_AttackSpeed;
}

// 최소거리에 있는 암석의 ID 반환 함수
int Player::GetMinLengthID()
{
	double best = INF;
	int bestId = -1;

	for (int i = 0; i < MAX_ROCKS; i++)
	{
		if (m_length[i] < best && m_length[i] != INF) // INF와 같지 않을 때만 비교
		{
			best = m_length[i];
			bestId = i;
		}
	}

	return bestId;
}

int Player::GetFual()
{
	return m_fual;
}

int Player::GetMaxFual()
{
	return m_MaxFual;
}

void Player::Spawn()
{
	m_hp = max_hp;
	m_x = 690;
	m_y = 400;
	attackType = 0;
	m_size = 20;
	m_fual = m_MaxFual;
	for (int i = 0; i < MAX_ROCKS; i++)
	{
		m_length[i] = INF;
	}

	m_AttackSpeed = 1000;
}

void Player::attack(Rock& r)
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
		if (rID == rMinID && m_length[rID] != -1) { 
			r.GetDemege(m_attackPower); 
		}
	}
	else if (attackType == 1) {
		NULL;
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

void Player::ConsumeFual()
{
	int stage = GameMap::GetStage();
	m_fual -= (1 + stage * 1.2 );
	if (m_fual < 0) {
		m_fual = 0;
	}
}

// 공격 종류 설정 함수
void Player::SetAttackType(int type) 
{
	attackType = type;
}

// 연료 증가 함수
void Player::SetFual(double deg)
{
	m_fual += deg;
}

// 속도 증가 함수
void Player::SetSpeed(double deg)
{
	m_speed += deg;
}

// 공격력 증가 함수
void Player::SetAttackPower(double deg)
{
	m_attackPower += deg;
}

void Player::SetAttackSpeed(double deg)
{
	m_AttackSpeed -= deg;
}