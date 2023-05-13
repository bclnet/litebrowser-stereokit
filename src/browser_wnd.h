#pragma once

class htmlview_wnd;
class toolbar_wnd;

class browser_wnd {
	pose_t m_pose = { };
	bounds_t m_bounds = { };
	int	m_hWnd;
	int m_hInst;
	htmlview_wnd* m_view;
#ifndef NO_TOOLBAR
	toolbar_wnd* m_toolbar;
#endif
public:
	browser_wnd(int hInst);
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

protected:
	virtual void OnCreate();
	virtual void OnSize(int width, int height);
	virtual void OnDestroy();

private:
	//static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
};
