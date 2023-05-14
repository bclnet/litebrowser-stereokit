#include "globals.h"
#include "browser_wnd.h"
#include "..\containers\cairo\cairo_font.h"
using namespace Gdiplus;

mesh_t     cube_mesh;
material_t cube_mat;
pose_t     cube_pose = { {0,0,-0.5f}, quat_identity };
browser_wnd browser(0);

CRITICAL_SECTION cairo_font::m_sync;

int main(int argc, char* argv[]) {
#if _WIN32
	CoInitialize(NULL);
	InitCommonControls();
	InitializeCriticalSectionAndSpinCount(&cairo_font::m_sync, 1000);
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
#endif

	sk_settings_t settings = {};
	settings.app_name = "litebrowser";
	settings.assets_folder = "Assets";
	settings.display_preference = display_mode_mixedreality;
	if (!sk_init(settings))
		return 1;

	cube_mesh = mesh_gen_rounded_cube(vec3_one * 0.1f, 0.02f, 4);
	cube_mat = material_find(default_id_material_ui);

	browser.create();
	if (argc > 1) {
		browser.open((LPCWSTR)argv[0]);
	}
	else {
		browser.open(L"http://www.dmoz.org/");
	}

	sk_run([]() {
		//ui_handle_begin("Cube", cube_pose, mesh_get_bounds(cube_mesh), false);
		//render_add_mesh(cube_mesh, cube_mat, matrix_identity);
		//ui_handle_end();
		browser.update();
		});

#if _WIN32
	GdiplusShutdown(gdiplusToken);
#endif
	return 0;
}