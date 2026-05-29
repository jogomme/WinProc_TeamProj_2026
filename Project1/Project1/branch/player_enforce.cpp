#include<Windows.h>
#include<windowsx.h>
#include<time.h>
#include<iostream>
#include<string.h>
#include<math.h>
#include<queue>
#pragma comment (lib, "msimg32.lib")

#include "resource.h"

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

int Enforce_Point_Calc(POINT rectViewMid, char c, int xy, int intrv)
{
	if (c == 'x')
		return rectViewMid.x + xy * intrv;
	else
		return rectViewMid.y + xy * intrv;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC, mDC, imgDC; // hDC - 최종 출력 화면 / mDC - 더블 퍼버링용. 대부분의 그림 출력은 여기에 / imgDC - 그림 선택할 때 사용.
	HBITMAP hBitmap; // 비트맵
	static HBITMAP imgBitmap[15]; // 이미지 로딩
	static BITMAP imgBmp; // 이미지 크기 잡는용, 배경 잡는용으로 썼는데, 배열로 바꿔서 써도 될듯
	static HPEN hPen[10], oldPen;
	static HBRUSH hBrush[10], oldBrush;
	static HFONT hFont, oldFont;

	static POINT point[10]; // Polygon 함수를 위한 변수. 그때그때 좌표 적어서 활용

	static RECT rectView; // "보이는" 화면 크기
	static POINT rectViewMid; // "보이는" 화면 중앙
	static SIZE size; // 화면 사이즈 변경시. WM_SIZE
	static int mx, my; // 마우스 클릭 좌표
	static int timercnt; // 타이머 갯수, 종료 및 초기화 등에서 KillTimer 함수를 위한 갯수를 저장. WM_CREATE 에서 갯수 저장할 것

	static int window_scene; // 현재 어느 화면을 띄울 것인가 // 0 - 메인 화면, 1 - 플레이어 강화 창
	
	// 강화 버튼
	struct Enforce {
		float x, y; // 버튼의 위치 배율 (최초 노드는 0,0)
		int type; // ex) 0-미사용 노드 1-공격, 2-방어, 3-체력, 4,5,6,7,,,-특수능력 등등
		int amount; // type 의 직접적인 수치 ex) 공격력 3 / 방어력 2 / 특수능력 1 해금 2 해금 등등;
		int draw; // 현재 이 버튼을 그려낼 것인지 ex) 근처의 노드를 열면 1을 주어 그리도록 // 3진 트리
		int open; // 현재 이 버튼이 강화가 되었는지
	};
	static Enforce enforce[20000];
	static int enforce_size; // 버튼의 반지름 크기
	static int enforce_intrv; // 버튼 사이 간격
	static int enforce_cnt; // 강화 버튼 갯수
	static int drag; // 마우스 드래그 중인지 체크
	static POINT drag_start; // 드래그 시작점 저장

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

		timercnt = 0;

		window_scene = 0;

		enforce_cnt = 5;
		enforce_size = 20;
		enforce_intrv = 80;
		// 사용법 // (화면 중앙 포인트, xy 좌표중 택1, xy 좌표의 배율[상대적 좌표], 버튼 사이 거리)
		// 예시작으로 5개 구현
		enforce[0].x = Enforce_Point_Calc(rectViewMid, 'x', 0, enforce_intrv);
		enforce[0].y = Enforce_Point_Calc(rectViewMid, 'y', 0, enforce_intrv);
		enforce[0].type = 0; enforce[0].amount = 1; enforce[0].draw = 1; enforce[0].open = 0;

		enforce[1].x = Enforce_Point_Calc(rectViewMid, 'x', 1, enforce_intrv);
		enforce[1].y = Enforce_Point_Calc(rectViewMid, 'y', 0, enforce_intrv);
		enforce[1].type = 0; enforce[1].amount = 1; enforce[1].draw = 0; enforce[1].open = 0;

		enforce[2].x = Enforce_Point_Calc(rectViewMid, 'x', 0, enforce_intrv);
		enforce[2].y = Enforce_Point_Calc(rectViewMid, 'y', 1, enforce_intrv);
		enforce[2].type = 0; enforce[1].amount = 1; enforce[1].draw = 0; enforce[1].open = 0;

		enforce[3].x = Enforce_Point_Calc(rectViewMid, 'x', -1, enforce_intrv);
		enforce[3].y = Enforce_Point_Calc(rectViewMid, 'y', 0, enforce_intrv);
		enforce[3].type = 0; enforce[1].amount = 1; enforce[1].draw = 0; enforce[1].open = 0;

		enforce[4].x = Enforce_Point_Calc(rectViewMid, 'x', 1, enforce_intrv);
		enforce[4].y = Enforce_Point_Calc(rectViewMid, 'y', -1, enforce_intrv);
		enforce[4].type = 0; enforce[1].amount = 1; enforce[1].draw = 0; enforce[1].open = 0;

		drag = 0;
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
			PostQuitMessage(0);
			return 0;
		}

		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		// 메인 화면에서, 시작 버튼 클릭.
		//* 현재는 강화 화면으로 이동하게 되어있음.
		if (window_scene == 0 && mx> rectViewMid.x - 100 && mx< rectViewMid.x + 100 && 
			my>rectViewMid.y-20 && my< rectViewMid.y + 20) {
			window_scene = 1;
		}

		else if (window_scene == 1) {
			int ck = 0;
			for (int i = 0; i < enforce_cnt; i++) {
				if (mx > enforce[i].x - enforce_size && mx < enforce[i].x + enforce_size &&
					my > enforce[i].y - enforce_size && my < enforce[i].y + enforce_size) {
					ck = 1;
					enforce[i].open = 1;
					enforce[i * 3 + 1].draw = 1;
					enforce[i * 3 + 2].draw = 1;
					enforce[i * 3 + 3].draw = 1;
				}
			}

			if (ck == 0) {
				drag = 1;
				drag_start.x = mx;
				drag_start.y = my;
			}
		}

		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_MOUSEMOVE:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		if (drag == 1) {
			for (int i = 0; i < enforce_cnt; i++) {
				enforce[i].x -= (drag_start.x - mx)/2;
				enforce[i].y -= (drag_start.y - my)/2;
			}
			drag_start.x = mx;
			drag_start.y = my;
		}

		InvalidateRect(hWnd, NULL, false);
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

		// 게임 시작 화면
		if (window_scene == 0) {
			oldPen = (HPEN)SelectObject(mDC, hPen[0]);
			oldBrush = (HBRUSH)SelectObject(mDC, hBrush[1]);
			//시작 버튼
			Rectangle(mDC, rectViewMid.x - 100, rectViewMid.y - 20, rectViewMid.x + 100, rectViewMid.y + 20);

			oldBrush = (HBRUSH)SelectObject(mDC, hBrush[4]);
			//설정 버튼
			Rectangle(mDC, rectViewMid.x - 100, rectViewMid.y + 30, rectViewMid.x + 100, rectViewMid.y + 70);

			SelectObject(mDC, oldBrush);
			SelectObject(mDC, oldPen);
		}
		// 강화 화면
		else if (window_scene == 1) {
			oldBrush = (HBRUSH)SelectObject(mDC, hBrush[2]);
			
			for (int i = 0; i < enforce_cnt; i++) {
				// 블럭 오픈 여부에 따른 색
				oldBrush = (HBRUSH)SelectObject(mDC, hBrush[8 - enforce[i].open]);
				// 블럭 그리기
				if (enforce[i].draw == 1)
					Rectangle(mDC, enforce[i].x - enforce_size, enforce[i].y - enforce_size, enforce[i].x + enforce_size, enforce[i].y + enforce_size);
			}
			
			SelectObject(mDC, oldBrush);
		}

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