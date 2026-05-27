#include<iostream>

#include  "GameObject.h"

// 무한
#define INF 9999

GameObject::GameObject()
	: m_hp(3), m_x(INF), m_y(INF), m_speed(0), m_attackPower(1)
{
	
}

GameObject::~GameObject()
{

}

void GameObject::show() const
{
	std::cout << "위치 : (" << m_x << ", " << m_y << "), 체력: " << m_hp << std::endl;
}


// X , Y 좌표와 체력을 반환하는 함수
int GameObject::GetX()
{
	return m_x;
}

int GameObject::GetY()
{
	return m_y;
}

double GameObject::GetHP()
{
	return m_hp;
}

void GameObject::GetDemege(double damage)
{
	m_hp -= damage;
	if (m_hp < 0) {
		m_hp = 0;
	}
}