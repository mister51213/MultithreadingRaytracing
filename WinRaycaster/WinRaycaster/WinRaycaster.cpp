#include "stdafx.h"
#include "WinRaycaster.h"
//#include "Input.h"

//// Global Variables:
#define MAX_LOADSTRING 100

HINSTANCE hInst;                                // current instance
HWND hWnd;

#pragma region WINDOWS INTERNALS
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
SYSTEMTIME time;
SYSTEMTIME timeLastFrame;

Game game;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINRAYCASTER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);// (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINRAYCASTER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	//HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	//	CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	RECT rect;
	rect.left = rect.top = 0;
	rect.right = SCRN_W;
	rect.bottom = SCRN_H;
	
	//rect.right += DebugScl * 11; rect.bottom = max(rect.bottom, DebugScl * 11);

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, true);

	//hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
		return FALSE;
	
	//SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)GetStockObject(BLACK_BRUSH));

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	game.pGfx = new Graphics(hWnd);

	//SetTimer(hWnd, 123, 1000 / FPS, NULL);
	//PostMessage(hWnd, WM_PAINT, NULL, NULL);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	//case WM_ERASEBKGND:
	//	return TRUE;

	case WM_PAINT:
	{
		// RENDERING CODE
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...

		/*RECT rect;
		rect.left = rect.top = 0;
		rect.right = SCRN_W;
		rect.bottom = SCRN_H;
		FillRect(hdc, &rect &ps.rcPaint, (HBRUSH)GetStockObject(BLACK_BRUSH));*/
/*
		static uint64 LastTime = 0;
		uint64 ThisTime;
		QueryUnbiasedInterruptTime(&ThisTime);

		if (!LastTime) {
			LastTime = ThisTime;
			break;
		}

		float DeltaTime = (float)(ThisTime - LastTime) / 10e6f;
		LastTime = ThisTime;

		//LockWindowUpdate(hWnd);
		//LockWindowUpdate(NULL);

		game.Tick(DeltaTime, hWnd);
*/
		if (game.pCam)
			game.Render();

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_TIMER:
		switch (wParam) {
		case 123:
			// TICK CODE

			//GetSystemTime(&time);
			//float deltaTime = min(.05f, abs((time.wMilliseconds - timeLastFrame.wMilliseconds) / 1000.f));
			//timeLastFrame = time;
/*
			static uint64 LastTime = 0;
			uint64 ThisTime;
			QueryUnbiasedInterruptTime(&ThisTime);

			if (!LastTime) {
				LastTime = ThisTime;
				break;
			}

			float DeltaTime = (float)(ThisTime - LastTime) / 10e6f;
			LastTime = ThisTime;
			
			game.Tick(DeltaTime, hWnd);
*/
			PostMessage(hWnd, WM_PAINT, NULL, NULL);
			//UpdateWindow(hWnd);
			
			//pMap->Scan(*pCam);
			//pCam->Dir += 1.0f * Ang2Rad;
			//pCam->Pos = Vec2(2, 2) + Vec2(sinf(pCam->Dir), cosf(pCam->Dir) * 0.75f);
			//PostMessage(hWnd, WM_PAINT, NULL, NULL);

			//OutputDebugStringA("Tick!\n");

			//return 0; 
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


typedef TempVect2D<Fixed16> Fix2;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	Quad Root(2);	// n=2, 2^2 = 4x4
	Root.Subdivide();
	Root[qTL]->Subdivide();

	Quad *pQuad = Root.FindCell(Int2(3, 3));


	DebugX   = SCRN_W;//640;
	DebugY   = 0;
	DebugScl = 80;

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WINRAYCASTER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	//timeBeginPeriod(1);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	game.InitGame();

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINRAYCASTER));

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// Main message loop:
	//while (GetMessage(&msg, nullptr, 0, 0))	{
	while (msg.message != WM_QUIT) {
		//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			continue;
		}

		static uint64 LastFPS = 0, Frames = 0;
		static uint64 LastTime = 0, ThisTime;
		QueryUnbiasedInterruptTime(&ThisTime);

		if (!LastTime) {
			LastTime = LastFPS = ThisTime;
			continue;
		}

		float DeltaTime = (float)(ThisTime - LastTime) / 10e6f;

		LastTime = ThisTime;

		//LockWindowUpdate(hWnd);
		//LockWindowUpdate(NULL);

		game.Tick(DeltaTime, hWnd);
		//PostMessage(hWnd, WM_PAINT, NULL, NULL);

		Frames++;
		QueryUnbiasedInterruptTime(&ThisTime);
		if (ThisTime - LastFPS > 10e6) {
			const double Time = (double)(ThisTime - LastFPS) / 10e6f;

			wchar_t str[128];
			swprintf_s(str, L"WinRaycaster - %.03f FPS", (double)Frames / Time);
			SetWindowText(hWnd, str);

			LastFPS = ThisTime; Frames = 0;
		}

		//OutputDebugStringA(".");
	}

	//timeEndPeriod(1);

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}

#pragma endregion