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


// Getter 함수
double GameObject::GetX() const
{
	return m_x;
}

double GameObject::GetY() const
{
	return m_y;
}

double GameObject::GetHP() const
{
	return m_hp;
}

double GameObject::GetSpeed() const
{
	return m_speed;
}

double GameObject::GetAttackPower() const
{
	return m_attackPower;
}

double GameObject::GetSize() const
{
	return m_size;
}

// 데미지 받는 함수
void GameObject::GetDemege(double damage)
{
	m_hp -= damage;
	if (m_hp < 0) {
		m_hp = 0;
	}
}

// 위치 설정 함수
void GameObject::SetPosition(double x, double y)
{
	m_x = x;
	m_y = y;
}

// 이동 함수
void GameObject::Move(double x, double y)
{
	m_x += x * m_speed;
	m_y += y * m_speed;
}