#include "control.h"
#include "../model/model.h"
#include "../bmp.h"

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
			//std::cout << "hits " << hits << std::endl;
			glMatrixMode(GL_MODELVIEW);
			glutPostRedisplay();
			reshape(window_size[0], window_size[1]);

			if (hits == 0) return;
			unsigned int i;
			GLuint name, minz, maxz, min, minName;
			bool flag = false;
			for (i = 0; i < hits; i++)
			{
				minz = selectBuffer[1 + i * 4];
				maxz = selectBuffer[2 + i * 4];
				name = selectBuffer[3 + i * 4];
				if (!flag) {
					min = minz;
					minName = name;
					flag = true;
					if (name == 10000) {
						flag = false;
					}
				} else {
					if (minz < min && name < map.get_size()[1] * map.get_size()[0] * 6) {
						min = minz;
						minName = name;
					}
				}
				//std::cout << "name" << name << "minz" << minz << "maxz" << maxz << std::endl;
			}
			//std::cout << "result " << minName << std::endl;
			box_move(minName);
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

		vector<int> end_cube_x = { int(end_x), int(floor(map_p[1])) };
		vector<int> end_cube_y = { int(floor(map_p[0])), int(end_y) };
		cout << end_cube_x << endl;
		cout << end_cube_y << endl;

		Object* cube_x = map.get_object(end_cube_x[0], end_cube_x[1]);
		if (!cube_x || !cube_x->can_enter() && end_cube_x[0] != floor(map_p[0])) {
			map_d[0] = 0;
		}
		Object* cube_y = map.get_object(end_cube_y[0], end_cube_y[1]);
		if (!cube_y || !cube_y->can_enter() && end_cube_y[1] != floor(map_p[1])) {
			map_d[1] = 0;
		}
		direction = map.real_position(map_d);
		eye = eye + direction * speed;
	}

	void box_move(GLuint name)
	{
		int face = name % 6;
		int x = (name / 6) % map.get_size()[0];
		int y = (name / 6) / map.get_size()[0];
		cout << "[" << x << "]" << "[" << y << "]" << " " << face << endl;
		Object* move_cube = map.get_object(x, y);
		if (!move_cube || !move_cube->is_movable()) {
			return;
		}
		Object* end_cube = nullptr;
		vector<int> end_p = { -1, -1, 0 };
		if (face == 2) {
			end_p = { x + 1, y, 0 };
		} else if (face == 3) {
			end_p = { x - 1, y, 0 };
		} else if (face == 4) {
			end_p = { x, y + 1, 0 };
		} else if (face == 5) {
			end_p = { x, y - 1, 0 };
		}
		end_cube = map.get_object(end_p[0], end_p[1]);
		if (end_cube && end_cube->can_enter()) {
			ObjectID move_id = move_cube->getID();
			ObjectID end_id = end_cube->getID();
			if (move_id == CUBE_ID && end_id == DST_ID) {
				delete map.get_object(x, y);
				delete map.get_object(end_p[0], end_p[1]);
				map.set_object(new CompleteCube({x, y, 0}), x, y);
				map.set_object(new EmptyCube(end_p), end_p[0], end_p[1]);
				move_cube = map.get_object(x, y);
			} else if (move_id == COMPLETE_ID && end_id == EMPTY_ID) {
				delete map.get_object(x, y);
				delete map.get_object(end_p[0], end_p[1]);
				map.set_object(new SolidCube({ x, y, 0 }), x, y);
				map.set_object(new DstCube(end_p), end_p[0], end_p[1]);
				move_cube = map.get_object(x, y);
			}
			
			move_cube->move_to(end_p[0], end_p[1]);
		}
	}
}