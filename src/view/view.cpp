#include "view.h"
#include "../model/model.h"

#include <iostream>


namespace Sokoban
{
	void init_view_paras()
	{
		light_mode = true;
		sleep_cnt = 0;
	}

	void display()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		vector<float> center = eye + direction;
		gluLookAt(
			eye[0], eye[1], eye[2],
			center[0], center[1], center[2],
			up[0], up[1], up[2]
		);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);

		vector<float> light_position = eye;
		light_position.push_back(1);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position.data());
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction.data());
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, vector<float>{1, 1, 1, 1}.data());
		glLightfv(GL_LIGHT0, GL_DIFFUSE, vector<float>{1, 1, 1, 1}.data());
		glLightfv(GL_LIGHT0, GL_SPECULAR, vector<float>{1, 1, 1, 1}.data());
		if (!light_mode) {
			glEnable(GL_LIGHT0);
		} else {
			glDisable(GL_LIGHT0);
		}

		glLightfv(GL_LIGHT1, GL_POSITION, light_position.data());
		glLightfv(GL_LIGHT1, GL_AMBIENT, vector<float>{0.2, 0.2, 0.2, 1}.data());
		glLightfv(GL_LIGHT1, GL_DIFFUSE, vector<float>{0.2, 0.2, 0.2, 1}.data());
		glLightfv(GL_LIGHT1, GL_SPECULAR, vector<float>{1, 1, 1, 1}.data());
		glEnable(GL_LIGHT1);
		if (light_mode) {
			glEnable(GL_LIGHT1);
		} else {
			glDisable(GL_LIGHT1);
		}

		// draw objects
		map.draw();
		display_bitmap();
		glutSwapBuffers();
	}

	void display_bitmap()
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, window_size[0], 0, window_size[1], -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		// FPS
		char buffer[256];
		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos2f(10, 10);
		sprintf_s(buffer, "FPS:%4.2f", getFPS());
		for (char *c = buffer; *c != '\0'; c++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
		}
		// star
		glColor3f(0.0f, 1.0f, 0.0f);
		glRasterPos2f(window_size[0] / 2, window_size[1] / 2);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '+');
		// game information
		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos2f(window_size[0] - 150, 10);
		sprintf_s(buffer, "stage %d steps %d", map.get_map_number(), map.get_step_number());
		for (char* c = buffer; *c != '\0'; c++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
		}
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glEnable(GL_DEPTH_TEST);
	}

	void reshape(int width, int height)
	{
		//cout << width << 'x' << height << endl;
		//cout << window_size << endl;
		if (height == 0)
			height = 1;
		window_size[0] = width;
		window_size[1] = height;
		//cout << window_size << endl;
		update_view(width, height);
	}

	void update_view(int width, int height)
	{
		glViewport(0, 0, width, height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(zoomAngle, (GLfloat)width / height, 0.1f, 100.0f);

		glMatrixMode(GL_MODELVIEW);
	}

	void idle()
	{
		if (!sleep_cnt)
			;
		else if (sleep_cnt < 1000)
			sleep_cnt++;
		else
		{
			Sokoban::map.to_next_map();
			sleep_cnt = 0;
		}
		glutPostRedisplay();
	}

	double getFPS()
	{
		static int frame = 0;
		static double time, timebase = 0;
		static double fps;

		frame++;
		time = glutGet(GLUT_ELAPSED_TIME);
		if (time - timebase > 1000) {
			fps = frame * 1000.0 / (time - timebase);
			timebase = time;
			frame = 0;
		}
		return fps;
	}
}