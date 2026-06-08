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

	// VOLUME 1000 ~ 0
	mciSendString(L"setaudio BGM_Fight volume to 700", NULL, 0, NULL);
	mciSendString(L"setaudio EFFECT_Shoot volume to 100", NULL, 0, NULL);
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