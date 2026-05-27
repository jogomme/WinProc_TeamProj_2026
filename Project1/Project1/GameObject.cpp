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

int GameObject::GetSpeed()
{
	return m_speed;
}

int GameObject::GetAttackPower()
{
	return m_attackPower;
}

int GameObject::GetSize()
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