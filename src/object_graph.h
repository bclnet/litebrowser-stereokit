#pragma once

class object_graph {
public:
public:
	object_graph();
	virtual ~object_graph();
	void update();

	void insertSprite(unsigned char* bits, int width, int height);
};
