#include "globals.h"
#include "browser_wnd.h"

XWND CreateWindowX(LPCWSTR lpWindowName, int x, int y, int z, int nWidth, int nHeight, int nDepth, XWND hWndParent, LPVOID hMenu, XINSTANCE hInstance, LPVOID lpParam, WNDPROC wndproc) {
	printf("CreateWindowX[%ls]: (%d,%d,%d).(%d,%d,%d)\n", lpWindowName, x, y, z, nWidth, nHeight, nDepth);
	XWND hWnd = (XWND)lpParam;
	hWnd->parent = hWndParent;
	hWnd->name = lpWindowName;
	hWnd->pose = {
		{
			(float)(x != CW_USEDEFAULT ? x : 0),
			(float)(y != CW_USEDEFAULT ? y : 0),
			(float)(z != CW_USEDEFAULT ? z : -2)
		}, quat_identity };
	hWnd->wndproc = wndproc;
	CREATESTRUCT c;
	ZeroMemory(&c, sizeof(c));
	c.lpCreateParams = (browser_wnd*)lpParam;
	c.hInstance = (HINSTANCE)hInstance;
	c.hMenu = (HMENU)hMenu;
	wndproc((HWND)hWnd, WM_CREATE, NULL, (LPARAM)&c);
	return hWnd;
}

BOOL GetClientRectX(XWND hWnd, RECTX* rect) {
	if (!hWnd) return FALSE;
	printf("GetClientRectX[%ls]:\n", hWnd->name);
	pose_t p = hWnd->pose;
	rect->left = p.position.x;
	rect->right = p.orientation.x - p.position.x;
	rect->top = p.position.y;
	rect->bottom = p.orientation.y - p.position.y;
	return TRUE;
}

BOOL SetWindowPosX(XWND hWnd, XWND hWndInsertAfter, int x, int y, int z, int cx, int cy, int cz, UINT uFlags) {
	printf("SetWindowPosX[%ls]:\n", hWnd->name);
	return TRUE;
}

XWND SetFocusX(XWND hWnd) {
	printf("SetFocusX[%ls]\n", hWnd->name);
	return hWnd;
}

BOOL InvalidateRectX(XWND hWnd, const RECTX* lpRect, BOOL bErase) {
	printf("InvalidateRectX[%ls]\n", hWnd->name);
	return TRUE;
}

BOOL UpdateWindowX(XWND hWnd) {
	printf("UpdateWindowX[%ls]\n", hWnd->name);
	return TRUE;
}

BOOL ShowWindowX(XWND hWnd, int nCmdShow) {
	printf("ShowWindowX[%ls]\n", hWnd->name);
	return TRUE;
}

int  MessageBoxX(XWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) {
	printf("MessageBoxX[%ls]\n", hWnd->name);
	return 0;
}

BOOL ShowScrollBarX(XWND hWnd, int wBar, BOOL bShow) {
	printf("ShowScrollBarX[%ls]\n", hWnd->name);
	return TRUE;
}

int  SetScrollInfoX(XWND hWnd, int nBar, LPCSCROLLINFO lpsi, BOOL redraw) {
	printf("SetScrollInfoX[%ls]\n", hWnd->name);
	return 0;
}

int  SetScrollPosX(XWND hWnd, int nBar, int nPos, BOOL bRedraw) {
	printf("SetScrollPosX[%ls]\n", hWnd->name);
	return 0;
}

BOOL SetWindowTextX(XWND hWnd, LPCWSTR lpString) {
	printf("SetWindowTextX[%ls]\n", hWnd->name);
	return TRUE;
}

XWND GetParentX(XWND hWnd) {
	printf("GetParentX[%ls]\n", hWnd->name);
	return hWnd;
}

BOOL MoveWindowX(XWND hWnd, int x, int y, int nWidth, int nHeight, BOOL bRepaint) {
	printf("MoveWindowX[%ls]\n", hWnd->name);
	return TRUE;
}

LRESULT SendMessageX(XWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	printf("SendMessageX[%ls]\n", hWnd->name);
	hWnd->wndproc((HWND)hWnd, Msg, wParam, lParam);
	return 0;
}

BOOL PostMessageX(XWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	printf("PostMessageX[%ls]\n", hWnd->name);
	return TRUE;
}

BOOL OpenClipboardX(XWND hWnd) {
	printf("OpenClipboardX[%ls]\n", hWnd->name);
	return FALSE;
}

BOOL SetPropX(XWND hWnd, LPCWSTR lpString, HANDLE hData) {
	printf("SetPropX[%ls]: %ls\n", hWnd->name, lpString);
	return TRUE;
}

HANDLE RemovePropX(XWND hWnd, LPCWSTR lpString) {
	printf("RemovePropX[%ls]: %ls\n", hWnd->name, lpString);
	return 0;
}

LRESULT DefWindowProcX(XWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	printf("DefWindowProcX[%ls]: %d\n", hWnd->name, Msg);
	return 0;
}