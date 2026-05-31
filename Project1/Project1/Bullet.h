#pragma once

#include<iostream>
#include "GameObject.h"
#include "Player.h"
#include "Rock.h"

// Player가 총알에게 direction을 준다 그리고 그 방향으로 총알을 발사
// 일정 거리 이상으로 가면 중지
class Bullet : public GameObject
{
public :
	// 스페셜 함수
	//생성자
	Bullet();
	// 소멸자 
	~Bullet();
	// 복사 생성자와 대입 연산자 삭제
	Bullet(const Bullet& other) = delete;
	Bullet& operator=(const Bullet& other) = delete;

	// 이동 함수
	virtual void Move(double x, double y) override;
	
	void attack();

	// 생성 함수
	void Spawn(const Player& p, const Rock& r);
	
	// 위치 재정의 밑 소멸 함수
	void SetPos();

	// 방향 벡터 초기화 함수
	void SetDir(const Rock& r);
	void CheckBoundary(int width, int height);
	
	// Getter 함수
	bool GetIsActive() const;
	
	// 운석과 총알의 거리 체크 함수
	double GetLength(const Rock& r);

	// 충돌 체크 함수
	void Crash(Rock& r);

	
private :
	// 방향 벡터
	double m_direction[2];

	double m_AttackPower;

	// 발사되고 있는 상태인지 알려주는 변수
	bool isActive;
};

