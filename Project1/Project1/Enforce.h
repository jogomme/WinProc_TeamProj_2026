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
	int 

	int Enforce_Point_Calc();

protected:
	int asd123;
};

// 강화 버튼 관련
{
	enforce_cnt = 20;
	enforce_size = 20;
	enforce_intrv = 80;
	// type 0-미사용 노드 1-공격, 2-방어, 3-체력, 4,5,6,7,,,-특수능력 등등
	// 사용법 // (화면 중앙 포인트, xy 좌표중 택1, xy 좌표의 배율[상대적 좌표], 버튼 사이 거리)
	// 예시작으로 5개 구현
	enforce[0].x = Enforce_Point_Calc(rectViewMid, 'x', 0, enforce_intrv);
	enforce[0].y = Enforce_Point_Calc(rectViewMid, 'y', 0, enforce_intrv);
	enforce[0].type = 1; enforce[0].amount = 1; enforce[0].draw = 1; enforce[0].open = 0;
}

struct Enforce {
	float x, y; // 버튼의 위치 배율 (최초 노드는 0,0)
	int type; // ex) 0-미사용 노드 1-공격, 2-방어, 3-체력, 4,5,6,7,,,-특수능력 등등
	int amount; // type 의 직접적인 수치 ex) 공격력 3 / 방어력 2 / 특수능력 1 해금 2 해금 등등;
	int draw; // 현재 이 버튼을 그려낼 것인지 ex) 근처의 노드를 열면 1을 주어 그리도록 // 3진 트리
	int open; // 현재 이 버튼이 강화가 되었는지
	int price;
};
static Enforce enforce[20000];
static int enforce_size; // 버튼의 반지름 크기
static int enforce_intrv; // 버튼 사이 간격
static int enforce_cnt; // 강화 버튼 갯수

int Enforce_Point_Calc(Point rectViewMid, char c, int xy, int intrv)
{
	if (c == 'x')
		return rectViewMid.x + xy * intrv;
	else
		return rectViewMid.y + xy * intrv;
}