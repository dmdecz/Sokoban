#include "view.h"
#include "../model/model.h"

#include <iostream>


namespace Sokoban
{
	void init_texture()
	{
		glGenTextures(texture_count, textures.data());
		// init texture for each image
		for (int i=0; i< texture_count; i++)
		{
			texture_images[i]->initTexture(textures[i]);
		}
	}

	void display()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		gluLookAt(
			eye[0], eye[1], eye[2],
			center[0], center[1], center[2],
			0, 1, 0
		);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);

		glLightfv(GL_LIGHT0, GL_POSITION, eye.data());
		glLightfv(GL_LIGHT0, GL_AMBIENT, vector<float>{1, 1, 1, 1}.data());
		glEnable(GL_LIGHT0);

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				map[i][j]->draw();
			}
		}
		getFPS();
		glutSwapBuffers();
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
		gluPerspective(45.0f, (GLfloat)width / height, 0.1f, 100.0f);

		glMatrixMode(GL_MODELVIEW);
	}

	void idle()
	{
		glutPostRedisplay();
	}

	void getFPS()
	{
		static int frame = 0, time, timebase = 0;
		static char buffer[256];

		char mode[64];

		frame++;
		time = glutGet(GLUT_ELAPSED_TIME);
		if (time - timebase > 1000) {
			sprintf_s(buffer, "FPS:%4.2f",
				frame * 1000.0 / (time - timebase));
			timebase = time;
			frame = 0;
		}

		char* c;
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, 480, 0, 480, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glRasterPos2f(10, 10);
		for (c = buffer; *c != '\0'; c++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
		}
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glEnable(GL_DEPTH_TEST);
	}
}