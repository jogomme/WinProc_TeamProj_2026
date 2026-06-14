#include<iostream>
#include "GameMap.h"
#include<Windows.h>
#include<random>

std::default_random_engine gen_gMap(time(NULL));
std::uniform_int_distribution<int> dist_gMap(0, 4);

int GameMap::m_rockNum = 0;
int GameMap::m_stage = 1;
MinRock GameMap::m_rock[3] = {};
int GameMap::m_rare = 0;

GameMap::GameMap()
	: TickRate(16) // 60 프레임으로 설정
{
	m_goal = 3;
	MaximumRock = 10;
}

GameMap::~GameMap()
{

}

int GameMap::GetStage() const
{
	return m_stage;
}

int GameMap::GetTickRate() const
{
	return TickRate;
}

int GameMap::GetMaximumRock() const
{
	return MaximumRock;
}

int GameMap::GetRare() const
{
	return m_rare;
}

void GameMap::SetRare(int deg)
{
	m_rare = deg;
}

bool GameMap::isfull() const
{
	return m_rockNum >= m_goal;
}

bool GameMap::isNextStage() const
{
	return m_isNextStage;
}

void GameMap::SetStage(int stage)
{
	if (stage <= 0) {
		m_stage = 1;
		return;
	}
	m_stage = stage;
}

void GameMap::SetMaximumRock(int PlusNum)
{
	MaximumRock += PlusNum;
}

void GameMap::SetMGoal(int goal)
{
	m_goal = goal;
}

void GameMap::NextStage()
{
	m_stage++;
	SetRockNum();
	m_isNextStage = true;
}

void GameMap::SetRockNum()
{
	m_rockNum = 0;
}

void GameMap::Draw(HDC mDC, RECT rectView, HBRUSH hBrush[], HFONT hFont)
{
	double midRECT = (rectView.left + rectView.right) / 2;

	if (m_rockNum > m_goal) {
		m_rockNum = m_goal;
	}
	double BreakRate = (double)m_rockNum / m_goal;

	HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush[7]);
	Rectangle(mDC, midRECT - 200, rectView.bottom - 45, midRECT + 200, rectView.bottom - 5);
	oldBrush = (HBRUSH)SelectObject(mDC, hBrush[1]);
	Rectangle(mDC, midRECT - 200, rectView.bottom - 45, midRECT - 200 + (BreakRate) * 400, rectView.bottom - 5);

	HFONT oldFont = (HFONT)SelectObject(mDC, hFont);
	SetBkMode(mDC, TRANSPARENT); // 글자 배경 투명

	wchar_t str[64];

	int f = m_rockNum;
	int mf = m_goal;
	wsprintf(str, L"%d / %d", f, mf);
	TextOut(mDC, midRECT - 30, rectView.bottom - 35, str, lstrlen(str));
}

void GameMap::DrawNextStage(HDC mDC, RECT rectView, HBRUSH hBrush[], HFONT hFont)
{
	HFONT oldFont = (HFONT)SelectObject(mDC, hFont);
	SetBkMode(mDC, TRANSPARENT); // 글자 배경 투명

	int middleX = (rectView.left + rectView.right) / 2;
	int middleY = (rectView.top + rectView.bottom) / 2;

	wchar_t str[64];
	wchar_t strRare[64];
	HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush[7]);
	Rectangle(mDC, 0, 0, rectView.right, rectView.bottom);

	wsprintf(str, L"Stage %d", m_stage);
	TextOut(mDC, middleX - 30, middleY - 15, str, lstrlen(str));
	if (m_rare == 0) {
		wsprintf(strRare, L"Rarity : Normal");
		TextOut(mDC, middleX - 30, middleY + 15, strRare, lstrlen(strRare));
	}
	else if (m_rare == 1) {
		wsprintf(strRare, L"Rarity : Rare");
		TextOut(mDC, middleX - 30, middleY + 15, strRare, lstrlen(strRare));
	}
	else if (m_rare == 2) {
		wsprintf(strRare, L"Rarity : Unique");
		TextOut(mDC, middleX - 30, middleY + 15, strRare, lstrlen(strRare));
	}
	else if (m_rare == 3) {
		wsprintf(strRare, L"Rarity : Epic");
		TextOut(mDC, middleX - 30, middleY + 15, strRare, lstrlen(strRare));
	}
	else if (m_rare == 4) {
		wsprintf(strRare, L"Rarity : Legendary");
		TextOut(mDC, middleX - 30, middleY + 15, strRare, lstrlen(strRare));
	}
}

void GameMap::StartRarityShuffle()
{
	m_shuffleCount = 0;
	m_maxShuffle = 20; // 20번 섞임 (100ms 타이머 기준 2초 동안 연출)
}

bool GameMap::UpdateRarityShuffle()
{
	if (m_shuffleCount < m_maxShuffle) {
		m_rare = dist_gMap(gen_gMap); // 텍스트 랜덤 변경
		m_shuffleCount++;
		return true; // 아직 셔플 중
	}

	m_isNextStage = false; // 셔플이 끝나면 다음 스테이지 텍스트 끄기
	return false; // 셔플 완료
}