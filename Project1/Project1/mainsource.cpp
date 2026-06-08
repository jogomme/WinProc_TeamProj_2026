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

void Draw(HDC mDC, HWND hWnd, RECT rectView, HBRUSH hBrush[], HFONT hFont);

//-----------------------------------------------------------------------------------------------
// 전역 변수 선언 구간
//-----------------------------------------------------------------------------------------------

// 콘솔 창 띄우는 용도입니다. Debug(디버그) 용도입니다.
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

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

// 현재 마우스 커서의 위치
int xPos{};
int yPos{};

// 현재 최대 총알 갯수
int BulletCnt{};

// 스테이지 넘어가기 까지 눌러야하는 스페이스바 꾹 수
const int isSpaceBarDown = 85;

// 지금 얼마나 눌렀는지
int isDowning = 0;

// 게임 상태 변수
bool isGaming = false;

// 캐릭터들 상태 타이머 변수
const int GoMove{ 1 };
const int GoAttack{ 2 };
const int GoConsumeFual{ 3 };
const int GoShow{ -1 };

// 현재 어느 화면을 띄울 것인가 // 0 - 메인 화면, 1 - 플레이어 강화 창, 2 - 전투 화면, 3 - 설정 창, 4 - 플링코 화면
int window_scene{0};

//-----------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
//-----------------------------------------------------------------------------------------------
{
	PAINTSTRUCT ps;
	HDC hDC, mDC, imgDC; // hDC - 최종 출력 화면 / mDC - 더블 퍼버링용. 대부분의 그림 출력은 여기에 / imgDC - 그림 선택할 때 사용.
	HBITMAP hBitmap; // 비트맵
	static HBITMAP imgBitmap[15]; // 이미지 로딩
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

	switch (iMessage) {
	case WM_CREATE:
		srand(time(NULL));
		/*
		imgBitmap[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		imgBitmap[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		imgBitmap[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
		GetObject(imgBitmap[0], sizeof(BITMAP), &imgBmp);
		*/

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

		Open_Sound();

		Play_Sound_BGM(L"BGM_Lobby");

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
		else if (wParam == VK_SPACE) {
			if (gMap.isfull()) {
				isDowning++;
				if (isDowning >= isSpaceBarDown) {
					gMap.NextStage();
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
			if (mx > rectViewMid.x - 100 && mx< rectViewMid.x + 100 &&
				my>rectViewMid.y - 20 && my < rectViewMid.y + 20) {
				GameStart(hWnd, rectView, mx, my, window_scene);
			}
			else if (mx > rectViewMid.x - 100 && mx < rectViewMid.x + 100 &&
				my>rectViewMid.y + 30 && my < rectViewMid.y + 70) {

				return_setting = window_scene;
				window_scene = 3;
			}
		}

		// 강화 화면
		else if (window_scene == 1) {

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

			// 강화 버튼 클릭
			int ck = 0;
			for (int i = 0; i < MAX_ENFORCE; i++) {
				if (mx > enforce[i].Get_Enforce_Point_x() - enforce_size && mx < enforce[i].Get_Enforce_Point_x() + enforce_size &&
					my > enforce[i].Get_Enforce_Point_y() - enforce_size && my < enforce[i].Get_Enforce_Point_y() + enforce_size &&
					enforce[i].Get_Enforce_Draw() == 1 && enforce[i].Get_Enforce_Open()==0) {
					// 가격 관련 조건, 마이너스 필요
					if (false) {
						Play_Sound(L"EFFECT_FEnforce");
					}
					else {
						Play_Sound(L"EFFECT_Enforce");
						ck = 1;
						enforce[i].Set_Open(1);
						enforce[enforce[i].Get_Enforce_Drawing(0)].Set_Draw(1);
						enforce[enforce[i].Get_Enforce_Drawing(1)].Set_Draw(1);
						enforce[enforce[i].Get_Enforce_Drawing(2)].Set_Draw(1);

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
				if ((mx > rectViewMid.x - 100 && mx < rectViewMid.x + 100 &&
					my > rectViewMid.y - 30 && my < rectViewMid.y + 30)) {
					window_scene = 1;
					Stop_BGM();
					Play_Sound_BGM(L"BGM_Lobby");
				}
			}
		}
		// 설정 화면
		else if (window_scene == 3) {
			if (mx > rectViewMid.x - 100 && mx < rectViewMid.x + 100 &&
				my > rectViewMid.y + 80 && my < rectViewMid.y + 120) {
				window_scene = return_setting;
			}
		}
		// 플링코 화면
		else if (window_scene == 4) {

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

	case WM_PAINT:
		// 기초 로드
		hDC = BeginPaint(hWnd, &ps);
		mDC = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, rectView.right, rectView.bottom);
		SelectObject(mDC, (HBITMAP)hBitmap);
		Rectangle(mDC, 0, 0, rectView.right, rectView.bottom);
		imgDC = CreateCompatibleDC(mDC);

		// 필요 내용 작성은 이 밑으로.

		oldPen = (HPEN)SelectObject(mDC, hPen[0]);
		// 게임 시작 화면
		if (window_scene == 0) {
			oldBrush = (HBRUSH)SelectObject(mDC, hBrush[1]);
			//시작 버튼
			Rectangle(mDC, rectViewMid.x - 100, rectViewMid.y - 20, rectViewMid.x + 100, rectViewMid.y + 20);
			
			oldFont = (HFONT)SelectObject(mDC, hFont);
			SetBkMode(mDC, TRANSPARENT); // 글자 배경 투명
			wchar_t str[64];
			wsprintf(str, L"Game Start"); // 추후 이미지 버튼 등으로 변경 예정
			TextOut(mDC, rectViewMid.x - 50, rectViewMid.y - 10, str, lstrlen(str));

			oldBrush = (HBRUSH)SelectObject(mDC, hBrush[4]);
			//설정 버튼
			Rectangle(mDC, rectViewMid.x - 100, rectViewMid.y + 30, rectViewMid.x + 100, rectViewMid.y + 70);
			wsprintf(str, L"Setting"); // 추후 이미지 버튼 등으로 변경 예정
			TextOut(mDC, rectViewMid.x - 40, rectViewMid.y + 40, str, lstrlen(str));
		}
		// 강화 화면
		else if (window_scene == 1) {
			oldBrush = (HBRUSH)SelectObject(mDC, hBrush[2]);

			for (int i = 0; i < MAX_ENFORCE; i++) {
				// 블럭 오픈 여부에 따른 색
				oldBrush = (HBRUSH)SelectObject(mDC, hBrush[8 - enforce[i].Get_Enforce_Open()]);
				// 블럭 그리기
				if (enforce[i].Get_Enforce_Draw() == 1) {
					// 타입에 따른 이미지
					double x = enforce[i].Get_Enforce_Point_x();
					double y = enforce[i].Get_Enforce_Point_y();
					Rectangle(mDC, x - enforce_size, y - enforce_size, x + enforce_size, y + enforce_size);

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
					else {
						wsprintf(str, L"미구현");
					}
					TextOut(mDC, x - 25, y + 20, str, lstrlen(str));

					double amount = enforce[i].Get_Enforce_Amount();
					wsprintf(str, L"%d", (int)amount);
					TextOut(mDC, x-25, y+40, str, lstrlen(str));
				}
			}

			// 전투 화면 진입 버튼
			oldBrush = (HBRUSH)SelectObject(mDC, hBrush[1]);
			Rectangle(mDC, rectView.left + 5, rectView.bottom - 85, rectView.left + 205, rectView.bottom - 5);

			oldFont = (HFONT)SelectObject(mDC, hFont);
			SetBkMode(mDC, TRANSPARENT); // 글자 배경 투명

			wchar_t str[64];
			wsprintf(str, L"Go Fight"); // 추후 이미지 버튼 등으로 변경 예정
			TextOut(mDC, rectView.left + 65, rectView.bottom - 55, str, lstrlen(str));

			// 플링코 화면 진입 버튼
			oldBrush = (HBRUSH)SelectObject(mDC, hBrush[6]);
			Rectangle(mDC, rectView.left + 5, rectView.top + 5, rectView.left + 105, rectView.top +45);

			wsprintf(str, L"Plinko"); // 추후 이미지 버튼 등으로 변경 예정
			TextOut(mDC, rectView.left + 25, rectView.top + 15, str, lstrlen(str));

			// 강화 화면 진입 버튼
			oldBrush = (HBRUSH)SelectObject(mDC, hBrush[2]);
			Rectangle(mDC, rectView.left + 115, rectView.top + 5, rectView.left + 215, rectView.top + 45);

			wsprintf(str, L"Enforce"); // 추후 이미지 버튼 등으로 변경 예정
			TextOut(mDC, rectView.left + 135, rectView.top + 15, str, lstrlen(str));

			// 설정 화면 진입 버튼
			oldBrush = (HBRUSH)SelectObject(mDC, hBrush[4]);
			Rectangle(mDC, rectView.left + 225, rectView.top + 5, rectView.left + 325, rectView.top + 45);

			wsprintf(str, L"Setting"); // 추후 이미지 버튼 등으로 변경 예정
			TextOut(mDC, rectView.left + 245, rectView.top + 15, str, lstrlen(str));
		}
		// 전투 화면
		else if (window_scene == 2) {
			
			Draw(mDC, hWnd, rectView, hBrush, hFont);

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
					oldBrush = (HBRUSH)SelectObject(mDC, hBrush[2]);
					Rectangle(mDC, rectViewMid.x - 100, rectViewMid.y - 30, rectViewMid.x + 100, rectViewMid.y + 30);

					oldFont = (HFONT)SelectObject(mDC, hFont);
					SetBkMode(mDC, TRANSPARENT); // 글자 배경 투명
					wchar_t str[64];
					wsprintf(str, L"Game Over");
					TextOut(mDC, rectViewMid.x - 60, rectViewMid.y - 60, str, lstrlen(str));

					wsprintf(str, L"Go Enforce Page");
					TextOut(mDC, rectViewMid.x - 80, rectViewMid.y - 10, str, lstrlen(str));
				}
			}
		}
		// 설정 화면
		else if (window_scene == 3) {
			oldBrush = (HBRUSH)SelectObject(mDC, hBrush[2]);
			Rectangle(mDC, rectViewMid.x - 100, rectViewMid.y + 80, rectViewMid.x + 100, rectViewMid.y + 120);

			oldFont = (HFONT)SelectObject(mDC, hFont);
			SetBkMode(mDC, TRANSPARENT); // 글자 배경 투명

			wchar_t str[64];
			wsprintf(str, L"Go Back"); // 추후 이미지 버튼 등으로 변경 예정
			TextOut(mDC, rectViewMid.x - 50, rectViewMid.y + 90, str, lstrlen(str));
		}

		else if (window_scene == 4) {
			plinkoInit(hWnd);
			plinkoDraw(mDC);

			oldFont = (HFONT)SelectObject(mDC, hFont);
			SetBkMode(mDC, TRANSPARENT); // 글자 배경 투명

			wchar_t str[64];
			// 플링코 화면 진입 버튼
			oldBrush = (HBRUSH)SelectObject(mDC, hBrush[6]);
			Rectangle(mDC, rectView.left + 5, rectView.top + 5, rectView.left + 105, rectView.top + 45);

			wsprintf(str, L"Plinko"); // 추후 이미지 버튼 등으로 변경 예정
			TextOut(mDC, rectView.left + 25, rectView.top + 15, str, lstrlen(str));

			// 강화 화면 진입 버튼
			oldBrush = (HBRUSH)SelectObject(mDC, hBrush[2]);
			Rectangle(mDC, rectView.left + 115, rectView.top + 5, rectView.left + 215, rectView.top + 45);

			wsprintf(str, L"Enforce"); // 추후 이미지 버튼 등으로 변경 예정
			TextOut(mDC, rectView.left + 135, rectView.top + 15, str, lstrlen(str));

			// 설정 화면 진입 버튼
			oldBrush = (HBRUSH)SelectObject(mDC, hBrush[4]);
			Rectangle(mDC, rectView.left + 225, rectView.top + 5, rectView.left + 325, rectView.top + 45);

			wsprintf(str, L"Setting"); // 추후 이미지 버튼 등으로 변경 예정
			TextOut(mDC, rectView.left + 245, rectView.top + 15, str, lstrlen(str));
		}

		SelectObject(mDC, oldBrush);
		SelectObject(mDC, oldFont);
		SelectObject(mDC, oldPen);

		// 사용한 DC 반환
		DeleteDC(imgDC);

		// 더블 버퍼링
		BitBlt(hDC, 0, 0, rectView.right, rectView.bottom, mDC, 0, 0, SRCCOPY);

		// 사용한 DC 반환
		DeleteDC(mDC);
		DeleteObject(hBitmap);

		EndPaint(hWnd, &ps);
		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			//player.Move(xPos, yPos);
			break;
		}

		InvalidateRect(hWnd, NULL, false);
		break;
	
	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		}

		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_DESTROY:
		for (int i = 1; i <= timercnt; i++) {
			KillTimer(hWnd, i);
		}
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
	}
	//--------------------------------------------------------------------
	//  공격 관련 타이머
	//--------------------------------------------------------------------
	else if (idEvent == GoAttack) {

		int targetRockID = player.GetMinLengthID();

		if (targetRockID != -1) {

			for (int i = 0; i < MAX_BULLETS; i++) {
				if (!bullet[i].GetIsActive()) {

					bullet[i].Spawn(player, rock[targetRockID]);

					break; 
				}
			}
		}
	}
	//--------------------------------------------------------------------
	//  연료 소비 관련 타이머
	//--------------------------------------------------------------------
	else if (idEvent == GoConsumeFual) {
		player.ConsumeFual();
	}

	ReleaseDC(hWnd, hDC);
	InvalidateRect(hWnd, NULL, false);
}

int Enforce_Point_Calc(Point rectViewMid, char c, int xy, int intrv)
{
	if (c == 'x')
		return rectViewMid.x + xy * intrv;
	else
		return rectViewMid.y + xy * intrv;
}

void GameStart(HWND hWnd, RECT& rectView, int mx, int my, int& window_scene)
{
	if (!isGaming) {
		window_scene = 2;

		// 스테이지 초기화
		gMap.SetStage(1);

		// 화면 중앙 고정
		SetCursorPos(width / 2, height / 2);

		// 플레이어 태어남
		player.Spawn();

		// 다음 스테이지로 가는데 필요한 돌 계수 초기화
		gMap.SetRockNum();

		isGaming = true;

		for (int i = 0; i < gMap.GetMaximumRock(); ++i) {
			rock[i].Spawn();
		}

		for (int i = 0; i < MAX_BULLETS; ++i) {
			bullet[i].SetActive(false); 
		}

		SetTimer(hWnd, GoMove, 16, (TIMERPROC)TimerProc);
		SetTimer(hWnd, GoAttack, player.GetAttackSpeed(), (TIMERPROC)TimerProc);
		SetTimer(hWnd, GoConsumeFual, 1000, (TIMERPROC)TimerProc);

		Stop_BGM();
		Play_Sound_BGM(L"BGM_Fight");
	}
}

void GameOver(HWND hWnd)
{
	KillTimer(hWnd, GoMove);
	KillTimer(hWnd, GoAttack);
	KillTimer(hWnd, GoConsumeFual);

	isGaming = false;

	InvalidateRect(hWnd, NULL, false);
}

void Draw(HDC mDC, HWND hWnd, RECT rectView, HBRUSH hBrush[], HFONT hFont)
{
	// 총알 Draw
	for (int i = 0; i < MAX_BULLETS; ++i) {
		if (bullet[i].GetIsActive()) {
			bullet[i].Draw(mDC);
		}
	}

	// 운석 Draw
	for (int i = 0; i < MAX_ROCKS; ++i) {
		if (rock[i].GetActive()) {
			rock[i].Draw(mDC);
		}
	}

	// Player
	player.Draw(mDC, rectView, hBrush, hFont);

	// 스테이지 그리기
	gMap.Draw(mDC, rectView, hBrush, hFont);

	for (int i = 0; i < MAX_FEED; ++i) {
		if (feed[i].GetActive()) {
			feed[i].Draw(mDC);
		}
	}
}