#include "Plinko.h"
#include "Player.h"
#include "GameObject.h"

/*
가격 합산 O
구역 계산 O
구역 그리기 O
운석 생성 및 낙하 X
핀볼 << 충돌처리 구현
*/

#define ROCK_SIZE 40	// 운석 사이즈
#define GOAL_LINE 700	// 플랑코 목표 지점 Y 좌표 (m_y가 넘으면 인식)

int plinkoView = 1, plinkoRunning = 0;

// 임시 변수 (현재 가진 총 소지금)
int userCash = 0, addPrice = 0;

int zonePrice[4] = { 1, 1, 1, 1 }; // 구역별 가격 배수

int rockNum;		// 운석 종류 구별 변수
int plinkoRockSum = 0;	// 운석 남은 갯수

struct goal {
	// y 좌표는 GOAL_LINE으로 고정
	int x1, x2;		
};

struct pin {	// 튕기는 핀 위치
	int x1, y1;
	int x2, y2;

	int getMidX() const {
		return (x1 + x2) / 2;
	}
	int getMidY() const {
		return (y1 + y2) / 2;
	}
};

std::vector<pin> pinPos;

PlinkoRock::PlinkoRock() {
	p_price = m_rock[rockNum].Price;
	m_x = rand() % 1280;
	m_y = 50;
}

PlinkoRock::~PlinkoRock() {
	
}

// Ellipse(hDC, m_x, m_y, m_x + ROCK_SIZE, m_y + ROCKSIZE);

void PlinkoRock::cntRock() {
	// 운석 전체 갯수
	plinkoRockSum = m_rock[0].Num + m_rock[1].Num + m_rock[2].Num;
	// std::cout << "sum : " << sum << std::endl;
}

// 값 더하기
int PlinkoRock::sumPrice(int totalPrice, int addPrice) {
	totalPrice = totalPrice + addPrice;
	return totalPrice;
}

RECT plinkoBox;
goal goalBox[4];
void plinkoInit(HWND hWnd) {	// 초기화
	pinPos.clear();
	GetClientRect(hWnd, &plinkoBox);
	// 플랑코 도착 지점 4구간 x1, x2 좌표
	goalBox[0] = { (plinkoBox.right / 4) * 0, (plinkoBox.right / 4) * 1 };
	goalBox[1] = { (plinkoBox.right / 4) * 1, (plinkoBox.right / 4) * 2 };
	goalBox[2] = { (plinkoBox.right / 4) * 2, (plinkoBox.right / 4) * 3 };
	goalBox[3] = { (plinkoBox.right / 4) * 3, plinkoBox.right };

	// 핀 좌표 설정
	// **어긋나는 부분 수정할 예정**
	int width = plinkoBox.right;
	int height = GOAL_LINE - 100;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; j++) {
			pin p;
			int xGap = width / 6;
			int x = xGap * (j+1);
			if (i % 2 == 0) {
				x += xGap / 2;
			}
			
			int y = height / 10;

			p.x1 = x - 5;
			p.x2 = x + 5;
			p.y1 = y * (i + 1) - 5;
			p.y2 = y * (i + 1) + 5;
			pinPos.push_back(p);
		}
	}
}

int PlinkoRock::checkGoal() {
	if (GOAL_LINE <= m_y && goalBox[0].x1 <= m_x && m_x <= goalBox[0].x2) {
		addPrice = m_rock[rockNum].Price;
		addPrice = addPrice* zonePrice[0];

		userCash = sumPrice(userCash, addPrice);
		return 1;
	}
	else if (GOAL_LINE <= m_y && goalBox[1].x1 <= m_x && m_x <= goalBox[1].x2) {
		addPrice = m_rock[rockNum].Price;
		addPrice = addPrice * zonePrice[1];

		userCash = sumPrice(userCash, addPrice);
		return 2;
	}
	else if (GOAL_LINE <= m_y && goalBox[2].x1 <= m_x && m_x <= goalBox[2].x2) {
		addPrice = m_rock[rockNum].Price;
		addPrice = addPrice * zonePrice[2];

		userCash = sumPrice(userCash, addPrice);
		return 3;
	}
	else if (GOAL_LINE <= m_y && goalBox[3].x1 <= m_x && m_x <= goalBox[3].x2) {
		addPrice = m_rock[rockNum].Price;
		addPrice = addPrice * zonePrice[3];

		userCash = sumPrice(userCash, addPrice);
		return 4;
	}
	return 0;
}

bool plinkoCollisionCheck() {
	
	// 핀과 운석, 운석과 운석 충돌 처리
	for (auto& p : pinPos) {
		/*if (plinkoRock.m_x) {

		}*/
		return TRUE;
	}
	return FALSE;

}

void plinkoDraw(HDC hDC) {
	// 구역 그리기
	Rectangle(hDC, plinkoBox.left, plinkoBox.top, plinkoBox.right, plinkoBox.bottom);	
	MoveToEx(hDC, 0, GOAL_LINE, NULL);
	LineTo(hDC, plinkoBox.right, GOAL_LINE);
	for (int i = 0; i < 3; i++) {
		MoveToEx(hDC, goalBox[i].x2, GOAL_LINE, NULL);
		LineTo(hDC, goalBox[i].x2, plinkoBox.bottom);
	}
	
	// 튕기는 핀 그리기
	for (auto& p : pinPos)
	{
		Ellipse(hDC, p.x1, p.y1, p.x2, p.y2);
	}

}

