#pragma once

class htmlview_wnd;
class toolbar_wnd;

class browser_wnd {
	XWND				m_hWnd;
	XINSTANCE			m_hInst;
	htmlview_wnd*		m_view;
#ifndef NO_TOOLBAR
	toolbar_wnd*		m_toolbar;
#endif
public:
	browser_wnd(XINSTANCE hInst);
	virtual ~browser_wnd(void);

	void create();
	void update();
	void open(LPCWSTR path);

	void back();
	void forward();
	void reload();
	void calc_time(int calc_repeat = 1);
	void calc_redraw(int calc_repeat = 1);
	void on_page_loaded(LPCWSTR url);

	virtual void OnCreate();
protected:
	virtual void OnSize(int width, int height);
	virtual void OnDestroy();

private:
	static LRESULT CALLBACK WndProc(XWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
};
