#pragma once

#include "../bmp.h"
#include "../glut.h"

#include <vector>
using namespace std;

namespace Sokoban
{

	void init_texture();

	void display();
	void display_bitmap();

	void reshape(int width, int height);
	void update_view(int width, int height);

	void idle();
	double getFPS();
}
