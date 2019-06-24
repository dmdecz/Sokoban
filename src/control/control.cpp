#include "control.h"
#include "../model/model.h"
#include "../bmp.h"
#include "../glut.h"
#include "..//view/view.h"

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

		speed = 0.1f;

		yaw = 270.0f;
		pitch = 0.0f;

		mouseAdjust = false;
	}

	void keyboard(unsigned char key, int  x, int y)
	{
		switch (key)
		{
		//case 'a':
		//	//map.get_object(0, 0, 0)->move_to(map.get_object(0, 0, 0)->get_position() - vector<int>{1, 0, 0});
		//	break;
		//case 'd':
		//	//map.get_object(0, 0, 0)->move_to(map.get_object(0, 0, 0)->get_position() + vector<int>{1, 0, 0});
		//	break;
		//case 'w':
		//	//map.get_object(0, 0, 0)->move_to(map.get_object(0, 0, 0)->get_position() + vector<int>{0, 1, 0});
		//	break;
		//case 's':
		//	//map.get_object(0, 0, 0)->move_to(map.get_object(0, 0, 0)->get_position() - vector<int>{0, 1, 0});
		//	break;
		case ' ': {
			screen_shot();
			break;
		}
		case 'a': {
			player_move(-cross(direction, up));
			break;
		}
		case 'd': {
			player_move(cross(direction, up));
			break;
		}
		case 'w': {
			player_move(direction);
			break;
		}
		case 's': {
			player_move(-direction);
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
		case '\r': {
			init_paras();
			Sokoban::map.reload();
		}
		default:
			break;
		}
	}

	void mouseClick(int button, int state, int x, int y)
	{
		GLuint selectBuffer[32];
		GLint hits;
		GLint viewport[4];
		if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
		{
			glGetIntegerv(GL_VIEWPORT, viewport);
			glSelectBuffer(32, selectBuffer);
			glRenderMode(GL_SELECT);
			glInitNames();
			glPushName(0);
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			gluPickMatrix(x, viewport[3] - y, 2.0, 2.0, viewport);
			glOrtho(-10, 10, -10, 10, 0, 2);
			glMatrixMode(GL_MODELVIEW);
			map.draw();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glFlush();
			hits = glRenderMode(GL_RENDER);
			std::cout << "hits " << hits << std::endl;
			glMatrixMode(GL_MODELVIEW);
			glutPostRedisplay();
			reshape(window_size[0], window_size[1]);

			if (hits == 0) return;
			unsigned int i;
			GLuint name, minz, maxz, min, minName;
			for (i = 0; i < hits; i++)
			{
				minz = selectBuffer[1 + i * 4];
				maxz = selectBuffer[2 + i * 4];
				name = selectBuffer[3 + i * 4];
				if (i == 0) {
					min = minz;
					minName = name;
				}
				else {
					if (minz < min) {
						min = minz;
						minName = name;
					}
				}
				std::cout << "name" << name << "minz" << minz << "maxz" << maxz << std::endl;
			}
			std::cout << "result" << minName << std::endl;
		}
	}

	void mouseMotion(int x, int y)
	{
		if (x == window_size[0] / 2 && y == window_size[1] / 2) {
			return;
		}
		int xoffset, yoffset;
		xoffset = x - window_size[0] / 2;
		yoffset = y - window_size[1] / 2;
		xoffset *= 0.5f;
		yoffset *= 0.5f;

		if (xoffset > window_size[0] / 8 || xoffset < -window_size[0] / 8) return;
		if (yoffset > window_size[1] / 8 || yoffset < -window_size[1] / 8) return;
		yaw += xoffset;
		pitch -= yoffset;

		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;
		if (yaw > 360.0f) yaw -= 360.0f;
		if (yaw < -360.0f) yaw += 360.0f;
		direction[0] = cos(pitch / 180.0 * 3.14159) * cos(yaw / 180.0 * 3.14159);
		direction[1] = sin(pitch / 180.0 * 3.14159);
		direction[2] = cos(pitch / 180.0 * 3.14159) * sin(yaw / 180.0 * 3.14159);
		direction = normalize(direction);
		glutWarpPointer(window_size[0] / 2, window_size[1] / 2);
	}

	void player_move(vector<float> direction)
	{
		direction[1] = 0;
		vector<float> map_p = map.map_position(eye) + vector<float>{map.get_cube_len() / 2, map.get_cube_len() / 2, 0};
		vector<float> map_d = map.map_position(direction);
		float end_x = map_p[0] + map_d[0];
		float end_y = map_p[1] + map_d[1];
		vector<int> end_cube = { int(end_x), int(end_y) };
		cout << map_p << endl;
		cout << floor(map_p[0]) << " " << floor(map_p[1]) << endl;
		cout << end_cube << endl;
		if (map.get_object(end_cube[0], end_cube[1]) && !map.get_object(end_cube[0], end_cube[1])->can_enter()) {
			if (floor(map_p[0]) != end_cube[0]) {
				map_d[0] = 0;
			}
			if (floor(map_p[1]) != end_cube[1]) {
				map_d[1] = 0;
			}
		}
		direction = map.real_position(map_d);
		eye = eye + direction * speed;
	}
}