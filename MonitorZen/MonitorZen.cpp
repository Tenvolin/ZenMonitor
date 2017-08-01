// MonitorZen.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MonitorZen.h"
#include <string>
#include <iostream>
#include <set>
#include <unordered_map>
#include <Commctrl.h> // checked listbox
#include <vector>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// virtual-screen information
												// to find resolution
std::unordered_map<HMONITOR, MONITORINFO> monitorInfos; 
HWND hCheckBox;		// 

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
	LoadStringW(hInstance, IDC_MONITORZEN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	RegisterScreen(hInstance);

	// !!! Delete
	// Grab #monitors; initializes monitorHandles set.
	MonitorCount();

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MONITORZEN));

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

	return (int)msg.wParam;
}



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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MONITORZEN));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(255, 255, 255)); /*(HBRUSH)GetStockObject(COLOR_WINDOW + 1);*/
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MONITORZEN);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}
//
//  FUNCTION: RegisterScreen(HINSTANCE hInstance)
//
//  PURPOSE: Registers windows used to cover selected monitors.
//
ATOM RegisterScreen(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProcScreen;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MONITORZEN));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0)); /*(HBRUSH)GetStockObject(COLOR_WINDOW + 1);*/
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MONITORZEN);
	wcex.lpszClassName = L"screen";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
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

	// create main window
	HWND hWnd = CreateWindowW(szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW & (~WS_THICKFRAME) & (~WS_MINIMIZEBOX) & (~WS_MAXIMIZEBOX),
		CW_USEDEFAULT,
		0,
		400,
		400,
		nullptr, nullptr, hInstance, nullptr);

	// !!! refactor; 
	// create button in main window
	// TODO: See if there is any other way of handling buttons.
	HWND hwndButton = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"Cover Monitors",      // Button text 
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_FLAT,//WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		133 + 12,         // x position 
		300,         // y position 
		105,        // Button width
		30,        // Button height
		hWnd,     // Parent window
		(HMENU)IDC_ACTIVATE_SCREEN,       // No menu.
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);      // Pointer not needed.

	

	// START @@@
	// Create the list-view window in report view with label editing enabled.
	long offset = 0;
	hCheckBox = CreateWindow(TEXT("button"), TEXT("Monitor 1"),
		WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
		20, 20, 185, 35,
		hWnd, (HMENU)IDC_CHECKBOX + offset, hInstance, 0);
	/*hCheckBox = CreateWindow(TEXT("button"), TEXT("Monitor X"),
		WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
		20, 20, 185, 35,
		hWnd, (HMENU)IDC_CHECKBOX, hInstance, NULL);*/

	// END

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);

	UpdateWindow(hWnd);

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
		BOOL checked;

		// Parse the menu selections:
		if (wmId >= IDC_CHECKBOX)
		{
			checked = IsDlgButtonChecked(hWnd, wmId);
			if (checked) {
				CheckDlgButton(hWnd, wmId, BST_UNCHECKED);
			}
			else {
				CheckDlgButton(hWnd, wmId, BST_CHECKED);
			}
			break;
		}

		switch (wmId)
		{
		case IDC_ACTIVATE_SCREEN: // handle main button press
			// change to: handle button specifically; by handle?
			CreateOverlays(hInst, 0);
			break;
		//case IDC_CHECKBOX:
		//	// START @@@
		//	checked = IsDlgButtonChecked(hWnd, wmId);
		//	if (checked) {
		//		CheckDlgButton(hWnd, wmId, BST_UNCHECKED);
		//	}
		//	else {
		//		CheckDlgButton(hWnd, wmId, BST_CHECKED);
		//	}
		//	break;
		//	// END
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
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
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


LRESULT CALLBACK WndProcScreen(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
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

BOOL CreateOverlays(HINSTANCE hInstance, int nCmdShow)
{
	// TODO: store a list of hWndScreens; allow hotkey to destroy and spawn them.
	int topLeftX = 0; int topLeftY = 0;
	int botRightX = 0; int botRightY = 0;
	MONITORINFO *info;
	HWND hWndScreen;

	
	for (auto it = monitorInfos.begin(); it != monitorInfos.end(); ++it)
	{
		info = &it->second;
		topLeftX = info->rcMonitor.left;
		topLeftY = info->rcMonitor.top;
		botRightX = info->rcMonitor.right;
		botRightY = info->rcMonitor.bottom;

		hWndScreen = CreateWindowW(L"screen",
			L"Meme",
			WS_OVERLAPPEDWINDOW & (~WS_THICKFRAME) & (~WS_MINIMIZEBOX) & (~WS_MAXIMIZEBOX),
			topLeftX,
			topLeftY,
			botRightX - topLeftX,
			botRightY - topLeftY,
			nullptr, nullptr, nullptr, nullptr);

		SetWindowLong(hWndScreen, GWL_STYLE, 0); // removes title bar
		SetMenu(hWndScreen, NULL); // removes menu bar

		ShowWindow(hWndScreen, SW_SHOWDEFAULT);
		UpdateWindow(hWndScreen);
	}




	return TRUE;
}

// Helps enumerate monitors and populates 
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	// Initialize and store MONITORINFO globally
	MONITORINFO info;
	info.cbSize = sizeof(info); // req
	GetMonitorInfo(hMonitor, &info);
	monitorInfos.insert({ hMonitor, info });

	int *Count = (int*)dwData;
	(*Count)++;
	return TRUE;
}

int MonitorCount()
{
	int Count = 0;
	if (EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&Count))
		return Count;
	return -1;		//signals an error
}

