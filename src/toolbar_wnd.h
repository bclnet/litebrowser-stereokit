#pragma once

#include "../containers/cairo/cairo_container.h"
#include "dib.h"
#include "litehtml/el_omnibox.h"
class browser_wnd;

class toolbar_wnd : public cairo_container {
	pose_t m_pose = { };
	bounds_t m_bounds = { };
	int m_hWnd;
	int m_hInst;
	litehtml::document::ptr	m_doc;
	browser_wnd* m_parent;
	std::shared_ptr<el_omnibox>	m_omnibox;
	litehtml::string		m_cursor;
	BOOL					m_inCapture;
public:
	toolbar_wnd(int hInst, browser_wnd* parent);
	virtual ~toolbar_wnd(void);

	void create(int x, int y, int width, int parent);
	int wnd() { return m_hWnd; }
	int height() {
		return m_doc ? m_doc->height() : 0;
	}
	int set_width(int width);
	void on_page_loaded(LPCWSTR url);

	// cairo_container members
	virtual void	make_url(LPCWSTR url, LPCWSTR basepath, std::wstring& out);
	virtual cairo_container::image_ptr get_image(LPCWSTR url, bool redraw_on_ready);

	// litehtml::document_container members
	virtual	void	set_caption(const char* caption);
	virtual	void	set_base_url(const char* base_url);
	virtual	void	link(std::shared_ptr<litehtml::document>& doc, litehtml::element::ptr el);
	virtual void	import_css(litehtml::string& text, const litehtml::string& url, litehtml::string& baseurl);
	virtual	void	on_anchor_click(const char* url, const litehtml::element::ptr& el);
	virtual	void	set_cursor(const char* cursor);
	virtual std::shared_ptr<litehtml::element> create_element(const char* tag_name, const litehtml::string_map& attributes, const std::shared_ptr<litehtml::document>& doc);

protected:
	virtual void	OnCreate();
	virtual void	OnPaint(simpledib::dib* dib, LPRECT rcDraw);
	virtual void	OnSize(int width, int height);
	virtual void	OnDestroy();
	virtual void	OnMouseMove(int x, int y);
	virtual void	OnLButtonDown(int x, int y);
	virtual void	OnLButtonUp(int x, int y);
	virtual void	OnMouseLeave();
	virtual void	OnOmniboxClicked();

	virtual void	get_client_rect(litehtml::position& client) const;

private:
	static LRESULT CALLBACK WndProc(int hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
	void render_toolbar(int width);
	void update_cursor();
};