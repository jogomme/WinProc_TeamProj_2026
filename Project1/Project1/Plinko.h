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
	PlinkoRock(const MinRock& rock);
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

void plinkoInit(HWND hWnd);
void plinkoSpawn(const MinRock& rock);	// 벡터에 rock 넣기

void rocksDraw(HDC hDC);
void rockUpdate();

void pTimerCheck(HWND hWnd);



// 초기화
void PlinkoRestart();
void PlinkoReset();

bool plinkoEmptyCheck();