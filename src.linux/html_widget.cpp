#include "globals.h"
#include "html_widget.h"
#include "browser_wnd.h"
#include <litehtml/url_path.h>
#include <litehtml/url.h>
#include <chrono>
#include <ostream>
#include <stack>

#define BUFF_SIZE    10 * 1024

class html_dumper : public litehtml::dumper {
	std::ofstream m_cout;
	int indent;
	std::list<std::tuple<int, std::string>> m_node_text;
private:
	void print_indent(int size) {
		m_cout << litehtml::string(size, '\t');
	}

public:
	html_dumper(const litehtml::string& file_name) : m_cout(file_name), indent(0) {
	}

	void begin_node(const litehtml::string& descr) override {
		m_node_text.emplace_back(std::make_tuple(indent, "#" + descr));
		indent++;
	}

	void end_node() override {
		indent--;
	}

	void begin_attrs_group(const litehtml::string& descr) override {
	}

	void end_attrs_group() override {
	}

	void add_attr(const litehtml::string& name, const litehtml::string& value) override {
		if (name == "display" || name == "float") {
			std::get<1>(m_node_text.back()) += " " + name + "[" + value + "]";
		}
	}

	void print() {
		for (const auto& data : m_node_text) {
			print_indent(std::get<0>(data));
			m_cout << std::get<1>(data) << std::endl;
		}
	}
};

html_widget::html_widget(browser_window* browser) {
	m_hash_valid = false;
	m_browser = browser;
	m_rendered_width = 0;
	m_html = nullptr;
	add_events(Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
}

html_widget::~html_widget() {
}

bool html_widget::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	litehtml::position pos;

	GdkRectangle rect;
	gdk_cairo_get_clip_rectangle(cr->cobj(), &rect);

	pos.width = rect.width;
	pos.height = rect.height;
	pos.x = rect.x;
	pos.y = rect.y;

	cr->rectangle(0, 0, get_allocated_width(), get_allocated_height());
	cr->set_source_rgb(1, 1, 1);
	cr->fill();

	if (m_html) {
		m_html->draw((litehtml::uint_ptr)cr->cobj(), 0, 0, &pos);
	}

	return true;
}

void html_widget::get_client_rect(litehtml::position& client) const {
	client.width = get_parent()->get_allocated_width();
	client.height = get_parent()->get_allocated_height();
	client.x = 0;
	client.y = 0;
}

void html_widget::on_anchor_click(const char* url, const litehtml::element::ptr& el) {
	if (url) {
		make_url(url, m_base_url.c_str(), m_clicked_url);
	}
}

void html_widget::set_cursor(const char* cursor) {
	if (cursor) {
		if (m_cursor != cursor) {
			m_cursor = cursor;
			update_cursor();
		}
	}
}

void html_widget::import_css(litehtml::string& text, const litehtml::string& url, litehtml::string& baseurl) {
	std::string css_url;
	make_url(url.c_str(), baseurl.c_str(), css_url);
	load_text_file(css_url, text);
	if (!text.empty()) {
		baseurl = css_url;
	}
}

void html_widget::set_caption(const char* caption) {
	if (get_parent_window()) {
		get_parent_window()->set_title(caption);
	}
}

void html_widget::set_base_url(const char* base_url) {
	if (base_url) {
		m_base_url = litehtml::resolve(litehtml::url(m_url), litehtml::url(base_url)).str();
	}
	else {
		m_base_url = m_url;
	}
}

Glib::RefPtr<Gdk::Pixbuf> html_widget::get_image(const char* url, bool redraw_on_ready) {
	Glib::RefPtr<Gio::InputStream> stream = m_http.load_file(url);
	Glib::RefPtr<Gdk::Pixbuf> ptr = Gdk::Pixbuf::create_from_stream(stream);
	return ptr;
}

Gtk::Allocation html_widget::get_parent_allocation() {
	Gtk::Container* parent = get_parent();
	return parent->get_allocation();
}

void html_widget::open_page(const litehtml::string& url, const litehtml::string& hash) {
	m_url = url;
	m_base_url = url;

	std::string html;
	load_text_file(url, html);
	m_url = m_http.get_url();
	m_base_url = m_http.get_url();
	m_browser->set_url(m_url);
	m_html = litehtml::document::createFromString(html.c_str(), this);
	if (m_html) {
		m_rendered_width = get_parent_allocation().get_width();
		m_html->render(m_rendered_width);
		m_hash = hash;
		m_hash_valid = true;
		set_size_request(m_html->width(), m_html->height());
	}

	queue_draw();
}

void html_widget::scroll_to(int x, int y) {
	auto vadj = m_browser->get_scrolled()->get_vadjustment();
	auto hadj = m_browser->get_scrolled()->get_hadjustment();
	vadj->set_value(vadj->get_lower() + y);
	hadj->set_value(hadj->get_lower() + x);
}

void html_widget::show_hash(const litehtml::string& hash) {
	if (hash.empty()) {
		scroll_to(0, 0);
	}
	else {
		std::string selector = "#" + hash;
		litehtml::element::ptr el = m_html->root()->select_one(selector);
		if (!el) {
			selector = "[name=" + hash + "]";
			el = m_html->root()->select_one(selector);
		}
		if (el) {
			litehtml::position pos = el->get_placement();
			scroll_to(0, pos.top());
		}
	}
}

void html_widget::make_url(const char* url, const char* basepath, litehtml::string& out) {
	if (!basepath || !basepath[0]) {
		if (!m_base_url.empty()) {
			out = litehtml::resolve(litehtml::url(m_base_url), litehtml::url(url)).str();
		}
		else {
			out = url;
		}
	}
	else {
		out = litehtml::resolve(litehtml::url(basepath), litehtml::url(url)).str();
	}
}

void html_widget::on_parent_size_allocate(Gtk::Allocation allocation) {
	if (m_html && m_rendered_width != allocation.get_width()) {
		m_rendered_width = allocation.get_width();
		m_html->media_changed();
		m_html->render(m_rendered_width);
		set_size_request(m_html->width(), m_html->height());
		queue_draw();
	}
}

void html_widget::on_parent_changed(Gtk::Widget* previous_parent) {
	Gtk::Widget* viewport = get_parent();
	if (viewport) {
		viewport->signal_size_allocate().connect(sigc::mem_fun(*this, &html_widget::on_parent_size_allocate));
	}
}

bool html_widget::on_button_press_event(GdkEventButton* event) {
	if (m_html) {
		litehtml::position::vector redraw_boxes;
		if (m_html->on_lbutton_down((int)event->x, (int)event->y, (int)event->x, (int)event->y, redraw_boxes)) {
			for (auto& pos : redraw_boxes) {
				queue_draw_area(pos.x, pos.y, pos.width, pos.height);
			}
		}
	}
	return true;
}

bool html_widget::on_button_release_event(GdkEventButton* event) {
	if (m_html) {
		litehtml::position::vector redraw_boxes;
		m_clicked_url.clear();
		if (m_html->on_lbutton_up((int)event->x, (int)event->y, (int)event->x, (int)event->y, redraw_boxes)) {
			for (auto& pos : redraw_boxes) {
				queue_draw_area(pos.x, pos.y, pos.width, pos.height);
			}
		}
		if (!m_clicked_url.empty()) {
			m_browser->open_url(m_clicked_url);
		}
	}
	return true;
}

bool html_widget::on_motion_notify_event(GdkEventMotion* event) {
	if (m_html) {
		litehtml::position::vector redraw_boxes;
		if (m_html->on_mouse_over((int)event->x, (int)event->y, (int)event->x, (int)event->y, redraw_boxes)) {
			for (auto& pos : redraw_boxes) {
				queue_draw_area(pos.x, pos.y, pos.width, pos.height);
			}
		}
	}
	return true;
}

void html_widget::update_cursor() {
	Gdk::CursorType cursType = Gdk::ARROW;
	if (m_cursor == "pointer") {
		cursType = Gdk::HAND2;
	}
	if (cursType == Gdk::ARROW) {
		get_window()->set_cursor();
	}
	else {
		get_window()->set_cursor(Gdk::Cursor::create(cursType));
	}
}

void html_widget::load_text_file(const litehtml::string& url, litehtml::string& out) {
	out.clear();
	Glib::RefPtr< Gio::InputStream > stream = m_http.load_file(url);
	gssize sz;
	char buff[BUFF_SIZE + 1];
	while ((sz = stream->read(buff, BUFF_SIZE)) > 0) {
		buff[sz] = 0;
		out += buff;
	}
}

long html_widget::draw_measure(int number) {
	auto vadj = m_browser->get_scrolled()->get_vadjustment();
	auto hadj = m_browser->get_scrolled()->get_hadjustment();

	int width = (int)hadj->get_page_size();
	int height = (int)vadj->get_page_size();

	int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, width);
	auto image = (unsigned char*)g_malloc(stride * height);

	cairo_surface_t* surface = cairo_image_surface_create_for_data(image, CAIRO_FORMAT_ARGB32, width, height, stride);
	cairo_t* cr = cairo_create(surface);

	litehtml::position pos;
	pos.width = width;
	pos.height = height;
	pos.x = 0;
	pos.y = 0;

	int x = (int)(hadj->get_value() - hadj->get_lower());
	int y = (int)(vadj->get_value() - vadj->get_lower());

	cairo_rectangle(cr, 0, 0, width, height);
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_paint(cr);
	m_html->draw((litehtml::uint_ptr)cr, -x, -y, &pos);
	cairo_surface_write_to_png(surface, "/tmp/litebrowser.png");

	auto t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < number; i++) {
		m_html->draw((litehtml::uint_ptr)cr, -x, -y, &pos);
	}
	auto t2 = std::chrono::high_resolution_clock::now();

	cairo_destroy(cr);
	cairo_surface_destroy(surface);
	g_free(image);

	return (std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1)).count();
}

long html_widget::render_measure(int number) {
	if (m_html) {
		auto t1 = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < number; i++) {
			m_html->render(m_rendered_width);
		}
		auto t2 = std::chrono::high_resolution_clock::now();
		return (std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1)).count();
	}
	return -1;
}

void html_widget::on_size_allocate(Gtk::Allocation& allocation) {
	Gtk::DrawingArea::on_size_allocate(allocation);
	if (m_hash_valid) {
		show_hash(m_hash);
		m_hash_valid = false;
	}
}

void html_widget::dump(const litehtml::string& file_name) {
	if (m_html) {
		html_dumper dumper(file_name);
		m_html->dump(dumper);
		dumper.print();
	}
}