#pragma once

#include<iostream>
#include<Windows.h>

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

	void Set_Pos(double, double);
	void Set_TAP(int, int, int);
	void Set_Draw(int);
	void Set_Drawing(int, int, int);

protected:
	double x, y; // 버튼의 위치 배율 (최초 노드는 0,0)
	int type; // ex) 0-미사용 노드 1-공격, 2-방어, 3-체력, 4,5,6,7,,,-특수능력 등등
	int amount; // type 의 직접적인 수치 ex) 공격력 3 / 방어력 2 / 특수능력 1 해금 2 해금 등등;
	int price; // 가격

	int draw; // 현재 이 버튼을 그려낼 것인지
	int drawing[3]; // 강화 되었을 시, 어떤 버튼을 그려낼 지.
	int open; // 현재 이 버튼이 강화가 되었는지
};