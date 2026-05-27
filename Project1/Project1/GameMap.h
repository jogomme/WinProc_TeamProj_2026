#pragma once

#include<iostream>

#define MAX_ROCKS 50

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

protected :

	// 부숴진 암석의 종류와 갯수를 저장하는 배열
	// 0번 index 가 가장 낮은 가치를 지님
	MinRock m_rock[3];
	int m_stage;

	// 게임의 진행 속도를 조절하는 변수
	int TickRate; 
};