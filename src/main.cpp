#include "globals.h"
#include "defaults.h"
#include "browser_wnd.h"
#include "..\containers\cairo\cairo_font.h"
#include <sstream>
using namespace std;
using namespace Gdiplus;

browser_wnd browser(0);

CRITICAL_SECTION cairo_font::m_sync;

#pragma region MODEL
//mesh_t     cube_mesh;
//material_t cube_mat;
//pose_t     cube_pose = { {0, 0, -0.5f}, quat_identity };
//: init
//cube_mesh = mesh_gen_rounded_cube(vec3_one * 0.1f, 0.02f, 4);
//cube_mat = material_find(default_id_material_ui);
//: render
//ui_handle_begin("Cube", cube_pose, mesh_get_bounds(cube_mesh), false);
//render_add_mesh(cube_mesh, cube_mat, matrix_identity);
//ui_handle_end();
#pragma endregion

#pragma region LOG

pose_t log_pose = { {-0.75f, -0.1f, -0.5f}, quat_lookat(vec3_zero, vec3_forward) };
vector<string> log_list;
string log_text = "";

void OnLog(log_ level, const char* text) {
	if (log_list.size() > 15) log_list.erase(log_list.end() - 1);
	log_list.insert(log_list.begin(), strlen(text) < 100 ? std::string(text) : std::string(text, 100) + "...");
	stringstream b;
	for (vector<string>::iterator iter = log_list.begin(); iter < log_list.end(); iter++) b << *iter;
	log_text = b.str();
}

void LogWindow() {
	ui_window_begin("Log", log_pose, vec2{ 40.0f, 0.0f }*0.01f);
	ui_text(log_text.c_str());
	ui_window_end();
}

#pragma endregion

int main(int argc, char* argv[]) {
#if _WIN32
	CoInitialize(NULL);
	InitCommonControls();
	InitializeCriticalSectionAndSpinCount(&cairo_font::m_sync, 1000);
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
#endif

	log_subscribe(OnLog);

	sk_settings_t settings = {};
	settings.app_name = "litebrowser";
	settings.assets_folder = "Assets";
	settings.display_preference = display_mode_mixedreality;
	if (!sk_init(settings))
		return 1;

	defaults::init();
	browser.init();
	browser.create();
	if (argc > 1) {
		browser.open((LPCWSTR)argv[0]);
	}
	else {
		browser.open(L"http://www.litehtml.com/");
		//browser.open(L"https://dmoz-odp.org/");
	}

	sk_run([]() {
		LogWindow();
		browser.update();
		});

#if _WIN32
	GdiplusShutdown(gdiplusToken);
#endif
	return 0;
}