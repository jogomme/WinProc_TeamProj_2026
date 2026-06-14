#include "GameObject.h"
#include "Player.h"
#include "Rock.h"
#include <iostream>
#include <math.h>

#define INF 999

extern HBITMAP imgBitmap[50];

Player::Player()
{
	m_attackPower = 1;
	max_hp = 1;
	m_speed = 4;
	m_x = 690;
	m_y = 400;
	attackType = 0;
	m_size = 20;
	m_MaxFual = 15;
	SearchBox = 20;
	for (int i = 0; i < MAX_ROCKS; i++)
	{
		m_length[i] = INF;
	}

	//2026,05,31
	// 1초에 1발은 쏜다는 것을 목표로 설정
	m_AttackSpeed_type_0 = 1000;
	m_AttackSpeed_type_1 = 500;

	m_AttackSpeed = m_AttackSpeed_type_0;

	m_AttackTypeChanged = false;

	frameX = 0;
}	

Player::~Player()
{

}

// 플레이어와 암석 사이의 거리 계산 함수
void Player::SetLength(const Rock& r)
{
	if (r.GetActive()) {
		double dx = m_x - r.GetX();
		double dy = m_y - r.GetY();
		m_length[r.GetID()] = sqrt(dx * dx + dy * dy);

		// 충돌 판정 - 암석의 크기를 고려해서, 
		// 플레이어와 암석 사이의 거리가 플레이어의 크기보다 작아지는 경우 데미지를 받는다.
		if (m_length[r.GetID()] - r.GetSize() < m_size)
		{
			GetDemege(r.GetAttackPower());
		}

	}
	else {
		m_length[r.GetID()] = INF;
	}
}

// Getter
int Player::GetAttackSpeed()
{
	return m_AttackSpeed;
}

double Player::GetSearchBox() const
{
	return SearchBox;
}

// 최소거리에 있는 암석의 ID 반환 함수
int Player::GetMinLengthID()
{
	double best = INF;
	int bestId = -1;

	for (int i = 0; i < MAX_ROCKS; i++)
	{
		if (m_length[i] < best && m_length[i] != INF) // INF와 같지 않을 때만 비교
		{
			best = m_length[i];
			bestId = i;
		}
	}

	return bestId;
}

int Player::GetFual()
{
	return m_fual;
}

int Player::GetMaxFual()
{
	return m_MaxFual;
}

bool Player::GetAttackTypeChanged() const
{
	return m_AttackTypeChanged;
}

int Player::GetAttackType()
{
	return attackType;
}

void Player::Spawn()
{
	m_hp = max_hp;
	m_x = 690;
	m_y = 400;
	attackType = 0;
	m_size = 20;
	m_fual = m_MaxFual;
	for (int i = 0; i < MAX_ROCKS; i++)
	{
		m_length[i] = INF;
	}
}

void Player::attack(Rock& r)
{
	// 암석의 ID
	int rID = r.GetID();
	
	// 암석의 최소거리 ID
	int rMinID = GetMinLengthID();

	if (m_length[rID] <= m_size )
	{
		GetDemege(1);
		return;
	}

	// attckType이 0이면 기본 공격
	
}

void Player::Move(double targetX, double targetY)
{
	// 1. 현재 위치에서 목표 위치를 바라보는 '방향 벡터(거리 차이)' 계산
	double dx = targetX - m_x;
	double dy = targetY - m_y;

	// 2. 피타고라스의 정리로 목표까지의 '실제 직선 거리' 계산
	double distance = sqrt(dx * dx + dy * dy);

	// 3. 도착 판정 (진동 현상 방지)
	// 거리가 내 이동 속도보다 가깝다면, 덜덜 떨리는 현상을 막기 위해 목표 위치에 고정시키고 종료
	if (distance <= m_speed)
	{
		frameX = 200;
		m_x = targetX;
		m_y = targetY;
		return;
	}

	// 4. 단위 벡터(정규화) 만들기
	// X, Y 거리 차이를 전체 거리로 나누면, 크기가 딱 1인 '순수한 방향'만 남게 됩니다.
	double dirX = dx / distance;
	double dirY = dy / distance;

	// 5. 순수한 방향(크기 1)에 나의 이동 속도(m_speed)를 곱해서 최종 이동
	m_x += dirX * m_speed;
	m_y += dirY * m_speed;
}

void Player::Draw(HDC mDC, RECT rectView, HBRUSH hBrush[], HFONT hFont, HINSTANCE g_hInst)
{
	// 1. 프레임 선택
	if (m_speed >= 8) frameX = 200;
	else if (m_speed >= 5) frameX = 100;

	HDC imgDC = CreateCompatibleDC(mDC);
	// 💡 플레이어 로켓은 21번 인덱스입니다.
	HBITMAP oldBmp = (HBITMAP)SelectObject(imgDC, imgBitmap[21]);

	// 회전 행렬 적용
	SetGraphicsMode(mDC, GM_ADVANCED);
	XFORM xForm;
	double angle = m_angle + 3.14159265 / 2.0;
	// ... (행렬 계산 코드는 기존과 동일하게 유지) ...
	xForm.eM11 = (FLOAT)cos(angle);
	xForm.eM12 = (FLOAT)sin(angle);
	xForm.eM21 = (FLOAT)-sin(angle);
	xForm.eM22 = (FLOAT)cos(angle);
	xForm.eDx = (FLOAT)m_x;
	xForm.eDy = (FLOAT)m_y;

	XFORM oldForm;
	GetWorldTransform(mDC, &oldForm);
	SetWorldTransform(mDC, &xForm);

	int half = (int)m_size;
	TransparentBlt(mDC, -half, -half, half * 2, half * 2, imgDC, frameX, 0, 100, 100, RGB(255, 255, 255));

	SetWorldTransform(mDC, &oldForm); // 복구

	SelectObject(imgDC, oldBmp); // 💡 비트맵 원상복구
	DeleteDC(imgDC);

	// 6. UI (연료바) 그리기 - 메모리 누수 완벽 차단
	double FualRate = m_fual / m_MaxFual;

	// 뒷배경 바
	HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush[7]);
	Rectangle(mDC, rectView.left + 5, rectView.top + 5, rectView.left + 405, rectView.top + 45);

	// 남은 연료 바
	SelectObject(mDC, hBrush[1]); // 어차피 위에서 oldBrush 백업했으니 바로 교체
	Rectangle(mDC, rectView.left + 5, rectView.top + 5, rectView.left + 5 + (int)(FualRate * 400), rectView.top + 45);

	// 글자 출력
	HFONT oldFont = (HFONT)SelectObject(mDC, hFont);
	SetBkMode(mDC, TRANSPARENT); // 글자 배경 투명

	wchar_t str[64];
	wsprintf(str, L"%d / %d", (int)m_fual, (int)m_MaxFual);
	TextOut(mDC, rectView.left + 165, rectView.top + 15, str, lstrlen(str));

	// 7. 제일 중요한 GDI 리소스 원상복구 (메모리 누수 방지)
	SelectObject(mDC, oldBrush);
	SelectObject(mDC, oldFont);
}

void Player::ConsumeFual()
{
	int stage = GameMap::GetStage();
	m_fual -= (1 + stage * 1.2 );
	if (m_fual < 0) {
		m_fual = 0;
		m_hp = 0;
	}
}


// Set 함수
// 공격 종류 설정 함수
void Player::SetAttackType(int type) 
{
	attackType = type;
	if (attackType == 0)
	{
		m_AttackSpeed = m_AttackSpeed_type_0;
	}
	else if (attackType == 1) {
		m_AttackSpeed = m_AttackSpeed_type_1;
	}
	m_AttackTypeChanged = true;
}

void Player::SetSearchBox(double deg)
{
	SearchBox += deg;
}

// 연료 증가 함수
// 2026.06.05 최대 연료를 증가하도록 수정.
void Player::SetFual(double deg)
{
	m_MaxFual += deg;
}

// 속도 증가 함수
void Player::SetSpeed(double deg)
{
	m_speed += deg;
}

// 공격력 증가 함수
void Player::SetAttackPower(double deg)
{
	m_attackPower += deg;
}

void Player::SetAttackSpeed(double deg)
{
	m_AttackSpeed_type_0 -= deg;
	m_AttackSpeed_type_1 -= deg * 2;

	if (m_AttackSpeed_type_0 <= 0) {
		m_AttackSpeed_type_0 = 0;
	}
	if (m_AttackSpeed_type_1 <= 0) {
		m_AttackSpeed_type_1 = 0;
	}
}

void Player::SetAttackTypeChanged(bool b)
{
	m_AttackTypeChanged = b;
}

void Player::HealPlayer(double heal)
{
	m_fual += heal;
	if (m_fual > m_MaxFual) m_fual = m_MaxFual;
}

void Player::SetAngle(double mouseX, double mouseY)
{
	double dx = mouseX - m_x;
	double dy = mouseY - m_y;
	m_angle = atan2(dy, dx); // 라디안
}