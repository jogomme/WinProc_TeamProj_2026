#pragma once

#include "GameObject.h"
#include "Rock.h"
#include<iostream>

#define MAX_ROCKS 50


class Player : public GameObject
{
public:
	// 생성자
	Player();
	// 소멸자 
	~Player();
	// 복사 생성자와 대입 연산자 삭제
	Player(const Player& other) = delete;
	Player& operator=(const Player& other) = delete;
	// 멤버 함수
	void show() const;

	void attack(Rock& r); // 공격 함수

	void SetLength(const Rock& r); // 플레이어와 암석사이의 거리 계산 함수

	// Getter
	int GetMinLengthID(); // 최소거리에 있는 암석의 ID 반환 함수

	int GetAttackSpeed();

	int GetMaxFual();
	int GetFual();

	double GetSearchBox() const;

	bool GetAttackTypeChanged() const;

	//최소 거리에 있는 암석의 위치 반환 함수
	Point GetMinLengthRock();
	
	// 강화화 관련된 함수
	int GetAttackType(); // 공격 종류 반환
	void SetAttackType(int type); // 공격 종류 설정 함수
	
	// 강화화 관련된 함수 - 아래 세 함수들은 값을 넣어주면 그만큼 값이 증가한다.
	void SetFual(double deg); // 연료 설정 함수
	void SetSpeed(double deg); // 속도 설정 함수
	void SetAttackPower(double deg); // 공격력 설정 함수
	void SetAttackSpeed(double deg);
	void SetSearchBox(double deg);

	void SetAttackTypeChanged(bool b);

	void Spawn();

	// 이동 함수
	virtual void Move(double x, double y) override;

	// 연로 감소 함수 - 스테이지에 비례해 연료 감소량이 증가함
	void ConsumeFual();

	void Draw(HDC mDC, RECT rectView, HBRUSH hBrush[], HFONT hFont, HINSTANCE g_hInst);

	void SetAngle(double mouseX, double mouseY);

	void HealPlayer(double);

private:
	// 0 - 기본 무기, 1 - 체인건
	int attackType;

	double m_fual;		// 현재 연료
	double m_MaxFual;	// 최대 연료

	int max_hp;

	// Feed의 탐색범위 강화하면 늘어난다(반지름 기준) - 기본은 Size와 동일하다
	double SearchBox;

	// 공격하는 텀 - 2026, 05, 31
	int m_AttackSpeed;			// 현재 공격 속도
	int m_AttackSpeed_type_0;	// 기본 공격 속도
	int m_AttackSpeed_type_1;	// 체인건 공격 속도

	// 공격 타입이 변했음을 알리는 변수 
	bool m_AttackTypeChanged;

	double m_length[MAX_ROCKS]; // 플레이어와 암석 사이의 거리
	
	int frameX = 0;
	double m_angle;
};
