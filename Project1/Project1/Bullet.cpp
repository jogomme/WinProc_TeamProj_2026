#include <iostream>
#include "Rock.h"
#include "GameObject.h"
#include "Player.h"
#include "Bullet.h"

#define INF 999

Bullet::Bullet()
{
	m_speed = 30;

	isActive = false;

	m_x = INF;
	m_y = INF;

	m_hp = INF;
	m_attackPower = 1;

	m_size = 10;
}

Bullet::~Bullet()
{

}

// 생성 함수
void Bullet::Spawn(const Player& p, const Rock& r)
{
	int px = p.GetX();
	int py = p.GetY();

	m_attackPower = p.GetAttackPower();

	isActive = true;

	m_x = px;
	m_y = py;

	SetDir(r);
}

// 이동 함수
void Bullet::Move(double xPos, double yPos) 
{
	// 암석이 이동하는 함수. setDirection으로 설정한 방향벡터에 속도를 곱해서 이동.
	m_x += m_direction[0] * m_speed;
	m_y += m_direction[1] * m_speed;

	CheckBoundary(xPos, yPos);
}

double Bullet::GetLength(const Rock& r)
{
	int rx = r.GetX();
	int ry = r.GetY();

	double dx = rx - m_x;
	double dy = ry - m_y;
	return sqrt(dx * dx + dy * dy);
}

// 방향벡터 생성 후 정규화
void Bullet::SetDir(const Rock& r)
{
	int rx = r.GetX();
	int ry = r.GetY();

	double dx = rx - m_x;
	double dy = ry - m_y;
	double distance = sqrt(dx * dx + dy * dy);

	// (0으로 나누는 에러 방지)
	if (distance == 0) return;

	// 정규화
	m_direction[0] = dx / distance;
	m_direction[1] = dy / distance;
}


// 화면 밖으로 나갔는지 체크하는 함수
void Bullet::CheckBoundary(int width, int height) 
{
	if (isActive) {
		// 화면 밖으로 완전히 나갔다면
		if (m_x < 0 || m_x > width || m_y < 0 || m_y > height) {
			isActive = false; // 총알 사망 처리 (재장전됨)
			m_x = INF;
			m_y = INF;
		}
	}
}

// Getter
bool Bullet::GetIsActive() const 
{
	return isActive;
}

void Bullet::Crash(Rock& r)
{
	if (r.GetActive()) {
		double distance = GetLength(r);

		if (distance - r.GetSize() < m_size) {
			r.GetDemege(m_attackPower);
			isActive = false;
		}
	}
}