#include "view/view.h"
#include "view/glut.h"

#include <iostream>

void Sokoban(int argc, char** argv);

int main(int argc, char **argv)
{
	std::cout << "Hello World!" << std::endl;
}

void Sokoban(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480, 480);

	int window_handle = glutCreateWindow("Sokoban");

	glutDisplayFunc(sokoban_display);
	glutReshapeFunc(sokoban_reshape);
	glutKeyboardFunc();
	glutIdleFunc(sokoban_idle);

	glutMainLoop();
}