#include<iostream>
#include "GameMap.h"

int GameMap::m_rockNum = 0;
int GameMap::m_stage = 1;

GameMap::GameMap()
	: TickRate(16) // 60 프레임으로 설정
{
	m_goal = 50;
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

bool GameMap::isfull() const
{
	return m_rockNum >= m_goal;
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

void GameMap::NextStage()
{
	m_stage++;
	SetRockNum();
}

void GameMap::SetRockNum()
{
	m_rockNum = 0;
}

void GameMap::Draw(HDC mDC, RECT rectView, HBRUSH hBrush[], HFONT hFont)
{
	double BreakRate = (double)m_rockNum / m_goal;

	double midRECT = (rectView.left + rectView.right) / 2;

	if (m_rockNum > m_goal) {
		m_rockNum = m_goal;
	}

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