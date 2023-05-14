#include "globals.h"
#include "browser_wnd.h"

const int WNDS_MAX = 5;
const int PAINTS_MAX = 5;

static WND wnds[WNDS_MAX]; static int wndi = 0;
static PAINTSTRUCTX paints[PAINTS_MAX]; static int painti = 0;
LPCWSTR MsgToString(UINT Msg);

BOOL IsWindowX(XWND hWnd) {
	return hWnd ? TRUE : FALSE;
}

XWND CreateWindowX(LPCWSTR lpWindowName, int x, int y, int z, int nWidth, int nHeight, int nDepth, XWND hWndParent, LPVOID hMenu, XINSTANCE hInstance, LPVOID lpParam, WNDPROC wndproc) {
	printf("CreateWindowX[%ls]: (%d,%d,%d).(%d,%d,%d)\n", lpWindowName, x, y, z, nWidth, nHeight, nDepth);
	XWND hWnd = &wnds[wndi++];
	hWnd->parent = hWndParent;
	hWnd->name = lpWindowName;
	hWnd->pose = {
		{
			(float)(x != CW_USEDEFAULT ? x : 0),
			(float)(y != CW_USEDEFAULT ? y : 0),
			(float)(z != CW_USEDEFAULT ? z : -1)
		}, quat_identity };
	hWnd->wndproc = wndproc;
	// send create
	CREATESTRUCT c;
	ZeroMemory(&c, sizeof(c));
	c.lpCreateParams = (browser_wnd*)lpParam;
	c.hInstance = (HINSTANCE)hInstance;
	c.hMenu = (HMENU)hMenu;
	SendMessageX(hWnd, WM_CREATE, NULL, (LPARAM)&c);
	// send size
	SendMessageX(hWnd, WM_SIZE, NULL, LOWORD(nWidth) | HIWORD(nHeight));
	// invalidate
	InvalidateRectX(hWnd, NULL, FALSE);
	return hWnd;
}

BOOL GetClientRectX(XWND hWnd, RECTX* rect) {
	if (!hWnd) return FALSE;
	//printf("GetClientRectX[%ls]:\n", hWnd->name);
	pose_t p = hWnd->pose;
	rect->left = p.position.x;
	rect->right = p.orientation.x - p.position.x;
	rect->top = p.position.y;
	rect->bottom = p.orientation.y - p.position.y;
	return TRUE;
}

BOOL SetWindowPosX(XWND hWnd, XWND hWndInsertAfter, int x, int y, int z, int cx, int cy, int cz, UINT uFlags) {
	printf("SetWindowPosX[%ls]: (%d,%d,%d).(%d,%d,%d) %d\n", hWnd->name, x, y, z, cx, cy, cz, uFlags);
	pose_t* p = &hWnd->pose;
	p->position.x = x;
	p->position.y = y;
	//p->position.z = z;
	return TRUE;
}

XWND SetFocusX(XWND hWnd) {
	printf("SetFocusX[%ls]:\n", hWnd->name);
	return hWnd;
}

BOOL InvalidateRectX(XWND hWnd, const RECTX* lpRect, BOOL bErase) {
	printf("InvalidateRectX[%ls]: %d %d\n", hWnd->name, lpRect, bErase);
	if (lpRect) UnionRectX(&hWnd->rcDirty, &hWnd->rcDirty, lpRect);
	else {
		hWnd->rcDirty.left = 0;
		hWnd->rcDirty.top = 0;
		hWnd->rcDirty.front = 0;
		hWnd->rcDirty.right = LONG_MAX;
		hWnd->rcDirty.bottom = LONG_MAX;
		hWnd->rcDirty.back = LONG_MAX;
	}
	if (bErase) hWnd->bErase = TRUE;
	return TRUE;
}

BOOL UpdateWindowX(XWND hWnd) {
	printf("UpdateWindowX[%ls]:\n", hWnd->name);
	if (!GetUpdateRectX(hWnd, NULL, FALSE)) return FALSE;
	SendMessageX(hWnd, WM_PAINT, NULL, NULL);
	return TRUE;
}

BOOL ShowWindowX(XWND hWnd, int nCmdShow) {
	printf("ShowWindowX[%ls]: %d\n", hWnd->name, nCmdShow);
	return TRUE;
}

int MessageBoxX(XWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) {
	printf("MessageBoxX[%ls]:\n", hWnd->name);
	return 0;
}

BOOL ShowScrollBarX(XWND hWnd, int wBar, BOOL bShow) {
	printf("ShowScrollBarX[%ls]:\n", hWnd->name);
	return TRUE;
}

int SetScrollInfoX(XWND hWnd, int nBar, LPCSCROLLINFO lpsi, BOOL redraw) {
	printf("SetScrollInfoX[%ls]:\n", hWnd->name);
	return 0;
}

int SetScrollPosX(XWND hWnd, int nBar, int nPos, BOOL bRedraw) {
	printf("SetScrollPosX[%ls]:\n", hWnd->name);
	return 0;
}

BOOL SetWindowTextX(XWND hWnd, LPCWSTR lpString) {
	printf("SetWindowTextX[%ls]:\n", hWnd->name);
	return TRUE;
}

XWND GetParentX(XWND hWnd) {
	printf("GetParentX[%ls]:\n", hWnd->name);
	return hWnd;
}

BOOL MoveWindowX(XWND hWnd, int x, int y, int z, int nWidth, int nHeight, int nDepth, BOOL bRepaint) {
	printf("MoveWindowX[%ls]: (%d,%d,%d).(%d,%d,%d) %d\n", hWnd->name, x, y, z, nWidth, nHeight, nDepth, bRepaint);
	return TRUE;
}

LRESULT SendMessageX(XWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	printf("SendMessageX[%ls]: %ls %d %d\n", hWnd->name, MsgToString(Msg), wParam, lParam);
	hWnd->wndproc((HWND)hWnd, Msg, wParam, lParam);
	return 0;
}

BOOL PostMessageX(XWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	printf("PostMessageX[%ls]: %ls %d %d\n", hWnd->name, MsgToString(Msg), wParam, lParam);
	return TRUE;
}

BOOL OpenClipboardX(XWND hWnd) {
	printf("OpenClipboardX[%ls]:\n", hWnd->name);
	return FALSE;
}

HANDLE GetPropX(XWND hWnd, LPCWSTR lpString) {
	//printf("GetPropX[%ls]: %ls\n", hWnd->name, lpString);
	return hWnd->props[lpString];
}

BOOL SetPropX(XWND hWnd, LPCWSTR lpString, HANDLE hData) {
	//printf("SetPropX[%ls]: %ls\n", hWnd->name, lpString);
	hWnd->props.insert({ lpString, hData });
	return TRUE;
}

HANDLE RemovePropX(XWND hWnd, LPCWSTR lpString) {
	//printf("RemovePropX[%ls]: %ls\n", hWnd->name, lpString);
	hWnd->props.erase(lpString);
	return 0;
}

HWND SetCaptureX(XWND hWnd) {
	printf("SetCaptureX[%ls]:\n", hWnd->name);
	return 0;
}

BOOL ReleaseCaptureX() {
	printf("ReleaseCaptureX:\n");
	return TRUE;
}

LRESULT DefWindowProcX(XWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	//printf("DefWindowProcX[%ls]: %ls %d %d\n", hWnd->name, MsgToString(Msg), wParam, lParam);
	switch (Msg) {
	case WM_PAINT:
		ZeroMemory(&hWnd->rcDirty, sizeof(RECTX));
		hWnd->bErase = FALSE;
		break;
	}
	return 0;
}

HDC BeginPaintX(XWND hWnd, LPPAINTSTRUCTX lpPaint) {
	printf("BeginPaintX[%ls]:\n", hWnd->name);
	RECTX rcPaint;
	if (GetUpdateRectX(hWnd, &rcPaint, FALSE)) return NULL;
	HDC hdc = CreateCompatibleDC(NULL);
	LPPAINTSTRUCTX p = &paints[painti++ % PAINTS_MAX];
	p->hdc = hdc;
	p->fErase = hWnd->bErase;
	p->rcPaint = rcPaint;
	p->fRestore = false;
	p->fIncUpdate = false;
	lpPaint = p;
	// reset
	ZeroMemory(&hWnd->rcDirty, sizeof(RECTX));
	hWnd->bErase = FALSE;
	return hdc;
}

BOOL EndPaintX(XWND hWnd, const PAINTSTRUCTX* lpPaint) {
	printf("EndPaintX[%ls]:\n", hWnd->name);
	DeleteDC(lpPaint->hdc);
	return TRUE;
}

BOOL IntersectRectX(LPRECTX lprcDst, const RECTX* lprcSrc1, const RECTX* lprcSrc2) {
	return IntersectRect((LPRECT)lprcDst, (RECT*)lprcSrc1, (RECT*)lprcSrc2);
}

BOOL UnionRectX(LPRECTX lprcDst, const RECTX* lprcSrc1, const RECTX* lprcSrc2) {
	return UnionRect((LPRECT)lprcDst, (RECT*)lprcSrc1, (RECT*)lprcSrc2);
}

BOOL GetUpdateRectX(XWND hWnd, LPRECTX lpRect, BOOL bErase) {
	if (!lpRect) {
		return !RECTX_EMPTY(hWnd->rcDirty);
	}
	if (RECTX_EMPTY(hWnd->rcDirty)) {
		ZeroMemory(lpRect, sizeof(RECTX));
		return FALSE;
	}
	if (bErase) SendMessageX(hWnd, WM_ERASEBKGND, NULL, NULL);
	lpRect = &hWnd->rcDirty;
	return TRUE;
}

BOOL EvaluteWndX(XWND hWnd) {
	if (!GetUpdateRectX(hWnd, NULL, FALSE)) return FALSE;
	SendMessageX(hWnd, WM_PAINT, NULL, NULL);
	return TRUE;
}

LPCWSTR MsgToString(UINT Msg) {
	switch (Msg) {
	case WM_NULL: return L"WM_NULL";
	case WM_CREATE: return L"WM_CREATE";
	case WM_DESTROY: return L"WM_DESTROY";
	case WM_MOVE: return L"WM_MOVE";
	case WM_SIZE: return L"WM_SIZE";
	case WM_ACTIVATE: return L"WM_ACTIVATE";
	case WM_SETFOCUS: return L"WM_SETFOCUS";
	case WM_KILLFOCUS: return L"WM_KILLFOCUS";
		//case WM_ENABLE: return L"WM_ENABLE";
		//case WM_SETREDRAW: return L"WM_SETREDRAW";
		//case WM_SETTEXT: return L"WM_SETTEXT";
		//case WM_GETTEXT: return L"WM_GETTEXT";
		//case WM_GETTEXTLENGTH: return L"WM_GETTEXTLENGTH";
	case WM_PAINT: return L"WM_PAINT";
	case WM_QUIT: return L"WM_QUIT";
		//case WM_ERASEBKGND: return L"WM_ERASEBKGND";
		//case WM_SYSCOLORCHANGE: return L"WM_SYSCOLORCHANGE";
		//case WM_SHOWWINDOW: return L"WM_SHOWWINDOW";
		//case WM_WININICHANGE: return L"WM_WININICHANGE";
	default: return std::to_wstring(Msg).c_str();
	}
}

