#pragma once

#include "Rock.h"
#include <Windows.h>
#include <vector>

extern int plinkoRunning;	// 0 실행 멈춤, 1 실행 중
extern int plinkoView;		// 0 실행 뷰 끄기, 1 실행 뷰 보기
extern int plinkoRockSum;	// 운석 남은 갯수 확인
// 플랑코 시스템 클래스
class PlinkoRock : public GameObject
{
private:
	std::vector<MinRock> P_rock;
public:
	PlinkoRock();
	~PlinkoRock();


	int p_price;

	void cntRock();
	int sumPrice(int totalPriceint, int addPrice);
	int checkGoal();
};

void plinkoDraw(HDC hDC);
void plinkoInit(HWND hWnd);