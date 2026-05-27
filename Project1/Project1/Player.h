#pragma once

#include "GameObject.h"
#include "Rock.h"
#include<iostream>

#define MAX_ROCKS 50
#define INF 9999

class Player : public GameObject
{
public:
	// 생성자
	Player();
	// 소멸자 
	~Player();
	// 복사 생성자와 대입 연산자 삭제
	Player(const Player& other) = delete;
	Player& operator=(const Player& other) = delete;
	// 멤버 함수
	void show() const;

	void attack(Rock r); // 공격 함수

	void SetLength(Rock r); // 플레이어와 암석사이의 거리 계산 함수

	int GetMinLengthID(); // 최소거리에 있는 암석의 ID 반환 함수

	// 이동 함수
	virtual void Move(double x, double y) override;

private:
	int attackType; // 공격 종류

	double m_length[MAX_ROCKS]; // 플레이어와 암석 사이의 거리
};
