#pragma once

#include "bmp.h"
#include "../glut.h"

#include <vector>

namespace Sokoban {

	void init_texture();

	void display();

	void reshape(int width, int height);
	void update_view(int width, int height);

	void idle();
}

namespace Texture {
	using std::vector;

	// textures
	static const int count = 1;
	enum TextureID {
		BOX
	};

	// texture ID
	extern vector<GLuint> textures;
	// images
	extern vector<bmpImage*> images;
}
