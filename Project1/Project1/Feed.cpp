#include"GameObject.h"
#include"Rock.h"
#include"Feed.h"
#include<random>
#include<iostream>
#include"Player.h"

extern Player player;

std::default_random_engine generate2(time(NULL)); // 난수 생성기 초기화
std::uniform_int_distribution<int> AttackTypeDist(1, 1); // 0, 1, 2 중에서 랜덤한 정수를 생성하는 분포

Feed::Feed()
{
	isActive = false;
	m_x = 9999;
	m_y = 9999;
	m_size = 5;
	m_hp = INFINITY;
}

Feed::~Feed()
{

}

void Feed::Drop(const Rock& r)
{
	m_x = r.GetX();
	m_y = r.GetY();
	m_price = r.GetPrice();

	isActive = true;

	m_RockType = r.GetRockType();

	if (m_RockType != 3) {
		int randomX = (rand() % 200) - 100;
		int randomY = (rand() % 200) - 100;
		setDirection(m_x + randomX, m_y + randomY);
	}
	else {

	}
	// 3. 파편의 튀는 속도를 랜덤하게 지정
	m_speed = ((rand() % 5) + 3) / 3;
}

void Feed::SetLength(Player& p)
{
	double dx = m_x - p.GetX();
	double dy = m_y - p.GetY();
	double length = sqrt(dx * dx + dy * dy);

	if (length - p.GetSearchBox() <= m_size) {
		if (m_RockType != 3) {
			GameMap::m_rock[m_RockType].Num++;

			// 레어도에 따른 운석 가치 배율 적용
			// 0-Normal: x1, 1-Rare: x2, 2-Unique: x3, 3-Epic: x5, 4-Legendary: x10
			int rare = GameMap::GetRare();
			int rareMult = 1;
			if (rare == 1) rareMult = 2;
			else if (rare == 2) rareMult = 3;
			else if (rare == 3) rareMult = 5;
			else if (rare == 4) rareMult = 10;

			GameMap::m_rock[m_RockType].Price = m_price * rareMult;
		}
		else {
			player.SetAttackType(AttackTypeDist(generate2));
		}
		isActive = false;
	}
}


void Feed::Move(double x, double y)
{
	if (!isActive) return;

	m_x += direction[0] * m_speed;
	m_y += direction[1] * m_speed;

	if (m_x < -50 || m_x > x + 50 || m_y < -50 || m_y > y + 50) {
		isActive = false;
	}
}


void Feed::SetActive(bool b)
{
	isActive = b;
}