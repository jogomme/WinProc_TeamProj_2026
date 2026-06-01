#pragma once

#include "GameObject.h"
#include "Rock.h"
#include<iostream>

#define MAX_ROCKS 50


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

	void attack(Rock& r); // 공격 함수

	void SetLength(const Rock& r); // 플레이어와 암석사이의 거리 계산 함수


	// Getter
	int GetMinLengthID(); // 최소거리에 있는 암석의 ID 반환 함수

	int GetAttackSpeed();

	//최소 거리에 있는 암석의 위치 반환 함수
	Point GetMinLengthRock();
	
	// 강화화 관련된 함수
	void SetAttackType(int type);// 공격 종류 설정 함수
	
	// 강화화 관련된 함수 - 아래 세 함수들은 값을 넣어주면 그만큼 값이 증가한다.
	void SetFual(double deg); // 연료 설정 함수
	void SetSpeed(double deg); // 속도 설정 함수
	void SetAttackPower(double deg); // 공격력 설정 함수
	void SetAttackSpeed(double deg);

	void Spawn();

	// 이동 함수
	virtual void Move(double x, double y) override;

	// 연로 감소 함수 - 스테이지에 비례해 연료 감소량이 증가함
	void ConsumeFual(int stage);


private:
	int attackType; // 공격 종류
	double m_fual; // 연료

	int max_hp;

	// 공격하는 텀 - 2026, 05, 31
	int m_AttackSpeed;

	double m_length[MAX_ROCKS]; // 플레이어와 암석 사이의 거리
};
