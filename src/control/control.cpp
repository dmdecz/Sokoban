#include "control.h"
#include "../model/model.h"
#include "../bmp.h"
#include "../glut.h"

#include <iostream>

namespace Sokoban {

	void keyboard(unsigned char key, int  x, int y)
	{
		switch (key)
		{
		case 'a':
			map.get_object(0, 0)->move_to(map.get_object(0, 0)->get_position() - vector<int>{1, 0, 0});
			break;
		case 'd':
			map.get_object(0, 0)->move_to(map.get_object(0, 0)->get_position() + vector<int>{1, 0, 0});
			break;
		case 'w':
			map.get_object(0, 0)->move_to(map.get_object(0, 0)->get_position() + vector<int>{0, 1, 0});
			break;
		case 's':
			map.get_object(0, 0)->move_to(map.get_object(0, 0)->get_position() - vector<int>{0, 1, 0});
			break;
		case ' ': {
			int width = window_size[0] % 4 ? (window_size[0] + 4 - window_size[0] % 4) : window_size[0];
			uchar* screen_data = new uchar[width * window_size[1] * 3];
			glReadPixels(0, 0, width, window_size[1], GL_RGB, GL_UNSIGNED_BYTE, screen_data);
			bmpImage screen(width, window_size[1], screen_data);
			screen.write_back("test.bmp");
			delete[] screen_data;
			break;
		}
		default:
			break;
		}
	}
}