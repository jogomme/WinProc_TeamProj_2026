#include<Windows.h>
#include<windowsx.h>
#include<time.h>
#include<iostream>
#include<string.h>
#include<math.h>
#include<queue>
#pragma comment (lib, "msimg32.lib")

#include "resource.h"
#include "GameObject.h"
#include "Player.h"
#include "Rock.h"

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC, mDC, imgDC; // hDC - 최종 출력 화면 / mDC - 더블 퍼버링용. 대부분의 그림 출력은 여기에 / imgDC - 그림 선택할 때 사용.
	HBITMAP hBitmap; // 비트맵
	static HBITMAP imgBitmap[15]; // 이미지 로딩
	static BITMAP imgBmp; // 이미지 크기 잡는용, 배경 잡는용으로 썼는데, 배열로 바꿔서 써도 될듯
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	static POINT point[10]; // Polygon 함수를 위한 변수. 그때그때 좌표 적어서 활용

	static RECT rectView; // "보이는" 화면 크기
	static POINT rectViewMid; // "보이는" 화면 중앙
	static SIZE size; // 화면 사이즈 변경시. WM_SIZE
	static int mx, my; // 마우스 클릭 좌표
	static int timercnt; // 타이머 갯수, 종료 및 초기화 등에서 KillTimer 함수를 위한 갯수를 저장. WM_CREATE 에서 갯수 저장할 것

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

		timercnt = 0;

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

	case WM_PAINT:
		// 기초 로드
		hDC = BeginPaint(hWnd, &ps);
		mDC = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, rectView.right, rectView.bottom);
		SelectObject(mDC, (HBITMAP)hBitmap);
		Rectangle(mDC, 0, 0, rectView.right, rectView.bottom);
		imgDC = CreateCompatibleDC(mDC);

		// 필요 내용 작성은 이 밑으로.

		/* // 필요한 경우 사용
		hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
		oldPen = (HPEN)SelectObject(mDC, hPen);
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
		SelectObject(mDC, oldBrush);
		DeleteObject(hBrush);
		SelectObject(mDC, oldPen);
		DeleteObject(hPen);
		*/

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