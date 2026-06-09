#include "Plinko.h"
#include "Player.h"
#include "GameObject.h"
#include "GameMap.h"

#define ROCK_SIZE 40	// 운석 사이즈
#define GOAL_LINE 700	// 플랑코 목표 지점 Y 좌표 (m_y가 넘으면 인식)

#define PIN_ROWS 7		// 핀 가로 갯수
#define PIN_COLS 7		// 핀 세로 갯수

wchar_t plinkoStr[100];

bool pCheck = FALSE;
bool plinkoStart = FALSE;

RECT plinkoBox;
RECT MoneyBox;

// 작동 / 시점 설정 변수
int plinkoView = 1, plinkoRunning = 0; // 0 OFF, 1 ON

// 임시 변수 (현재 가진 총 소지금)
int userCash = 0, addPrice = 0;

int zonePrice[4] = { 1, 1, 1, 1 }; // 구역별 가격 배수

// 구역 가격 배수 초기화 (매 플랑코 진입 시 초기화, PlinkoRestart() 안에 포함)
void zoneReset() {
	zonePrice[0] = rand() % 4 + 1;
	zonePrice[1] = rand() % 4 + 1;
	zonePrice[2] = rand() % 4 + 1;
	zonePrice[3] = rand() % 4 + 1;
}


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

std::vector<PlinkoRock*> rocks;	// 운석 저장
std::vector<pin> pinPos;	// 핀 위치 저장

PlinkoRock::PlinkoRock(const MinRock& rock, int type) {
	p_price = rock.Price;
	// p_type = rock.Num;
	p_type = type;
	m_x = rand() % (plinkoBox.right - 300) + 150;
	m_y = 50;

	m_speed = (rand() % 5 + 5) * 0.1;
}

PlinkoRock::~PlinkoRock() {
	
}
void PlinkoRock::plinkoNumInit() {	// 돌 갯수 초기화 > 플랑코 재진입 시 시행
	m_rock[0].Num = 0;
	m_rock[1].Num = 0;
	m_rock[2].Num = 0;
}

// 플랑코 임시 스폰

int mr1 = 0, mr2 = 0, mr3 = 0;
void PlinkoRock::spawn() {
	
	for (int i = 0; i < GameMap::m_rock[0].Num; i++) {
		plinkoSpawn(GameMap::m_rock[0], 0);
	}
	for (int i = 0; i < GameMap::m_rock[1].Num; i++) {
		plinkoSpawn(GameMap::m_rock[1], 1);
	}
	for (int i = 0; i < GameMap::m_rock[2].Num; i++) {
		plinkoSpawn(GameMap::m_rock[2], 2);
	}
	
}

// 벡터에 rock 넣기
void plinkoSpawn(const MinRock& rock, int type) {
	
	rocks.push_back(new PlinkoRock(rock, type));
	std::cout << "price : " << rock.Price << ", Num : " << rock.Num << std::endl;
}

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

goal goalBox[4];
void plinkoInit(HWND hWnd) {	// 초기화
	
	pinPos.clear();
	GetClientRect(hWnd, &plinkoBox);
	// 유저 금액 표시 구역
	MoneyBox = { plinkoBox.right - 200, plinkoBox.top + 10, plinkoBox.right - 20, plinkoBox.top + 200 };
	
	// 플랑코 도착 지점 4구간 x1, x2 좌표
	goalBox[0] = { (plinkoBox.right / 4) * 0, (plinkoBox.right / 4) * 1 };
	goalBox[1] = { (plinkoBox.right / 4) * 1, (plinkoBox.right / 4) * 2 };
	goalBox[2] = { (plinkoBox.right / 4) * 2, (plinkoBox.right / 4) * 3 };
	goalBox[3] = { (plinkoBox.right / 4) * 3, plinkoBox.right };

	// 핀 좌표 설정
	int width = plinkoBox.right;
	int height = GOAL_LINE - 100;

	// 핀 설치 영역
	int left = plinkoBox.left + 50;
	int right = plinkoBox.right - 100;
	int top = plinkoBox.top + 200;
	int bottom = GOAL_LINE - 100;

	// 핀들 간의 간격
	int xGap = (right - left) / PIN_ROWS;
	int yGap = (bottom - top) / PIN_COLS;

	int pinCount;
	int centerX = plinkoBox.right / 2;
	for (int row = 0; row < 7; row++)
	{
		if (row % 2 == 0) {
			pinCount = 7;
		}
		else {
			pinCount = 6;
		}
		int startX = centerX - ((pinCount - 1) * xGap) / 2;

		int y = top + row * yGap;

		for (int col = 0; col < pinCount; col++)
		{
			pin p;

			int x = startX + col * xGap;

			p.x1 = x - 5;
			p.x2 = x + 5;
			p.y1 = y - 5;
			p.y2 = y + 5;

			pinPos.push_back(p);
		}
	}
}

bool plinkoEmptyCheck() {	// rocks 비어있는지 확인
	return rocks.empty();
}

/***************************************************************************/
// 초기화
/***************************************************************************/
void PlinkoRestart() {
	zoneReset();	// 구역 배수 랜덤 초기화
	std::cout << "zonePrice* : " << zonePrice[0] << ", " << zonePrice[1] << ", " << zonePrice[2] << ", " << zonePrice[3] << std::endl;
	PlinkoReset();   // 기존 제거
	PlinkoRock::spawn(); // 다시 생성
}

void PlinkoReset() {
	for (auto rock : rocks) {
		delete rock;
	}
	rocks.clear();	
}
/***************************************************************************/


// 플랑코 돈 적립 구간 함수
void checkGoal() {
	for (auto it = rocks.begin(); it != rocks.end(); ) {

		PlinkoRock* rock = *it;

		if (rock->GetY() >= GOAL_LINE) {

			if (goalBox[0].x1 <= rock->GetX() && rock->GetX() <= goalBox[0].x2) {
				std::cout << "BOX 1 : " << zonePrice[0] << std::endl;
				userCash += rock->p_price * zonePrice[0];
			}
			else if (goalBox[1].x1 <= rock->GetX() && rock->GetX() <= goalBox[1].x2) {
				std::cout << "BOX 2 : " << zonePrice[1] << std::endl;
				userCash += rock->p_price * zonePrice[1];
			}
			else if (goalBox[2].x1 <= rock->GetX() && rock->GetX() <= goalBox[2].x2) {
				std::cout << "BOX 3 : " << zonePrice[2] << std::endl;
				userCash += rock->p_price * zonePrice[2];
			}
			else if (goalBox[3].x1 <= rock->GetX() && rock->GetX() <= goalBox[3].x2) {
				std::cout << "BOX 4 : " << zonePrice[3] << std::endl;
				userCash += rock->p_price * zonePrice[3];
			}
			
			std::cout << "Goal Check, userCash : " << userCash << std::endl;		// 확인용
			delete rock;
			it = rocks.erase(it);
		}
		else {
			++it;
		}
	}
}

// rock - rock 충돌 처리
void rockCollisionCheck() {
	// 핀과 운석, 운석과 운석 충돌 처리
	for (int i = 0; i < rocks.size(); i++)
	{
		for (int j = i + 1; j < rocks.size(); j++)
		{
			PlinkoRock* rock1 = rocks[i];
			PlinkoRock* rock2 = rocks[j];

			double x1 = rock1->GetX() + ROCK_SIZE / 2;
			double y1 = rock1->GetY() + ROCK_SIZE / 2;

			double x2 = rock2->GetX() + ROCK_SIZE / 2;
			double y2 = rock2->GetY() + ROCK_SIZE / 2;

			double dx = x1 - x2;
			double dy = y1 - y2;

			double dist = sqrt(dx * dx + dy * dy);

			if (dist <= ROCK_SIZE)
			{
				rock1->Move(dx * 0.1, dy * 0.1);
				rock2->Move(-dx * 0.1, -dy * 0.1);
			}
		}
	}
}

// pin - rock 충돌 처리
void pinCollisionCheck() {
	// 핀과 운석, 운석과 운석 충돌 처리
	for (auto rock : rocks)
	{
		for (auto& p : pinPos)
		{
			double rockCenterX = rock->GetX() + ROCK_SIZE / 2;
			double rockCenterY = rock->GetY() + ROCK_SIZE / 2;


			double dx = rockCenterX - p.getMidX();
			double dy = rockCenterY - p.getMidY();

			double dist = sqrt(dx * dx + dy * dy);

/*			if (dist <= ROCK_SIZE / 2 + 6)
			{
				//-(rand() % 5 + 6)
				if (rock->GetX() < p.getMidX()) {
					rock->Move(-(rand() % 10), -20);	// -3 ~ -5
					break;
				}
				else if (rock->GetX() > p.getMidX()) {
					rock->Move(rand() % 10, -20);	//  3 ~ 5
					break;
				}
				else if (rock->GetX() == p.getMidX()) {
					rock->Move(rand() % 5 - 2, -20);	// -2 ~ 2
					break;
				}
					
			}*/
			if (dist <= ROCK_SIZE / 2 + 6)
			{
				double nx = dx / dist;
				double ny = dy / dist;

				rock->Move(nx * 5, ny * 5);
			}
		}
	}
}



/*wsprintf(str, L"Setting"); // 추후 이미지 버튼 등으로 변경 예정
	TextOut(mDC, rectView.left + 245, rectView.top + 15, str, lstrlen(str));*/
	// wchar_t str[64];

void MoneyBoxDraw(HDC hDC) {
	HFONT hFont = CreateFont(
		20,     // 높이(글자 크기)
		0,      // 너비
		0, 0,
		FW_BOLD,    // 굵기
		FALSE, FALSE, FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"맑은 고딕"
	);

	HFONT oldFont = (HFONT)SelectObject(hDC, hFont);
	// 구역 그리기	(구역 배수 표시 예정)


	wsprintf(plinkoStr, L"Money : %d", userCash);
	int num = wcslen(plinkoStr);
	DrawText(hDC, plinkoStr, num, &MoneyBox, DT_RIGHT | DT_TOP);

	SelectObject(hDC, oldFont);
	DeleteObject(hFont);
}

void plinkoDraw(HDC hDC) {

	MoveToEx(hDC, 0, GOAL_LINE, NULL);
	LineTo(hDC, plinkoBox.right, GOAL_LINE);
	for (int i = 0; i < 3; i++) {
		MoveToEx(hDC, goalBox[i].x2, GOAL_LINE, NULL);
		LineTo(hDC, goalBox[i].x2, plinkoBox.bottom);
	}
	
	// 튕기는 핀 그리기
	for (auto& p : pinPos) {
		Ellipse(hDC, p.x1, p.y1, p.x2, p.y2);
	}
}

void rocksDraw(HDC hDC) {
	for (auto rock : rocks) {
		HBRUSH brush;

		if (rock->p_type == 0) {
			brush = CreateSolidBrush(RGB(0, 0, 0));
		}
		else if (rock->p_type == 1) {
			brush = CreateSolidBrush(RGB(255, 0, 0));
		}
		else {
			brush = CreateSolidBrush(RGB(255, 255, 0));
		}
		HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, brush);

		Ellipse(hDC, (int)rock->GetX(), (int)rock->GetY(), (int)rock->GetX() + ROCK_SIZE, (int)rock->GetY() + ROCK_SIZE);
		
		SelectObject(hDC, oldBrush);
		DeleteObject(brush);
	}
}

void rockUpdate() {		// 운석 내려감
	for (auto rock : rocks)
	{
		rock->Move(0, 5);
	}
}

void pTimerCheck(HWND hWnd) {	// 운석 위치 업데이트 타이머 삭제
	if (rocks.empty()) {
		KillTimer(hWnd, 4);
	}
}
