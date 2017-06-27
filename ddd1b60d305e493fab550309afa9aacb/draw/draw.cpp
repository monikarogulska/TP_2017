#include <stdafx.h>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <queue>
#include <iostream>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

INT value=0;
INT pozycja_aktualna=500;
INT stop = 0;
INT nacisniecia = 0;
INT i = 0;

struct dane_winda {
	int pozycja_windy1 = 500;
	int pozycja_windy2 = 500;
	int kierunek = 0;
	int stop = 0;
};

dane_winda dane;
std::queue <dane_winda> kolejka;
dane_winda dane_aktualne;

#define TMR_1 1
#define ID_z1na2 2
#define ID_z1na3 3
#define ID_z2na1 4 
#define ID_z2na3 5
#define ID_z3na1 6
#define ID_z3na2 7

VOID DrawPeople(HDC hdc, int nacisniecia);

VOID Example_DrawRectangle(HDC hdc, int pozycja_windy1, int pozycja_windy2, int kierunek )
{
	Graphics graphics(hdc);
	
		if (stop == 1) pozycja_aktualna = pozycja_windy2;
		else pozycja_aktualna = pozycja_windy1 + value*kierunek; 

		if (pozycja_aktualna > pozycja_windy2 && kierunek == -1)	value++;

		if (pozycja_aktualna < pozycja_windy2 && kierunek == 1)		value++;

		// Create a Pen object.
		Pen blackPen(Color(255, 0, 0, 0), 3);
		// Create a Rect object.
		Rect rect(400, pozycja_aktualna, 400, 200);
		// Draw rect.
		graphics.DrawRectangle(&blackPen, rect);

		if (pozycja_aktualna == pozycja_windy2&&value*kierunek == pozycja_windy2 - pozycja_windy1) {
			value = 0;
			stop = 1;
			if (nacisniecia>0) nacisniecia--;
		}

}

VOID funkcja_kolejka(HDC hdc) {

	dane_aktualne = kolejka.front();
	Example_DrawRectangle(hdc, dane_aktualne.pozycja_windy1, dane_aktualne.pozycja_windy2, dane_aktualne.kierunek);

	if (stop == 1 && kolejka.size() != 1) {
		
		kolejka.pop();
		stop = 0;
		i++;

		if (i == 2) {
			nacisniecia++;
			i = 0;
			Sleep(500);
		}
		else if(nacisniecia>0) nacisniecia--;
		
	}
}

VOID dodawanie_do_kolejki( int pozycja_windy1, int pozycja_windy2, int kierunek, int stop) {

	dane.pozycja_windy1 = dane.pozycja_windy2;
	dane.pozycja_windy2 = pozycja_windy1;

	if (dane.pozycja_windy1 - dane.pozycja_windy2 > 0) dane.kierunek = -1;
	else dane.kierunek = 1;

	dane.stop = 0;
	kolejka.push(dane);

	dane.pozycja_windy1 = pozycja_windy1;
	dane.pozycja_windy2 = pozycja_windy2;
	dane.kierunek = kierunek;
	dane.stop = stop;

	kolejka.push(dane);
}
	

VOID Example_DrawLine(HDC hdc)
{
	Graphics graphics(hdc);

	// Create a Pen object.
	Pen blackPen(Color(255, 0, 0, 255), 3);

	// Create two Point objects that define the line.
	Point point1(100, 300);
	Point point2(400, 300);
	Point point3(100, 500);
	Point point4(400, 500); 
	Point point5(100, 700);
	Point point6(400, 700);

	// Draw the line.
	graphics.DrawLine(&blackPen, point1, point2);
	graphics.DrawLine(&blackPen, point3, point4);
	graphics.DrawLine(&blackPen, point5, point6);

}

VOID DrawPeople(HDC hdc, int nacisniecia) {
	
	Graphics graphics(hdc);

if (nacisniecia<=9) {
		while (nacisniecia != 0) {
		
			Pen bluePen(Color(255, 0, 0, 0),2);
			Rect ellipseRect(790-nacisniecia*40, pozycja_aktualna+50, 35, 40);
			Pen blackPen(Color(255, 0, 0, 0), 3);
			Point point1(805 - nacisniecia * 40, pozycja_aktualna + 90);
			Point point2(805 - nacisniecia * 40, pozycja_aktualna + 150);
			Point point3(805 - nacisniecia * 40, pozycja_aktualna + 150);
			Point point4(790 - nacisniecia * 40, pozycja_aktualna + 200);
			Point point5(805 - nacisniecia * 40, pozycja_aktualna + 150);
			Point point6(820 - nacisniecia * 40, pozycja_aktualna + 200);
			Point point7(790 - nacisniecia * 40, pozycja_aktualna + 120);
			Point point8(820 - nacisniecia * 40, pozycja_aktualna + 120);
			graphics.DrawLine(&blackPen, point7, point8);
			graphics.DrawLine(&blackPen, point1, point2);
			graphics.DrawLine(&blackPen, point5, point6);
			graphics.DrawLine(&blackPen, point3, point4);
			graphics.DrawEllipse(&bluePen, ellipseRect);
			nacisniecia--;
		}
	}
   
}

int OnCreate(HWND window)
{
   SetTimer(window, TMR_1, 5, 0);
   return 0;
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC          hdc;
	PAINTSTRUCT  ps;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		Example_DrawLine(hdc);
		funkcja_kolejka(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_COMMAND:
		{

		hdc = BeginPaint(hWnd, &ps);
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
			
		case ID_z1na2:
		{
			dodawanie_do_kolejki( 500, 300, -1, 0);
		} break;

		case ID_z1na3:
		{
			dodawanie_do_kolejki(500, 100, -1, 0);
		} break;

		case ID_z2na1:
		{
			dodawanie_do_kolejki(300, 500, 1, 0);
		} break;

		case ID_z2na3:
		{
			dodawanie_do_kolejki(300, 100, -1, 0);
		} break;

		case ID_z3na1:
		{
			dodawanie_do_kolejki(100, 500, 1, 0);
		} break;

		case ID_z3na2:
		{
			dodawanie_do_kolejki(100, 300, 1, 0);
		}break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_TIMER:
		switch (wParam)
		{
		case TMR_1:
			//force window to repaint
			InvalidateRect(hWnd, NULL, TRUE);
			hdc = BeginPaint(hWnd, &ps);
			Example_DrawLine(hdc);
			funkcja_kolejka(hdc);
			DrawPeople(hdc, nacisniecia);
			EndPaint(hWnd, &ps);
			break;
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
} // WndProc

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
	HWND                hWnd;
	MSG                 msg;
	WNDCLASS            wndClass;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	
	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = TEXT("GettingStarted");

	RegisterClass(&wndClass);

	hWnd = CreateWindow(
		TEXT("GettingStarted"),   // window class name
		TEXT("Getting Started"),  // window caption
		WS_OVERLAPPEDWINDOW,      // window style
		CW_USEDEFAULT,            // initial x position
		CW_USEDEFAULT,            // initial y position
		900,					  // initial x size
		800,					  // initial y size
		NULL,                     // parent window handle
		NULL,                     // window menu handle
		hInstance,                // program instance handle
		NULL);                    // creation parameters
	
	HWND z1na2 = CreateWindowEx(NULL, L"BUTTON", L"2", WS_CHILD | WS_VISIBLE | WS_BORDER,
		100, 650, 50,50, hWnd, (HMENU)ID_z1na2, hInstance, NULL); 
	HWND z1na3 = CreateWindowEx(NULL, L"BUTTON", L"3", WS_CHILD | WS_VISIBLE | WS_BORDER,
		200, 650, 50, 50, hWnd, (HMENU)ID_z1na3, hInstance, NULL);
	HWND z2na1 = CreateWindowEx(NULL, L"BUTTON", L"1", WS_CHILD | WS_VISIBLE | WS_BORDER,
		100, 450, 50, 50, hWnd, (HMENU)ID_z2na1, hInstance, NULL);
	HWND z2na3 = CreateWindowEx(NULL, L"BUTTON", L"3", WS_CHILD | WS_VISIBLE | WS_BORDER,
		200, 450, 50, 50, hWnd, (HMENU)ID_z2na3, hInstance, NULL);
	HWND z3na1 = CreateWindowEx(NULL, L"BUTTON", L"1", WS_CHILD | WS_VISIBLE | WS_BORDER,
		100, 250, 50, 50, hWnd, (HMENU)ID_z3na1, hInstance, NULL); 			
	HWND z3na2 = CreateWindowEx(NULL, L"BUTTON", L"2", WS_CHILD | WS_VISIBLE | WS_BORDER,
		200, 250, 50, 50, hWnd, (HMENU)ID_z3na2, hInstance, NULL);

	kolejka.push(dane);

	OnCreate(hWnd);

	 if (!hWnd)
   {
      return FALSE;
   }

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(gdiplusToken);
	return msg.wParam;
}  // WinMain
