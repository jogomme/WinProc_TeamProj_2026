#include "Boss.h"
#include "Player.h" // 전방선언만 된 Player의 완전한 정의는 .cpp에서 include
#include <cmath>

// PI 매크로 충돌 방지를 위해 상수형 변수로 선언
const double BOSS_PI = 3.14159265358979;

Boss::Boss()
{
	m_isActive = false;
	m_hp = 30.0;
	m_size = 40.0;
	m_speed = 1.0;
	attackPattern = 0;
	patternAngle = 0.0;

	for (int i = 0; i < MAX_BOSS_BULLETS; i++) {
		bBullets[i].active = false;
	}
}

Boss::~Boss() {}

void Boss::Spawn(double x, double y)
{
	m_x = x;
	m_y = y;
	m_hp = 100.0;
	m_isActive = true;
	attackPattern = 0;
	patternAngle = 0.0;
}

bool Boss::TakeDamage(double dmg)
{
	if (!m_isActive) return false;
	m_hp -= dmg;
	if (m_hp <= 0.0) {
		m_hp = 0.0;
		m_isActive = false;
		return true; // 사망
	}
	return false;
}

void Boss::Move(double px, double py)
{
	if (!m_isActive) return;

	double dx = px - m_x;
	double dy = py - m_y;
	double dist = sqrt(dx * dx + dy * dy);

	if (dist > 0.0) {
		m_x += (dx / dist) * m_speed;
		m_y += (dy / dist) * m_speed;
	}
}

void Boss::FireDanmaku(const Player& p)
{
	if (!m_isActive) return;

	// 패턴 0: 전방위 원형 확산 (360도로 18발 발사)
	if (attackPattern == 0) {
		int bulletCount = 18;
		for (int i = 0; i < bulletCount; i++) {
			// C2065 방지: angle을 for 바깥에 선언
			double angle = (double)i * (360.0 / (double)bulletCount) * (BOSS_PI / 180.0);

			for (int j = 0; j < MAX_BOSS_BULLETS; j++) {
				if (!bBullets[j].active) {
					bBullets[j].active = true;
					bBullets[j].x = m_x;
					bBullets[j].y = m_y;
					bBullets[j].dx = cos(angle);
					bBullets[j].dy = sin(angle);
					bBullets[j].speed = 5.0;
					break;
				}
			}
		}
		attackPattern = 1;
	}
	// 패턴 1: 나선형 (회전하며 발사)
	else if (attackPattern == 1) {
		for (int i = 0; i < 4; i++) {
			double angle = patternAngle + (double)i * 90.0 * (BOSS_PI / 180.0);

			for (int j = 0; j < MAX_BOSS_BULLETS; j++) {
				if (!bBullets[j].active) {
					bBullets[j].active = true;
					bBullets[j].x = m_x;
					bBullets[j].y = m_y;
					bBullets[j].dx = cos(angle);
					bBullets[j].dy = sin(angle);
					bBullets[j].speed = 4.0;
					break;
				}
			}
		}
		patternAngle += 5.0;

		if (patternAngle > 360.0) {
			patternAngle = 0.0;
			attackPattern = 0;
		}
	}
}

void Boss::MoveBullets(int width, int height)
{
	for (int i = 0; i < MAX_BOSS_BULLETS; i++) {
		if (bBullets[i].active) {
			bBullets[i].x += bBullets[i].dx * bBullets[i].speed;
			bBullets[i].y += bBullets[i].dy * bBullets[i].speed;

			if (bBullets[i].x < -50.0 || bBullets[i].x >(double)width + 50.0 ||
				bBullets[i].y < -50.0 || bBullets[i].y >(double)height + 50.0) {
				bBullets[i].active = false;
			}
		}
	}
}

bool Boss::CheckBulletCollision(Player& p)
{
	for (int i = 0; i < MAX_BOSS_BULLETS; i++) {
		if (bBullets[i].active) {
			double dx = p.GetX() - bBullets[i].x;
			double dy = p.GetY() - bBullets[i].y;
			double dist = sqrt(dx * dx + dy * dy);

			if (dist < p.GetSize() + 5.0) {
				bBullets[i].active = false;
				p.GetDemege(1.0);
				return true;
			}
		}
	}
	return false;
}

void Boss::Draw(HDC mDC)
{
	if (!m_isActive) return;

	HBRUSH bBrush = CreateSolidBrush(RGB(150, 0, 200));
	HBRUSH oldbBrush = (HBRUSH)SelectObject(mDC, bBrush);

	Rectangle(mDC, (int)(m_x - m_size), (int)(m_y - m_size), (int)(m_x + m_size), (int)(m_y + m_size));

	SelectObject(mDC, oldbBrush);
	DeleteObject(bBrush);

	HBRUSH hpBrush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH oldHpBrush = (HBRUSH)SelectObject(mDC, hpBrush);

	double hpRate = m_hp / 100.0;
	Rectangle(mDC, (int)(m_x - m_size), (int)(m_y - m_size - 15),
		(int)(m_x - m_size + (m_size * 2.0 * hpRate)), (int)(m_y - m_size - 5));

	SelectObject(mDC, oldHpBrush);
	DeleteObject(hpBrush);
}

void Boss::DrawBullets(HDC mDC)
{
	HBRUSH bulletBrush = CreateSolidBrush(RGB(255, 100, 100));
	HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, bulletBrush);

	for (int i = 0; i < MAX_BOSS_BULLETS; i++) {
		if (bBullets[i].active) {
			int bs = 5;
			Ellipse(mDC, (int)(bBullets[i].x - bs), (int)(bBullets[i].y - bs),
				(int)(bBullets[i].x + bs), (int)(bBullets[i].y + bs));
		}
	}

	SelectObject(mDC, oldBrush);
	DeleteObject(bulletBrush);
}