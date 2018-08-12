#include "stdafx.h"
#include "WinRaycaster.h"
#include "Input.h"

//// Global Variables:
#define MAX_LOADSTRING 100
#define SCRN_W 640
#define SCRN_H 480
HINSTANCE hInst;                                // current instance
HWND hWnd;

class Game {
public:
	Input input;
	Graphics *pGfx = nullptr;
	Camera	*pCam = nullptr;
	Map<4>	*pMap = nullptr;

	void InitGame() {
		InitMap();

		pCam = new Camera(SCRN_W, SCRN_H);
		pCam->Pos = Vec2(2.5f, 2.5f);
		pCam->Dir = -45.0f * Ang2Rad;
		pCam->FoV = 90.0f * Ang2Rad;

		input.Initialize(hInst, hWnd, SCRN_W, SCRN_H);

	}

	void InitMap()
	{
		pMap = new Map<4>(480);

		const Pixel R(0xFFFF0000);
		const Pixel G(0xFF00FF00);
		const Pixel B(0xFF00007F);

		pMap->Cell(0, 0) = R;

		pMap->Cell(1, 0) = G;
		pMap->Cell(2, 0) = G;
		pMap->Cell(3, 0) = G;

		pMap->Cell(0, 1) = B;
		pMap->Cell(0, 2) = B;
		pMap->Cell(0, 3) = B;

		pMap->Cell(3, 3) = R;
	}


	int Tick(float deltaTime, HWND hWnd) {
		pMap->Scan(*pCam);
		pCam->Dir += 10.0f * Ang2Rad * deltaTime;
		pCam->Pos = Vec2(2, 2) + Vec2(sinf(pCam->Dir), cosf(pCam->Dir) * 0.75f);
		PostMessage(hWnd, WM_PAINT, NULL, NULL);

		OutputDebugStringA("Tick!\n");

		return 0;
	}

	int Render() {
		pCam->Display(pGfx);
		return 0;
	}

};

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
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
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
	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	game.pGfx = new Graphics(hWnd);

	SetTimer(hWnd, 123, 1000 / 60, NULL);

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
	case WM_PAINT:
	{
		// RENDERING CODE
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...

		if (game.pCam)
			game.Render();

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_TIMER:
		switch (wParam) {
		case 123:
			// TICK CODE

			GetSystemTime(&time);
			double deltaTime = min(.05f, abs((time.wMilliseconds - timeLastFrame.wMilliseconds) / 1000.f));
			timeLastFrame = time;

			return game.Tick(deltaTime, hWnd);

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

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WINRAYCASTER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	game.InitGame();

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINRAYCASTER));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}

#pragma endregion