#include "view.h"
#include "../model/model.h"
#include "../glut.h"

#include <iostream>

namespace Sokoban {

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
		glutSwapBuffers();
	}

	void reshape(int width, int height)
	{
		if (height == 0)
			height = 1;
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

}