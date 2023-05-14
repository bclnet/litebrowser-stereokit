#include "globals.h"
#include "object_graph.h"

object_graph::object_graph() {
}

object_graph::~object_graph() {
}

tex_t tex = nullptr;
sprite_t sprite = nullptr;

void object_graph::insertSprite(unsigned char* bits, int width, int height) {
	tex = tex_create_color32((color32*)bits, width, height, 0);
	//tex = tex_create_mem(bits, width * height * 4);
	sprite = sprite_create(tex, sprite_type_single);
}

void object_graph::update() {
	if (sprite) ui_image(sprite, { 2, 2 });
}