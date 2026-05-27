#pragma once
#include "GameMap.h"

class GameObject : public GameMap
{
public:

	// 생성자
	GameObject();
	
	// 소멸자 
	~GameObject();

	// 복사 생성자와 대입 연산자 삭제
	GameObject(const GameObject& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;


	// 멤버 함수
	void show() const;		

	// X , Y 좌표와 체력을 반환하는 함수
	int GetX();
	int GetY();
	double GetHP();

	void GetDemege(double damage); // 암석이 데미지를 받는 함수
	void attack(); // 공격 함수)

protected :
	int m_hp; // 체력
	double m_x;  // x 좌표
	double m_y;  // y 좌표
	double m_speed; // 이동 속도
	double m_attackPower; // 공격력
	double m_size; // 크기 - 반지름의 크기
};