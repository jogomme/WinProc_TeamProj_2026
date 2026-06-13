#include<Windows.h>
#include<windowsx.h>
#include<time.h>
#include<iostream>
#include<string.h>
#include<math.h>
#include<queue>
#include<random>
#pragma comment (lib, "msimg32.lib")
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include "resource.h"
#include "GameObject.h"
#include "Player.h"
#include "Rock.h"
#include "GameMap.h"
#include "Plinko.h"
#include "Enforce.h"
#include "Feed.h"
#include "Bullet.h"
#include "sound.h"
#include "Boss.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"My Window Class";
LPCTSTR lpszWindowName = L"Windows Program TeamProject";

// 화면 크기 구성
int width = 1280;
int height = 800;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL; // MAKEINTRESOURCE(IDR_MENU1); // 메뉴가 필요한 경우
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, width, height, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
//-----------------------------------------------------------------------------------------------
// 랜덤값 선언 구간
//-----------------------------------------------------------------------------------------------

std::default_random_engine dre(time(NULL));
std::uniform_int_distribution<int> uid(0, 255);

//-----------------------------------------------------------------------------------------------
// 함수 선언 구간
//-----------------------------------------------------------------------------------------------

int Enforce_Point_Calc(Point rectViewMid, char c, int xy, int intrv);
void CALLBACK TimerProc(HWND hWnd, UINT iMsg, UINT idEvent, DWORD dwTime);
void GameStart(HWND hWnd, RECT& rect, int mx, int my, int& WinSinec);
void GameOver(HWND hWnd);

void DrawFight(HDC mDC, HWND hWnd, RECT rectView, HBRUSH hBrush[], HFONT hFont);
void GameSleep(HWND hWnd);
void CheckBossMerge();
//-----------------------------------------------------------------------------------------------
// 전역 변수 선언 구간
//-----------------------------------------------------------------------------------------------

// 콘솔 창 띄우는 용도입니다. Debug(디버그) 용도입니다.
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#define MAX_ROCKS 50
#define MAX_BULLETS 150
#define MAX_ENFORCE 500
#define MAX_FEED 150

Player player;
Rock rock[MAX_ROCKS];
Bullet bullet[MAX_BULLETS];
GameMap gMap;
Enforce enforce[MAX_ENFORCE];
Feed feed[MAX_FEED];
Boss boss;

// 현재 마우스 커서의 위치
int xPos{};
int yPos{};

// 현재 최대 총알 갯수
int BulletCnt{};

// 스테이지 넘어가기 까지 눌러야하는 스페이스바 꾹 수, 3초 기준
const int isSpaceBarDown = 85;

// 지금 얼마나 눌렀는지
int isDowning = 0;

// 무기 상자 시간 변수, 기본 6초 추후 늘려도 됌
unsigned MAX_AttackTypeChanged = 170;
unsigned isAttackTypeChanged = 170;

// 게임 상태 변수
bool isGaming = false;

// 캐릭터들 상태 타이머 변수
const int GoMove{ 1 };
const int GoAttack{ 2 };
const int GoConsumeFual{ 3 };
const int GoShow{ -1 };
const int SetAttackType{ 4 };
const int GoRarityShuffle{ 5 };
const int PlinkoTimer{ 6 };
const int StoneTimer{ 7 };
const int GoBossAttack{ 8 };


// 현재 어느 화면을 띄울 것인가 // 0 - 메인 화면, 1 - 플레이어 강화 창, 2 - 전투 화면, 3 - 설정 창, 4 - 플링코 화면
int window_scene{ 0 };

// 스테이지 이동시 회복할 확률
int heal_percent{ 0 };
double attack_heal{ 0 };

// 설정창 버튼
#define IDC_BUTTON1 1001
#define IDC_BUTTON2 1002
#define IDC_EDIT 2001
int volume{ 10 };

//-----------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
//-----------------------------------------------------------------------------------------------
{
	PAINTSTRUCT ps;
	HDC hDC, mDC, imgDC; // hDC - 최종 출력 화면 / mDC - 더블 퍼버링용. 대부분의 그림 출력은 여기에 / imgDC - 그림 선택할 때 사용.
	HBITMAP hBitmap; // 비트맵
	static HBITMAP imgBitmap[50]; // 이미지 로딩
	static BITMAP imgBmp; // 이미지 크기 잡는용, 배경 잡는용으로 썼는데, 배열로 바꿔서 써도 될듯
	static HPEN hPen[10], oldPen;
	static HBRUSH hBrush[10], oldBrush;
	static HFONT hFont, oldFont;

	static Point point[10]; // Polygon 함수를 위한 변수. 그때그때 좌표 적어서 활용

	static RECT rectView; // "보이는" 화면 크기
	static Point rectViewMid; // "보이는" 화면 중앙
	static SIZE size; // 화면 사이즈 변경시. WM_SIZE
	static int mx, my; // 마우스 클릭 좌표
	static int timercnt; // 타이머 갯수, 종료 및 초기화 등에서 KillTimer 함수를 위한 갯수를 저장. WM_CREATE 에서 갯수 저장할 것


	// 강화 버튼
	static int enforce_size; // 버튼의 반지름 크기
	static int enforce_intrv; // 버튼 사이 간격
	static int drag; // 마우스 드래그 중인지 체크
	static Point drag_start; // 드래그 시작점 저장

	static int return_setting; // 세팅 화면에 오기 전, 어디 화면 이었는지 저장

	static HWND hButton1, hButton2, hEdit; // 설정 버튼

	switch (iMessage) {
	case WM_CREATE:
		srand(time(NULL));
		// 배경
		imgBitmap[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		imgBitmap[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		imgBitmap[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
		imgBitmap[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
		imgBitmap[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));
		GetObject(imgBitmap[0], sizeof(BITMAP), &imgBmp); // 배경 사이즈

		// 강화 이미지
		imgBitmap[5] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6)); // attack
		imgBitmap[6] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP9)); // speed
		imgBitmap[7] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8)); // fuel
		imgBitmap[8] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7)); // attack_speed
		imgBitmap[9] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP10)); // type1
		imgBitmap[10] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12)); // type3
		imgBitmap[11] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11)); // type2

		// 버튼 이미지
		imgBitmap[12] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP13)); // Done
		imgBitmap[13] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP14)); // Enforce
		imgBitmap[14] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP15)); // Go Fight
		imgBitmap[15] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP16)); // Plinko
		imgBitmap[16] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP17)); // Return Enforce
		imgBitmap[17] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP18)); // Setting
		imgBitmap[18] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP19)); // Game Start

		// 오브젝트
		imgBitmap[19] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP20)); // bullet
		imgBitmap[20] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP21)); // plinko
		imgBitmap[21] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP22)); // rocket
		imgBitmap[22] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP23)); // stone1
		imgBitmap[23] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP24)); // stone2
		imgBitmap[24] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP25)); // stone3
		imgBitmap[25] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP26)); // stone4

		GetClientRect(hWnd, &rectView);
		rectViewMid.x = (rectView.left + rectView.right) / 2;
		rectViewMid.y = (rectView.top + rectView.bottom) / 2;

		hPen[0] = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));

		hBrush[0] = CreateSolidBrush(RGB(0, 0, 0)); //black
		hBrush[1] = CreateSolidBrush(RGB(255, 0, 0)); //red
		hBrush[2] = CreateSolidBrush(RGB(0, 255, 0)); //blue
		hBrush[3] = CreateSolidBrush(RGB(0, 0, 255)); //green
		hBrush[4] = CreateSolidBrush(RGB(255, 255, 0)); //yellow
		hBrush[5] = CreateSolidBrush(RGB(255, 0, 255)); //magenta
		hBrush[6] = CreateSolidBrush(RGB(0, 255, 255)); //cyan
		hBrush[7] = CreateSolidBrush(RGB(255, 255, 255)); //white
		hBrush[8] = CreateSolidBrush(RGB(70, 70, 70)); //gray

		// 크기, 0, 0, 0, 두께(BOLD, NORMAL), ,,, 글꼴 이름
		hFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
			HANGUL_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, L"맑은 고딕");

		timercnt = 0;

		window_scene = 0; // 현재 시작시 화면. 기본값: 0 / 현재 테스트 중인 값: n

		// 강화 버튼 관련
		{
			enforce_size = 20;
			enforce_intrv = 80;

			for (int i = 0; i < MAX_ENFORCE; i++) {
				enforce[i].Start_Setting(enforce[i], i);
				enforce[i].Move_Mid_Pos(rectViewMid, enforce_intrv);
			}
		}

		drag = 0;


		hButton1 = CreateWindow(L"button", L"Down", WS_CHILD | BS_PUSHBUTTON,
			rectViewMid.x - 120, rectViewMid.y - 20, 50, 40, hWnd, (HMENU)IDC_BUTTON1, g_hInst, NULL);
		hButton2 = CreateWindow(L"button", L"Up", WS_CHILD | BS_PUSHBUTTON,
			rectViewMid.x + 70, rectViewMid.y - 20, 50, 40, hWnd, (HMENU)IDC_BUTTON2, g_hInst, NULL);

		hEdit = CreateWindow(L"edit", L"", WS_CHILD | ES_READONLY,
			rectViewMid.x - 50, rectViewMid.y - 20, 100, 40, hWnd, (HMENU)IDC_EDIT, g_hInst, NULL);

		{
			TCHAR edit_str[50] = L"";
			swprintf_s(edit_str, 50, L"Volume: %d: ", volume);
			SetDlgItemText(hWnd, IDC_EDIT, edit_str);
		}

		Open_Sound();

		Set_Volume(volume);

		Play_Sound_BGM(L"BGM_Lobby");

		SetTimer(hWnd, StoneTimer, 50, (TIMERPROC)TimerProc);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1:
			volume -= 1;
			if (volume < 0) volume = 0;
			{
				TCHAR edit_str[50] = L"";
				swprintf_s(edit_str, 50, L"Volume: %d ", volume);
				SetDlgItemText(hWnd, IDC_EDIT, edit_str);
			}
			Set_Volume(volume);
			break;

		case IDC_BUTTON2:
			volume += 1;
			if (volume > 10) volume = 10;
			{
				TCHAR edit_str[50] = L"";
				swprintf_s(edit_str, 50, L"Volume: %d ", volume);
				SetDlgItemText(hWnd, IDC_EDIT, edit_str);
			}
			Set_Volume(volume);
			break;
		}

		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_SIZE:
		height = HIWORD(lParam);
		width = LOWORD(lParam);
		GetClientRect(hWnd, &rectView);
		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_KEYDOWN:
		if (wParam == 'Q') {
			for (int i = 1; i <= timercnt; i++) {
				KillTimer(hWnd, i);
			}
			Stop_BGM();
			Quit_SoundAll();
			PostQuitMessage(0);
			return 0;
		}

		else if (wParam == 'M') {
			std::cout << "Show Me The Money" << '\n';
			SetMoney(10000);
			player.SetFual(10000);
		}

		else if (wParam == VK_SPACE) {
			if (gMap.isfull()) {
				isDowning++;
				if (isDowning >= isSpaceBarDown) {
					gMap.NextStage();
					player.SetAttackType(0);
					for (int i = 0; i < MAX_FEED; ++i) {
						feed[i].SetActive(false);
					}
					gMap.StartRarityShuffle();
					SetTimer(hWnd, GoRarityShuffle, 100, (TIMERPROC)TimerProc); // 100ms마다 갱신
					isDowning = 0; // 스페이스바 게이지 초기화
					GameSleep(hWnd);

					int rand_temp = rand() % 100 + 1;
					if (rand_temp <= heal_percent) {
						player.HealPlayer(player.GetMaxFual());
					}
				}
			}
		}

		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_KEYUP: {
		if (wParam == VK_SPACE) {
			isDowning = 0;
		}
	}
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		Play_Sound(L"EFFECT_Click1");

		// 메인 화면
		if (window_scene == 0) {
			if (mx > rectViewMid.x - 70 && mx< rectViewMid.x + 70 &&
				my>rectViewMid.y - 50 && my < rectViewMid.y + 20) {
				GameStart(hWnd, rectView, mx, my, window_scene);
			}
			else if (mx > rectViewMid.x - 70 && mx < rectViewMid.x + 70 &&
				my>rectViewMid.y + 30 && my < rectViewMid.y + 100) {

				return_setting = window_scene;
				window_scene = 3;

				ShowWindow(hButton1, SW_SHOW);
				ShowWindow(hButton2, SW_SHOW);
				ShowWindow(hEdit, SW_SHOW);
			}
		}

		// 강화 화면
		else if (window_scene == 1) {
			//UseMoney(-5); // 테스트를 위한 돈 벌기 한 줄
			// 전투 진입 버튼 클릭
			if (mx > rectView.left + 5 && mx < rectView.left + 205 &&
				my > rectView.bottom - 85 && my < rectView.bottom - 5) {
				GameStart(hWnd, rectView, mx, my, window_scene);
				window_scene = 2;
				break;
			}

			// 플링코 진입 버튼
			else if (mx > rectView.left + 5 && mx < rectView.left + 105 &&
				my > rectView.top + 5 && my < rectView.top + 45) {
				window_scene = 4;
				plinkoStart = TRUE;
				break;
			}

			// 강화 진입 버튼
			else if (mx > rectView.left + 115 && mx < rectView.left + 215 &&
				my > rectView.top + 5 && my < rectView.top + 45) {
				window_scene = 1;
				break;
			}

			// 세팅 진입 버튼
			else if (mx > rectView.left + 225 && mx < rectView.left + 325 &&
				my > rectView.top + 5 && my < rectView.top + 45) {
				return_setting = window_scene;
				window_scene = 3;

				ShowWindow(hButton1, SW_SHOW);
				ShowWindow(hButton2, SW_SHOW);
				ShowWindow(hEdit, SW_SHOW);
				break;
			}

			// 강화 버튼 클릭
			int ck = 0;
			for (int i = 0; i < MAX_ENFORCE; i++) {
				if (mx > enforce[i].Get_Enforce_Point_x() - enforce_size && mx < enforce[i].Get_Enforce_Point_x() + enforce_size &&
					my > enforce[i].Get_Enforce_Point_y() - enforce_size && my < enforce[i].Get_Enforce_Point_y() + enforce_size &&
					enforce[i].Get_Enforce_Draw() == 1 && enforce[i].Get_Enforce_Open() == 0) {
					// 가격 관련 조건, 마이너스 필요
					if (GetMoney() < enforce[i].Get_Enforce_Price()) {
						Play_Sound(L"EFFECT_FEnforce");
					}
					else {
						UseMoney(enforce[i].Get_Enforce_Price());
						Play_Sound(L"EFFECT_Enforce");
						ck = 1;
						enforce[i].Set_Open(1);

						// 강화 버튼이 눌러졌을 때 그 다음에 켜지는 강화 버튼이 없다면
						// MaximumRocks를 침범하여 강제로 -1로 고정하기 때문에
						// 이러한 코드를 사용하여 방지하였습니다.
						int next1 = enforce[i].Get_Enforce_Drawing(0);
						int next2 = enforce[i].Get_Enforce_Drawing(1);
						int next3 = enforce[i].Get_Enforce_Drawing(2);

						if (next1 >= 0 && next1 < MAX_ENFORCE) enforce[next1].Set_Draw(1);
						if (next2 >= 0 && next2 < MAX_ENFORCE) enforce[next2].Set_Draw(1);
						if (next3 >= 0 && next3 < MAX_ENFORCE) enforce[next3].Set_Draw(1);

						double amount = enforce[i].Get_Enforce_Amount();
						//type 1-공격, 2-이동속도, 3-연료, 4-공격속도
						if (enforce[i].Get_Enforce_Type() == 1) {
							player.SetAttackPower(amount);
						}
						else if (enforce[i].Get_Enforce_Type() == 2) {
							player.SetSpeed(amount);
						}
						else if (enforce[i].Get_Enforce_Type() == 3) {
							player.SetFual(amount);
						}
						else if (enforce[i].Get_Enforce_Type() == 4) {
							player.SetAttackSpeed(amount);
						}
						else if (enforce[i].Get_Enforce_Type() == 5) {
							// 무기상자 - 속도업 해금
							for (int i = 0; i < MAX_ROCKS; i++) {
								rock[i].UnlockRockType(3);
							}
						}
						else if (enforce[i].Get_Enforce_Type() == 6) {
							heal_percent += enforce[i].Get_Enforce_Amount();
						}
						else if (enforce[i].Get_Enforce_Type() == 7) {
							attack_heal += enforce[i].Get_Enforce_Amount();
						}
					}
				}
			}

			// 빈 공간 클릭
			if (ck == 0 && window_scene == 1) {
				drag = 1;
				drag_start.x = mx;
				drag_start.y = my;
			}
		}

		else if (window_scene == 2) {
			// 강제 사망 (임시)
			if ((mx > rectView.left + 5 && mx < rectView.left + 35 &&
				my > rectView.top + 50 && my < rectView.top + 80)) {
				GameOver(hWnd);
			}
			if (isGaming == false) {
				if ((mx > rectViewMid.x - 70 && mx < rectViewMid.x + 70 &&
					my > rectViewMid.y - 50 && my < rectViewMid.y + 20)) {
					window_scene = 1;
					Stop_BGM();
					Play_Sound_BGM(L"BGM_Lobby");
				}
			}
		}
		// 설정 화면
		else if (window_scene == 3) {
			if (mx > rectViewMid.x - 50 && mx < rectViewMid.x + 50 &&
				my > rectViewMid.y + 90 && my < rectViewMid.y + 140) {
				window_scene = return_setting;

				ShowWindow(hButton1, SW_HIDE);
				ShowWindow(hButton2, SW_HIDE);
				ShowWindow(hEdit, SW_HIDE);
			}
		}
		// 플링코 화면
		else if (window_scene == 4) {

			// 플링코 진입 버튼
			if (mx > rectView.left + 5 && mx < rectView.left + 105 &&
				my > rectView.top + 5 && my < rectView.top + 45) {
				window_scene = 4;
				break;
			}

			// 강화 진입 버튼
			else if (mx > rectView.left + 115 && mx < rectView.left + 215 &&
				my > rectView.top + 5 && my < rectView.top + 45) {
				window_scene = 1;
				break;
			}

			// 세팅 진입 버튼
			else if (mx > rectView.left + 225 && mx < rectView.left + 325 &&
				my > rectView.top + 5 && my < rectView.top + 45) {
				return_setting = window_scene;
				window_scene = 3;
				break;
			}

		}

		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_MOUSEMOVE:
		xPos = GET_X_LPARAM(lParam);
		yPos = GET_Y_LPARAM(lParam);
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		if (drag == 1) {
			for (int i = 0; i < MAX_ENFORCE; i++) {
				double x = enforce[i].Get_Enforce_Point_x();
				double y = enforce[i].Get_Enforce_Point_y();
				enforce[i].Set_Pos(x - (drag_start.x - mx) / 2, y - (drag_start.y - my) / 2);
			}
			drag_start.x = mx;
			drag_start.y = my;

		}
		if (window_scene == 1) {
			InvalidateRect(hWnd, NULL, false);

		}
		break;

	case WM_LBUTTONUP:
		if (drag == 1) drag = 0;
		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_PAINT: {
		// 기초 로드
		hDC = BeginPaint(hWnd, &ps);
		mDC = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, rectView.right, rectView.bottom);
		SelectObject(mDC, (HBITMAP)hBitmap);
		Rectangle(mDC, 0, 0, rectView.right, rectView.bottom);
		imgDC = CreateCompatibleDC(mDC);

		HBITMAP oldHBitmap = (HBITMAP)SelectObject(mDC, hBitmap);

		// 필요 내용 작성은 이 밑으로.

		oldPen = (HPEN)SelectObject(mDC, hPen[0]);
		// 게임 시작 화면
		if (window_scene == 0) {
			// 배경
			SelectObject(imgDC, imgBitmap[0]);
			StretchBlt(mDC, 0, 0, rectView.right, rectView.bottom, imgDC, 0, 0, imgBmp.bmWidth, imgBmp.bmHeight, SRCCOPY);

			// start
			SelectObject(imgDC, imgBitmap[18]);
			TransparentBlt(mDC, rectViewMid.x - 70, rectViewMid.y - 50, 140, 70, imgDC, 0, 0, 400, 200, RGB(255, 255, 255));

			// setting
			SelectObject(imgDC, imgBitmap[17]);
			TransparentBlt(mDC, rectViewMid.x - 70, rectViewMid.y + 30, 140, 70, imgDC, 0, 0, 400, 200, RGB(255, 255, 255));
		}
		// 강화 화면
		else if (window_scene == 1) {
			MoneyBoxDraw(mDC);
			oldBrush = (HBRUSH)SelectObject(mDC, hBrush[2]);

			for (int i = 0; i < MAX_ENFORCE; i++) {
				// 블럭 오픈 여부에 따른 색
				oldBrush = (HBRUSH)SelectObject(mDC, hBrush[7 - enforce[i].Get_Enforce_Open() * 3]);
				// 블럭 그리기
				if (enforce[i].Get_Enforce_Draw() == 1) {
					// 타입에 따른 이미지
					double x = enforce[i].Get_Enforce_Point_x();
					double y = enforce[i].Get_Enforce_Point_y();
					Rectangle(mDC, x - enforce_size, y - enforce_size, x + enforce_size, y + enforce_size);

					if (enforce[i].Get_Enforce_Type()) {
						SelectObject(imgDC, imgBitmap[4 + enforce[i].Get_Enforce_Type()]);
						TransparentBlt(mDC, x - enforce_size, y - enforce_size, enforce_size * 2, enforce_size * 2, imgDC, 0, 0, 400, 400, RGB(255, 255, 255));
					}
					oldFont = (HFONT)SelectObject(mDC, hFont);
					SetBkMode(mDC, TRANSPARENT); // 글자 배경 투명
					wchar_t str[64];

					//type 1-공격, 2-이동속도, 3-연료, 4-공격속도
					if (enforce[i].Get_Enforce_Type() == 1) {
						wsprintf(str, L"공격력");
					}
					else if (enforce[i].Get_Enforce_Type() == 2) {
						wsprintf(str, L"이동속도");
					}
					else if (enforce[i].Get_Enforce_Type() == 3) {
						wsprintf(str, L"연료");
					}
					else if (enforce[i].Get_Enforce_Type() == 4) {
						wsprintf(str, L"공격속도");
					}
					else if (enforce[i].Get_Enforce_Type() == 5) {
						wsprintf(str, L"속도업 해금");
					}
					else if (enforce[i].Get_Enforce_Type() == 6) {
						wsprintf(str, L"스테이지 회복 확률");
					}
					else if (enforce[i].Get_Enforce_Type() == 7) {
						wsprintf(str, L"공격시 연료 회복");
					}
					else {
						wsprintf(str, L"미구현");
					}
					TextOut(mDC, x - 25, y + 20, str, lstrlen(str));

					double amount = enforce[i].Get_Enforce_Amount();
					wsprintf(str, L"%d", (int)amount);
					TextOut(mDC, x - 25, y + 40, str, lstrlen(str));
				}
			}

			// 전투 화면 진입 버튼
			// Go Fight
			SelectObject(imgDC, imgBitmap[14]);
			TransparentBlt(mDC, rectView.left + 5, rectView.bottom - 85, 200, 80, imgDC, 0, 0, 400, 200, RGB(255, 255, 255));

			// 플링코 화면 진입 버튼
			// Plinko
			SelectObject(imgDC, imgBitmap[15]);
			TransparentBlt(mDC, rectView.left + 5, rectView.top + 5, 100, 40, imgDC, 0, 0, 400, 200, RGB(255, 255, 255));

			// 강화 화면 진입 버튼
			// Enforce
			SelectObject(imgDC, imgBitmap[13]);
			TransparentBlt(mDC, rectView.left + 115, rectView.top + 5, 100, 40, imgDC, 0, 0, 400, 200, RGB(255, 255, 255));

			// 설정 화면 진입 버튼
			// Setting
			SelectObject(imgDC, imgBitmap[17]);
			TransparentBlt(mDC, rectView.left + 225, rectView.top + 5, 100, 40, imgDC, 0, 0, 400, 200, RGB(255, 255, 255));

		}
		// 전투 화면
		else if (window_scene == 2) {
			HBITMAP oldImgBmp = NULL;
			// 배경
			if (gMap.GetRare() == 0) {
				oldImgBmp = (HBITMAP)SelectObject(imgDC, imgBitmap[0]);
			}
			else if (gMap.GetRare() == 1) {
				oldImgBmp = (HBITMAP)SelectObject(imgDC, imgBitmap[1]);
			}
			else if (gMap.GetRare() == 2) {
				oldImgBmp = (HBITMAP)SelectObject(imgDC, imgBitmap[2]);
			}
			else if (gMap.GetRare() == 3) {
				oldImgBmp = (HBITMAP)SelectObject(imgDC, imgBitmap[3]);
			}
			else if (gMap.GetRare() == 4) {
				oldImgBmp = (HBITMAP)SelectObject(imgDC, imgBitmap[4]);
			}
			StretchBlt(mDC, 0, 0, rectView.right, rectView.bottom, imgDC, 0, 0, imgBmp.bmWidth, imgBmp.bmHeight, SRCCOPY);

			SelectObject(imgDC, oldImgBmp);

			DrawFight(mDC, hWnd, rectView, hBrush, hFont);

			// 강제 사망 (임시)
			{
				oldBrush = (HBRUSH)SelectObject(mDC, hBrush[7]);
				Rectangle(mDC, rectView.left + 5, rectView.top + 50, rectView.left + 35, rectView.top + 80);

				oldFont = (HFONT)SelectObject(mDC, hFont);
				SetBkMode(mDC, TRANSPARENT); // 글자 배경 투명
				wchar_t str[64];
				wsprintf(str, L"Death");
				TextOut(mDC, rectView.left + 5, rectView.top + 50, str, lstrlen(str));
			}
			// 사망시 게임 오버 버튼
			{
				if (isGaming == false) {
					// Return Enforce
					SelectObject(imgDC, imgBitmap[16]);
					TransparentBlt(mDC, rectViewMid.x - 70, rectViewMid.y - 50, 140, 70, imgDC, 0, 0, 400, 200, RGB(255, 255, 255));

					oldFont = (HFONT)SelectObject(mDC, hFont);
					SetBkMode(mDC, TRANSPARENT); // 글자 배경 투명
					wchar_t str[64];
					wsprintf(str, L"Game Over");
					TextOut(mDC, rectViewMid.x - 60, rectViewMid.y - 50, str, lstrlen(str));
				}
			}
		}
		// 설정 화면
		else if (window_scene == 3) {
			// 배경
			SelectObject(imgDC, imgBitmap[0]);
			StretchBlt(mDC, 0, 0, rectView.right, rectView.bottom, imgDC, 0, 0, imgBmp.bmWidth, imgBmp.bmHeight, SRCCOPY);

			// Done
			SelectObject(imgDC, imgBitmap[12]);
			TransparentBlt(mDC, rectViewMid.x - 50, rectViewMid.y + 90, 100, 50, imgDC, 0, 0, 400, 200, RGB(255, 255, 255));
		}

		else if (window_scene == 4) {

			if (plinkoEmptyCheck() && plinkoStart) {

				PlinkoRestart();
				PlinkoRock::plinkoNumInit();
				plinkoStart = FALSE;

				SetTimer(hWnd, PlinkoTimer, 1, (TIMERPROC)TimerProc);
			}

			plinkoInit(hWnd);

			MoneyBoxDraw(mDC);
			plinkoDraw(mDC);
			rocksDraw(mDC, g_hInst);

			oldFont = (HFONT)SelectObject(mDC, hFont);
			SetBkMode(mDC, TRANSPARENT); // 글자 배경 투명


			// 플링코 화면 진입 버튼
			// Plinko
			SelectObject(imgDC, imgBitmap[15]);
			TransparentBlt(mDC, rectView.left + 5, rectView.top + 5, 100, 40, imgDC, 0, 0, 400, 200, RGB(255, 255, 255));

			// 강화 화면 진입 버튼
			// Enforce
			SelectObject(imgDC, imgBitmap[13]);
			TransparentBlt(mDC, rectView.left + 115, rectView.top + 5, 100, 40, imgDC, 0, 0, 400, 200, RGB(255, 255, 255));

			// 설정 화면 진입 버튼
			// Setting
			SelectObject(imgDC, imgBitmap[17]);
			TransparentBlt(mDC, rectView.left + 225, rectView.top + 5, 100, 40, imgDC, 0, 0, 400, 200, RGB(255, 255, 255));
		}

		SelectObject(mDC, oldBrush);
		SelectObject(mDC, oldFont);
		SelectObject(mDC, oldPen);

		// 사용한 DC 반환
		DeleteDC(imgDC);

		// 더블 버퍼링
		BitBlt(hDC, 0, 0, rectView.right, rectView.bottom, mDC, 0, 0, SRCCOPY);

		SelectObject(mDC, oldHBitmap);

		// 사용한 DC 반환
		DeleteDC(mDC);
		DeleteObject(hBitmap);

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_TIMER:
		switch (wParam) {
		case 1:
			break;
		}

		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_DESTROY:
		for (int i = 1; i <= timercnt; i++) {
			KillTimer(hWnd, i);
		}
		KillTimer(hWnd, GoMove);
		KillTimer(hWnd, GoAttack);
		KillTimer(hWnd, GoConsumeFual);
		KillTimer(hWnd, SetAttackType);
		KillTimer(hWnd, GoRarityShuffle);

		Stop_BGM();
		Quit_SoundAll();
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

//----------------------------------------------------------------------------
void CALLBACK TimerProc(HWND hWnd, UINT iMsg, UINT idEvent, DWORD dwTime) {
	//----------------------------------------------------------------------------
	HDC hDC;
	HBRUSH MyBrush, OldBrush;
	RECT rect;
	hDC = GetDC(hWnd);
	GetClientRect(hWnd, &rect);

	//--------------------------------------------------------------------
	//  이동 및 재생성 관련 타이머
	//--------------------------------------------------------------------
	if (idEvent == GoMove) {
		if (player.GetHP() <= 0) {
			GameOver(hWnd);
			return;
		}

		CheckBossMerge();

		player.Move(xPos, yPos);

		for (int i = 0; i < gMap.GetMaximumRock(); ++i) {
			player.SetLength(rock[i]);

			rock[i].Move(width, height);
		}

		for (int i = 0; i < MAX_BULLETS; ++i) {
			if (bullet[i].GetIsActive()) {

				for (int j = 0; j < MAX_ROCKS; ++j) {
					bullet[i].Crash(rock[j]);
				}

				bullet[i].Move(width, height);

			}
		}

		for (int i = 0; i < MAX_FEED; ++i) {
			if (feed[i].GetActive()) {
				feed[i].Move(width, height);
				feed[i].SetLength(player);
			}
		}

		// 플레이어 총알 - 보스 타격 체크 (SetActive 후 GetAttackPower 호출 버그 수정)
		for (int i = 0; i < MAX_BULLETS; ++i) {
			if (bullet[i].GetIsActive()) {
				if (boss.IsActive()) {
					double dx = boss.GetX() - bullet[i].GetX();
					double dy = boss.GetY() - bullet[i].GetY();
					if (sqrt(dx * dx + dy * dy) < boss.GetSize() + 10) {
						double dmg = bullet[i].GetAttackPower();
						bullet[i].SetActive(false);
						bool bossDead = boss.TakeDamage(dmg);
						if (bossDead) {
							KillTimer(hWnd, GoBossAttack);
						}
					}
				}
				if (bullet[i].GetIsActive()) {
					bullet[i].Move(width, height);
				}
			}
		}

		// 보스 이동 / 탄막 이동 / 플레이어 피격 판정 - 16ms GoMove 안에서 처리
		// GoBossAttack 타이머는 탄막 발사 패턴만 담당
		if (boss.IsActive()) {
			boss.Move(player.GetX(), player.GetY());
			boss.MoveBullets(width, height);
			boss.CheckBulletCollision(player);
		}
	}
	//--------------------------------------------------------------------
	//  공격 관련 타이머
	//--------------------------------------------------------------------
	else if (idEvent == GoAttack) {

		if (player.GetAttackTypeChanged()) {
			SetTimer(hWnd, GoAttack, player.GetAttackSpeed(), (TIMERPROC)TimerProc);
			SetTimer(hWnd, SetAttackType, 16, (TIMERPROC)TimerProc);
			player.SetAttackTypeChanged(false);
		}

		int targetRockID = player.GetMinLengthID();

		if (targetRockID != -1) {

			for (int i = 0; i < MAX_BULLETS; i++) {
				if (!bullet[i].GetIsActive()) {

					bullet[i].Spawn(player, rock[targetRockID]);

					break;
				}
			}
		}

		player.HealPlayer(attack_heal / 10);
	}
	//--------------------------------------------------------------------
	//  연료 소비 관련 타이머
	//--------------------------------------------------------------------
	else if (idEvent == GoConsumeFual) {
		player.ConsumeFual();
	}
	//--------------------------------------------------------------------
	// 무기 상자 교체 타이머
	//--------------------------------------------------------------------
	else if (idEvent == SetAttackType) {
		isAttackTypeChanged--;
		if (isAttackTypeChanged == 0) {
			KillTimer(hWnd, SetAttackType);
			player.SetAttackType(0);
			isAttackTypeChanged = MAX_AttackTypeChanged;
		}
	}
	//--------------------------------------------------------------------
	// 레어도 셔플 타이머
	//--------------------------------------------------------------------
	else if (idEvent == GoRarityShuffle) {
		if (!gMap.UpdateRarityShuffle()) {
			// 셔플이 완전히 끝나면 (false를 반환하면) 타이머를 끕니다.
			KillTimer(hWnd, GoRarityShuffle);
			SetTimer(hWnd, GoMove, 16, (TIMERPROC)TimerProc);
			SetTimer(hWnd, GoAttack, player.GetAttackSpeed(), (TIMERPROC)TimerProc);
			SetTimer(hWnd, GoConsumeFual, 1000, (TIMERPROC)TimerProc);
		}
		// 100ms마다 화면을 무효화하여 새로운 레어도 텍스트를 그리게 함
	}
	else if (idEvent == PlinkoTimer) {
		pinCollisionCheck();	// pin - rock
		rockCollisionCheck();	// rock - rock
		rockUpdate();
		checkGoal();
		if (pTimerCheck() == true) {
			KillTimer(hWnd, PlinkoTimer);
		}

	}

	// 돌 애니매이션
	else if (idEvent == StoneTimer) {
		for (int i = 0; i < MAX_ROCKS; i++) {
			rock[i].SetRockMotion(1);
		}
	}
	else if (idEvent == GoBossAttack) {
		// 탄막 발사 패턴만 담당 (800ms 간격 유지)
		// 이동/탄막이동/피격판정은 GoMove(16ms)에서 처리
		if (boss.IsActive()) {
			boss.FireDanmaku(player);
		}
	}

	ReleaseDC(hWnd, hDC);
	InvalidateRect(hWnd, NULL, false);
}

void GameStart(HWND hWnd, RECT& rectView, int mx, int my, int& window_scene)
{
	if (!isGaming) {
		window_scene = 2;

		// 스테이지 초기화
		gMap.SetStage(1);

		// 화면 중앙 고정
		SetCursorPos(width / 2, height / 2);
		player.SetAttackTypeChanged(false);
		// 플레이어 태어남
		player.Spawn();

		// 다음 스테이지로 가는데 필요한 돌 계수 초기화
		gMap.SetRockNum();

		isGaming = true;

		for (int i = 0; i < MAX_ROCKS; ++i) {
			rock[i].SetActive(false);
		}

		for (int i = 0; i < gMap.GetMaximumRock(); ++i) {
			rock[i].Spawn();
		}

		for (int i = 0; i < MAX_BULLETS; ++i) {
			bullet[i].SetActive(false);
		}

		for (int i = 0; i < MAX_FEED; ++i) {
			feed[i].SetActive(false);
		}

		SetTimer(hWnd, GoMove, 16, (TIMERPROC)TimerProc);
		SetTimer(hWnd, GoAttack, player.GetAttackSpeed(), (TIMERPROC)TimerProc);
		SetTimer(hWnd, GoConsumeFual, 1000, (TIMERPROC)TimerProc);
		SetTimer(hWnd, GoBossAttack, 800, (TIMERPROC)TimerProc); // 0.8초마다 탄막 발사

		boss.SetActive(false); // 재시작 시 보스 초기화

		Stop_BGM();
		Play_Sound_BGM(L"BGM_Fight");
	}
}

void GameOver(HWND hWnd)
{
	KillTimer(hWnd, GoMove);
	KillTimer(hWnd, GoAttack);
	KillTimer(hWnd, GoConsumeFual);
	KillTimer(hWnd, SetAttackType);
	KillTimer(hWnd, GoRarityShuffle);
	KillTimer(hWnd, GoBossAttack);

	isGaming = false;
	player.SetAttackType(0);
	InvalidateRect(hWnd, NULL, false);
}

void DrawFight(HDC mDC, HWND hWnd, RECT rectView, HBRUSH hBrush[], HFONT hFont)
{
	// 총알 Draw
	for (int i = 0; i < MAX_BULLETS; ++i) {
		if (bullet[i].GetIsActive()) {
			bullet[i].Draw(mDC, player.GetAttackType(), g_hInst);
		}
	}

	if (boss.IsActive()) {
		boss.Draw(mDC);
		boss.DrawBullets(mDC);
	}

	// 운석 Draw
	for (int i = 0; i < MAX_ROCKS; ++i) {
		if (rock[i].GetActive()) {
			rock[i].Draw(mDC, g_hInst);
		}
	}

	// Player
	player.Draw(mDC, rectView, hBrush, hFont);

	// 스테이지 그리기
	gMap.SetMGoal(gMap.GetStage() * 3);
	gMap.Draw(mDC, rectView, hBrush, hFont);

	for (int i = 0; i < MAX_FEED; ++i) {
		if (feed[i].GetActive()) {
			feed[i].Draw(mDC, g_hInst);
		}
	}

	if (gMap.isNextStage()) {
		gMap.DrawNextStage(mDC, rectView, hBrush, hFont);
	}
}

void GameSleep(HWND hWnd)
{
	KillTimer(hWnd, GoMove);
	KillTimer(hWnd, GoAttack);
	KillTimer(hWnd, GoConsumeFual);
}

void CheckBossMerge()
{
	// 2스테이지가 아니거나, 이미 보스가 살아있으면 검사 안 함
	if (gMap.GetStage() != 2 || boss.IsActive()) return;

	int mergeRadius = 150; // 융합 반경(픽셀)
	int mergeCondition = 2; // 5개가 모이면 융합

	for (int i = 0; i < MAX_ROCKS; i++) {
		if (!rock[i].GetActive()) continue;

		int nearbyCount = 0;
		double centerX = 0, centerY = 0;

		// 현재 운석(i) 기준으로 반경 내의 다른 운석 개수를 센다
		for (int j = 0; j < MAX_ROCKS; j++) {
			if (!rock[j].GetActive()) continue;

			double dx = rock[i].GetX() - rock[j].GetX();
			double dy = rock[i].GetY() - rock[j].GetY();
			double dist = sqrt(dx * dx + dy * dy);

			if (dist <= mergeRadius) {
				nearbyCount++;
				centerX += rock[j].GetX();
				centerY += rock[j].GetY();
			}
		}

		// 조건 만족 시 융합 시작!
		if (nearbyCount >= mergeCondition) {
			centerX /= nearbyCount; // 모여있는 놈들의 중심점
			centerY /= nearbyCount;

			// 모여있던 운석들은 제물로 바쳐짐 (파괴)
			for (int j = 0; j < MAX_ROCKS; j++) {
				if (!rock[j].GetActive()) continue;
				double dx = rock[i].GetX() - rock[j].GetX();
				double dy = rock[i].GetY() - rock[j].GetY();
				if (sqrt(dx * dx + dy * dy) <= mergeRadius) {
					rock[j].SetActive(false);
				}
			}

			// 중심점에 보스 스폰!
			boss.Spawn(centerX, centerY);
			return; // 한 번 스폰되면 루프 종료
		}
	}
}