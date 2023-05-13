#include "globals.h"
#include "browser_wnd.h"
#include "htmlview_wnd.h"
#include "toolbar_wnd.h"

browser_wnd::browser_wnd(int hInst) {
	m_hInst = hInst;
	m_hWnd = NULL;
	m_view = new htmlview_wnd(hInst, this);
#ifndef NO_TOOLBAR
	m_toolbar = new toolbar_wnd(hInst, this);
#endif
}

browser_wnd::~browser_wnd(void) {
	if (m_view) delete m_view;
#ifndef NO_TOOLBAR
	if (m_toolbar) delete m_toolbar;
#endif
}

void browser_wnd::OnCreate() {
	RECTX rcClient;
	GetClientRect(m_pose, m_bounds, &rcClient);
#ifndef NO_TOOLBAR
	m_toolbar->create(rcClient.left, rcClient.top, rcClient.right - rcClient.left, m_hWnd);
	m_view->create(rcClient.left, rcClient.top + m_toolbar->height(), rcClient.right - rcClient.left, rcClient.bottom - rcClient.top - m_toolbar->height(), m_hWnd);
#else
	m_view->create(rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, m_hWnd);
#endif
	//SetFocus(m_view->wnd());
}

void browser_wnd::OnSize(int width, int height) {
	RECTX rcClient;
	GetClientRect(m_pose, m_bounds, &rcClient);
#ifndef NO_TOOLBAR
	int toolbar_height = m_toolbar->set_width(rcClient.right - rcClient.left);
#else
	int toolbar_height = 0;
#endif
	//SetWindowPos(m_view->wnd(), NULL, rcClient.left, rcClient.top + toolbar_height, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top - toolbar_height, SWP_NOZORDER);
	//UpdateWindow(m_view->wnd());
#ifndef NO_TOOLBAR
	//SetWindowPos(m_toolbar->wnd(), NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, toolbar_height, SWP_NOZORDER);
	//UpdateWindow(m_toolbar->wnd());
#endif
}

void browser_wnd::OnDestroy() {
}

void browser_wnd::create() {
	m_hWnd = 0;
	//ShowWindow(m_hWnd, SW_SHOW);
}

void browser_wnd::update() {
	ui_window_begin("Window", m_pose, {100, 100});
	
	ui_window_end();
}

void browser_wnd::open(LPCWSTR path) {
	if (m_view) {
		m_view->open(path, true);
	}
}

void browser_wnd::back() {
	if (m_view) {
		m_view->back();
	}
}

void browser_wnd::forward() {
	if (m_view) {
		m_view->forward();
	}
}

void browser_wnd::reload() {
	if (m_view) {
		m_view->refresh();
	}
}

void browser_wnd::calc_time(int calc_repeat) {
	if (m_view) {
		m_view->render(TRUE, TRUE, calc_repeat);
	}
}

void browser_wnd::calc_redraw(int calc_repeat) {
	if (m_view) {
		m_view->calc_draw(calc_repeat);
	}
}

void browser_wnd::on_page_loaded(LPCWSTR url) {
	if (m_view) {
		//SetFocus(m_view->wnd());
	}
#ifndef NO_TOOLBAR
	m_toolbar->on_page_loaded(url);
#endif
}
