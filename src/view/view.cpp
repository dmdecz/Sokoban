#include "view.h"
#include "glut.h"

void sokoban_display()
{

}

void sokoban_reshape(int width, int height)
{
	if (height == 0)
		height = 1;
	sokoban_update_view(width, height);
}

void sokoban_update_view(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
}

void sokoban_idle()
{
	glutPostRedisplay();
}
