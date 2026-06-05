#include"GameObject.h"
#include"Rock.h"
#include"Feed.h"
#include<random>
#include<iostream>


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

	m_RockType = GetRockType();

	int randomX = (rand() % 200) - 100;
	int randomY = (rand() % 200) - 100;
	setDirection(m_x + randomX, m_y + randomY);

	// 3. 파편의 튀는 속도를 랜덤하게 지정
	m_speed = ((rand() % 5) + 3)/3;
}

void Feed::SetLength(const Player& p)
{
	double dx = m_x - p.GetX();
	double dy = m_y - p.GetY();
	double length = sqrt(dx * dx + dy * dy);
	
	if (length - p.GetSearchBox() <= m_size) {
		GameMap::m_rock[m_RockType].Num++;
		GameMap::m_rock[m_RockType].Price = m_price;
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
