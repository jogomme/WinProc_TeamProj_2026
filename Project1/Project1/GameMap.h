#pragma once

#include<iostream>
#include<Windows.h>

#define MAX_ROCKS 50

// 이 헤더는 게임 맵( 이 게임의 본게임을 하는 곳 )을 다루는 헤더입니다.

struct MinRock
{
	int	Price; // 가격
	int Num; // 암석 갯수
};

class GameMap
{
public:
	GameMap();
	~GameMap();
	// 복사 생성자와 대입 연산자 삭제
	GameMap(const GameMap& other) = delete;
	GameMap& operator=(const GameMap& other) = delete;
	
	// 게임 맵을 출력하는 함수
	void show() const; 

	// Getter 함수
	int GetStage() const;
	int GetTickRate() const;

	// Set 함수
	void SetStage(int stage);
	void SetRockNum();

	// 다음 스테이지로 넘어가는 함수
	void NextStage(); 

	void Draw(HDC mDC, RECT rectView, HBRUSH hBrush[], HFONT hFont);

protected :

	// 부숴진 암석의 종류와 갯수를 저장하는 배열
	// 0번 index 가 가장 낮은 가치를 지님
	MinRock m_rock[3];

	static int m_stage;

	// 스테이지 올라가기까지 필요한 암석의 갯수
	int m_goal;

	// 이번 스테이지에 파괴된 암석의 갯수
	static int m_rockNum;

	// 게임의 진행 속도를 조절하는 변수
	int TickRate; 
};