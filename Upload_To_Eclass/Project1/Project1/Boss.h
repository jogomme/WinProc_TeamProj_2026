#pragma once
#include "GameObject.h"
#include <Windows.h>

// Player 전방선언 - 순환 include 방지
// Boss.h가 Player.h를 직접 include하면 헤더 체인이 꼬여서
// C2601(지역 함수 정의 오류), C1075 등이 발생합니다.
class Player;

#define MAX_BOSS_BULLETS 50000

// 보스 전용 탄막 총알 구조체
struct BossBullet {
	double x, y;
	double dx, dy; // 날아갈 방향 벡터
	double speed;
	bool active;
};

class Boss : public GameObject
{
public:
	Boss();
	~Boss();

	// 생성 및 이동
	void Spawn(double x, double y);
	void Move(double px, double py) override; // 플레이어를 향해 서서히 이동
	void Draw(HDC mDC, HINSTANCE);

	// 탄막(총알) 관련
	void FireDanmaku(const Player& p);  // 탄막 발사 패턴 로직
	void MoveBullets(int width, int height);
	void DrawBullets(HDC mDC);
	bool CheckBulletCollision(Player& p); // 플레이어 피격 판정

	// 상태 관리
	bool IsActive() const { return m_isActive; }
	void SetActive(bool b) { m_isActive = b; }
	bool TakeDamage(double dmg); // true 반환 시 보스 사망

private:
	bool m_isActive;     // 보스 활성화 여부
	int attackPattern;   // 0: 원형 확산, 1: 나선형 회전 등 패턴 구분용
	double patternAngle; // 나선형 회전 각도 기억용
	BossBullet bBullets[MAX_BOSS_BULLETS];
	double max_hp;
};