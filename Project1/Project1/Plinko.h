#pragma once

#include "Rock.h"
#include <Windows.h>
#include <vector>

extern bool pCheck;
extern bool plinkoStart;

extern int plinkoRunning;	// 0 실행 멈춤, 1 실행 중
extern int plinkoView;		// 0 실행 뷰 끄기, 1 실행 뷰 보기
extern int plinkoRockSum;	// 운석 남은 갯수 확인

extern int userCash;	// 유저 보유 머니

// 플랑코 시스템 클래스
class PlinkoRock : public GameObject
{
public:
	PlinkoRock(const MinRock& rock, int type);
	~PlinkoRock();

	int p_price;	// 돈
	int p_type;		// 타입

	void cntRock();
	int sumPrice(int totalPriceint, int addPrice);
	static void spawn();

	static void plinkoNumInit();

};
void checkGoal();

void plinkoDraw(HDC hDC);
void MoneyBoxDraw(HDC hDC);	// 돈 표시

int GetMoney(); // 가진 돈의 양을 리턴 합니다
void UseMoney(int); // 해당 수치만큼 돈을 사용합니다

void plinkoInit(HWND hWnd);
void plinkoSpawn(const MinRock& rock, int type);	// 벡터에 rock 넣기

void rocksDraw(HDC hDC);
void rockUpdate();

bool pTimerCheck();

// 물리 함수
void rockCollisionCheck();
void pinCollisionCheck();

// 초기화
void PlinkoRestart();
void PlinkoReset();

bool plinkoEmptyCheck();