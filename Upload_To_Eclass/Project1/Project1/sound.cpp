#include<Windows.h>
#include<windowsx.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include "resource.h"

void Open_Sound()
{
	// BGM
	mciSendString(L"open \"sound\\music\\lobby.wav\" type mpegvideo alias BGM_Lobby", NULL, 0, NULL);
	mciSendString(L"open \"sound\\music\\fight.wav\" type mpegvideo alias BGM_Fight", NULL, 0, NULL);
	mciSendString(L"open \"sound\\music\\boss.wav\" type mpegvideo alias BGM_Boss", NULL, 0, NULL);

	// EFFECT
	mciSendString(L"open \"sound\\effect\\click1.wav\" type mpegvideo alias EFFECT_Click1", NULL, 0, NULL);
	mciSendString(L"open \"sound\\effect\\enforce.wav\" type mpegvideo alias EFFECT_Enforce", NULL, 0, NULL);
	mciSendString(L"open \"sound\\effect\\fail_enforce.wav\" type mpegvideo alias EFFECT_FEnforce", NULL, 0, NULL);
	mciSendString(L"open \"sound\\effect\\plinko_coin.wav\" type mpegvideo alias EFFECT_Coin", NULL, 0, NULL);
	mciSendString(L"open \"sound\\effect\\shoot.wav\" type mpegvideo alias EFFECT_Shoot", NULL, 0, NULL);
}
void Stop_BGM()
{
	mciSendString(L"stop BGM_Lobby", NULL, 0, NULL);
	mciSendString(L"stop BGM_Fight", NULL, 0, NULL);
	mciSendString(L"stop BGM_Boss", NULL, 0, NULL);
}
void Quit_SoundAll()
{
	mciSendString(L"close BGM_Lobby", NULL, 0, NULL);
	mciSendString(L"close BGM_Fight", NULL, 0, NULL);
	mciSendString(L"close BGM_Boss", NULL, 0, NULL);

	mciSendString(L"close EFFECT_Click1", NULL, 0, NULL);
	mciSendString(L"close EFFECT_Enforce", NULL, 0, NULL);
	mciSendString(L"close EFFECT_FEnforce", NULL, 0, NULL);
	mciSendString(L"close EFFECT_Coin", NULL, 0, NULL);
	mciSendString(L"close EFFECT_Shoot", NULL, 0, NULL);
}

void Play_Sound_BGM(const wchar_t name[50])
{
	wchar_t cmd[128] = {};
	wsprintf(cmd, L"seek %s to start", name);
	mciSendString(cmd, NULL, 0, NULL);
	wsprintf(cmd, L"play %s repeat", name);
	mciSendString(cmd, NULL, 0, NULL);
}

void Play_Sound(const wchar_t name[50])
{
	wchar_t cmd[128] = {};
	wsprintf(cmd, L"seek %s to start", name);
	mciSendString(cmd, NULL, 0, NULL);
	wsprintf(cmd, L"play %s", name);
	mciSendString(cmd, NULL, 0, NULL);
}

void Set_Volume(int v)
{
	wchar_t cmd[128] = {};
	wsprintf(cmd, L"setaudio BGM_Lobby volume to %d", v * 100);
	mciSendString(cmd, NULL, 0, NULL);
	wsprintf(cmd, L"setaudio BGM_Fight volume to %d", v * 70);
	mciSendString(cmd, NULL, 0, NULL);
	wsprintf(cmd, L"setaudio BGM_Boss volume to %d", v * 100);
	mciSendString(cmd, NULL, 0, NULL);
	wsprintf(cmd, L"setaudio EFFECT_Click1 volume to %d", v * 100);
	mciSendString(cmd, NULL, 0, NULL);
	wsprintf(cmd, L"setaudio EFFECT_Enforce volume to %d", v * 100);
	mciSendString(cmd, NULL, 0, NULL);
	wsprintf(cmd, L"setaudio EFFECT_FEnforce volume to %d", v * 100);
	mciSendString(cmd, NULL, 0, NULL);
	wsprintf(cmd, L"setaudio EFFECT_Coin volume to %d", v * 100);
	mciSendString(cmd, NULL, 0, NULL);
	wsprintf(cmd, L"setaudio EFFECT_Shoot volume to %d", v * 10);
	mciSendString(cmd, NULL, 0, NULL);
}