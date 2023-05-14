#pragma once

typedef int XINSTANCE;

typedef struct tagWND {
	struct tagWND*  parent;
	LPCWSTR			name;
	sk::pose_t		pose;
	sk::bounds_t	bounds;
	WNDPROC			wndproc;
	LPVOID			prop;
} WND, * XWND;

typedef struct tagRECTX {
	LONG    left;
	LONG    top;
	LONG    right;
	LONG    bottom;
} RECTX, * PRECTX, NEAR* NPRECTX, FAR* LPRECTX;

#define GET_X_LPARAMX(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAMX(lp) ((int)(short)HIWORD(lp))

BOOL IsWindowX(XWND hWnd);
XWND CreateWindowX(LPCWSTR lpWindowName, int x, int y, int z, int nWidth, int nHeight, int nDepth, XWND hWndParent, LPVOID hMenu, XINSTANCE hInstance, LPVOID lpParam, WNDPROC wndproc);
BOOL GetClientRectX(XWND hWnd, RECTX* rect);
BOOL SetWindowPosX(XWND hWnd, XWND hWndInsertAfter, int x, int y, int z, int cx, int cy, int cz, UINT uFlags);
XWND SetFocusX(XWND hWnd);
BOOL InvalidateRectX(XWND hWnd, const RECTX* lpRect, BOOL bErase);
BOOL UpdateWindowX(XWND hWnd);
BOOL ShowWindowX(XWND hWnd, int nCmdShow);
int  MessageBoxX(XWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);
BOOL ShowScrollBarX(XWND hWnd, int wBar, BOOL bShow);
int  SetScrollInfoX(XWND hwnd, int nBar, LPCSCROLLINFO lpsi, BOOL redraw);
int  SetScrollPosX(XWND hWnd, int nBar, int nPos, BOOL bRedraw);
BOOL SetWindowTextX(XWND hwnd, LPCWSTR lpString);
XWND GetParentX(XWND hWnd);
BOOL MoveWindowX(XWND hWnd, int x, int y, int nWidth, int nHeight, BOOL bRepaint);
LRESULT SendMessageX(XWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT SendMessageX(XWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL PostMessageX(XWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL OpenClipboardX(XWND hWndNewOwner);
HANDLE GetPropX(XWND hWnd, LPCWSTR lpString);
BOOL SetPropX(XWND hWnd, LPCWSTR lpString, HANDLE hData);
HANDLE RemovePropX(XWND hWnd, LPCWSTR lpString);
HWND SetCaptureX(XWND hWnd);
BOOL ReleaseCaptureX();
LRESULT DefWindowProcX(XWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);