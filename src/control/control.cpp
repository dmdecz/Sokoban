#include "control.h"
#include "../model/model.h"
#include "../bmp.h"
#include "../glut.h"

#include <iostream>

namespace Sokoban {

	int lastmouseX;
	int lastmouseY;
	float speed;
	float yaw , pitch;
	bool mouseAdjust;

	void init_ctrl_paras()
	{
		lastmouseX = 0;
		lastmouseY = 0;

		speed = 0.05f;

		yaw = 270.0f;
		pitch = 0.0f;

		mouseAdjust = false;
	}

	void keyboard(unsigned char key, int  x, int y)
	{
		switch (key)
		{
		case 'a':
			map.get_object(0, 0, 0)->move_to(map.get_object(0, 0, 0)->get_position() - vector<int>{1, 0, 0});
			break;
		case 'd':
			map.get_object(0, 0, 0)->move_to(map.get_object(0, 0, 0)->get_position() + vector<int>{1, 0, 0});
			break;
		case 'w':
			map.get_object(0, 0, 0)->move_to(map.get_object(0, 0, 0)->get_position() + vector<int>{0, 1, 0});
			break;
		case 's':
			map.get_object(0, 0, 0)->move_to(map.get_object(0, 0, 0)->get_position() - vector<int>{0, 1, 0});
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
		case 'j': {
			eye = eye - cross(direction, up) * speed;
			break;
		}
		case 'l': {
			eye = eye + cross(direction, up) * speed;
			break;
		}
		case 'i': {
			eye = eye + direction * speed;
			break;
		}
		case 'k': {
			eye = eye - direction * speed;
			break;
		}
		case 'z': {
			zoomAngle += 2.0f;
			if (zoomAngle > 60.0f) zoomAngle = 60.0f;
			break;
		}
		case 'c': {
			zoomAngle -= 2.0f;
			if (zoomAngle < 15.0f) zoomAngle = 15.0f;

			break;
		}
		case 27: {
			exit(0);
		}
		default:
			break;
		}
	}

	void mouseClick(int button, int state, int x, int y) {
		
	}

	void mouseMotion(int x, int y) {
		int xoffset, yoffset;
		xoffset = x - lastmouseX;
		yoffset = y - lastmouseY;
		if (x < 0.05 * window_size[0] || x > 0.95 * window_size[0])
		{
			lastmouseX = window_size[0] / 2;
			glutWarpPointer(window_size[0] / 2, y);
		}

		if (y < 0.05 * window_size[1] || y > 0.95 * window_size[1]) {
			lastmouseY = window_size[1] / 2;
			glutWarpPointer(x, window_size[1] / 2);
		}
		lastmouseX = x;
		lastmouseY = y;
		xoffset *= 0.5f;
		yoffset *= 0.5f;
		//std::cout << xoffset << "  " << yoffset << std::endl;
		if (xoffset > window_size[0] / 8 || xoffset < -window_size[0] / 8) return;
		if (yoffset > window_size[1] / 8 || yoffset < -window_size[1] / 8) return;
		yaw += xoffset;
		pitch -= yoffset;
		//std::cout << "yaw" << yaw << "pitch" << pitch << std::endl;
		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;
		if (yaw > 360.0f) yaw -= 360.0f;
		if (yaw < -360.0f) yaw += 360.0f;
		direction[0] = cos(pitch / 180.0 * 3.14159) * cos(yaw / 180.0 * 3.14159);
		direction[1] = sin(pitch / 180.0 * 3.14159);
		direction[2] = cos(pitch / 180.0 * 3.14159) * sin(yaw / 180.0 * 3.14159);
		direction = normalize(direction);
		//std::cout << direction;
	}
}