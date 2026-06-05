#pragma once

#include<iostream>
#include<Windows.h>

#include "GameObject.h"

class Enforce
{
public:
	Enforce();
	~Enforce();
	// 복사 생성자와 대입 연산자 삭제
	Enforce(const Enforce& other) = delete;
	Enforce& operator=(const Enforce& other) = delete;

	// Getter 함수
	int Get_Enforce_Point_x();
	int Get_Enforce_Point_y();
	int Get_Enforce_Type();
	int Get_Enforce_Amount();
	int Get_Enforce_Price();
	int Get_Enforce_Draw();
	int Get_Enforce_Drawing(int);
	int Get_Enforce_Open();

	void Set_Pos(double, double); // 위치 지정 함수
	void Move_Mid_Pos(Point rectViewMid, int intrv); // Set_Pos 기반 중앙 위치 이동 함수
	void Set_TAP(int, int, int); // type, amount, price 지정 함수
	void Set_Draw(int); // draw 지정
	void Set_Drawing(int, int, int); // 강화시 그릴 버튼 지정 함수 최대 3개
	void Set_Open(int); // open 지정

	void Start_Setting(Enforce&, int); // 위 함수들을 실행하는 함수

protected:
	double x, y; // 버튼의 위치 배율 (최초 노드는 0,0)
	int type; // 강화 할 타입
	int amount; // type 의 직접적인 수치
	int price; // 가격

	int draw; // 현재 이 버튼을 그려낼 것인지
	int drawing[3]; // 강화 되었을 시, 어떤 버튼을 그려낼 지.
	int open; // 현재 이 버튼이 강화가 되었는지
};