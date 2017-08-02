#pragma once
#include <utility>
#include "resource.h"
BOOL CreateOverlays(HINSTANCE hInstance, int nCmdShow);
ATOM RegisterScreen(HINSTANCE hInstance);
LRESULT CALLBACK WndProcScreen(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
int MonitorCount();


//std::sort(items.begin(), items.end(), cmp);
class compare_1
{
public:
	bool operator()(const MONITORINFO &x, const MONITORINFO &y)
	{
		return x.rcMonitor.left < y.rcMonitor.left;
	}
};