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
	void show() const;

	int GetID() const;

private :
	int id; // 암석 고유 번호
	static int gid; // 암석 고유 번호를 위한 전역 변수

	double m_price; // 가격
	int m_RockType; // 암석 종류
};