#include "view.h"
#include "../model/model.h"

#include <iostream>


namespace Sokoban
{
	void init_texture()
	{
		glGenTextures(texture_count, textures.data());
		// init texture for each image
		for (int i = 0; i< texture_count; i++)
		{
			texture_images[i]->initTexture(textures[i]);
		}
	}

	void display()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		vector<float> center;
		center = eye + direction;
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
		glLightfv(GL_LIGHT0, GL_AMBIENT, vector<float>{1, 1, 1, 1}.data());
		glEnable(GL_LIGHT0);

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (map.get_object(i, j)) {
					map.get_object(i, j)->draw();
				}
			}
		}
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
		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos2f(10, 10);
		char buffer[256];
		sprintf_s(buffer, "FPS:%4.2f", getFPS());
		for (char *c = buffer; *c != '\0'; c++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
		}
		glColor3f(0.0f, 1.0f, 0.0f);
		glRasterPos2f(window_size[0] / 2, window_size[1] / 2);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '+');
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glEnable(GL_DEPTH_TEST);
	}

	void reshape(int width, int height)
	{
		if (height == 0)
			height = 1;
		window_size[0] = width;
		window_size[1] = height;
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