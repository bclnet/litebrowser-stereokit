#include "globals.h"
#include "defaults.h"

material_t defaults::material;
material_t defaults::materialUnlit;
mesh_t defaults::meshQuad;
mesh_t defaults::meshSphere;

void defaults::init() {
	material = material_find(default_id_material);
	materialUnlit = material_find(default_id_material_unlit);
	meshQuad = mesh_find(default_id_mesh_quad);
	meshSphere = mesh_find(default_id_mesh_sphere);
}