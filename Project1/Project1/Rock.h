#pragma once

#include<iostream>
#include "GameObject.h"
#include "GameMap.h"
#include <time.h>


class Rock : public GameObject
{
public:
	// 생성자
	Rock();
	// 소멸자 
	~Rock();
	// 복사 생성자와 대입 연산자 삭제
	Rock(const Rock& other) = delete;
	Rock& operator=(const Rock& other) = delete;

	// 멤버 함수

	// Getter 함수
	int GetID() const;// 완
	bool GetActive() const;

	// 암석이 이동하는 방향 설정 함수
	void setDirection(int X, int Y); // 완

	// 이동 함수
	void Move(double x, double y) override; // 완

	// 암석 해금 함수
	void UnlockRockType(int rockType); // 완

	// 운석 생성 함수
	void Spawn();

	// 운석 위치 세팅 함수
	void SetRockPos();

	void CheckBoundary(int widths, int heights);

private:
	int id; // 암석 고유 번호
	static int gid; // 암석 고유 번호를 위한 전역 변수
	double direction[2]; // 암석이 이동하는 방향, x, y 좌표이다.

	double m_price; // 가격

	// 현재 그려지고 있는지 알려주는 변수
	bool isActive;

	const int m_MaxRockType = 4; // 암석의 최대 종류
	int m_RockType; // 현재 내가 어떤 암석인지
	int m_unlockType[4] = { 1,0,0,0 }; // 지금 암석이 나오는 종류

};