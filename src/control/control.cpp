#include "control.h"
#include "../model/model.h"

#include <iostream>

namespace Sokoban {

	void keyboard(unsigned char key, int  x, int y)
	{
		switch (key)
		{
		case 'a':
			object_list[0]->move_to(object_list[0]->get_position() - vector<int>{1, 0, 0});
			break;
		case 'd':
			object_list[0]->move_to(object_list[0]->get_position() + vector<int>{1, 0, 0});
			break;
		case 'w':
			object_list[0]->move_to(object_list[0]->get_position() + vector<int>{0, 1, 0});
			break;
		case 's':
			object_list[0]->move_to(object_list[0]->get_position() - vector<int>{0, 1, 0});
			break;
		default:
			break;
		}
	}
}