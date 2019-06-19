#include "view/view.h"
#include "model/model.h"
#include "glut.h"

#include <iostream>

namespace Sokoban {
	void Sokoban(int* argc, char** argv);
}

int main(int argc, char **argv)
{
	std::cout << "Hello World!" << std::endl;
	Sokoban::Sokoban(&argc, argv);
}

void Sokoban::Sokoban(int *argc, char** argv)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480, 480);

	int window_handle = glutCreateWindow("Sokoban");
	Sokoban::init_map();

	glutDisplayFunc(Sokoban::sokoban_display);
	glutReshapeFunc(Sokoban::sokoban_reshape);
	//glutKeyboardFunc();
	glutIdleFunc(sokoban_idle);

	glutMainLoop();
}