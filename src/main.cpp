#include "view/view.h"
#include "model/model.h"
#include "control/control.h"
#include "glut.h"
#include "windows.h"

#include <iostream>

namespace Sokoban {
	void Sokoban(int *argc, char **argv);
}

int main(int argc, char **argv)
{
	//ShowCursor(false);
	std::cout << "Hello World!" << std::endl;
	//std::cout << glGetString(GL_VERSION) << std::endl;
	printf("%s\n", glGetString(GL_VERSION));
	Sokoban::Sokoban(&argc, argv);
}

void Sokoban::Sokoban(int *argc, char** argv)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(Sokoban::window_size[0], Sokoban::window_size[1]);

	int window_handle = glutCreateWindow("Sokoban");
	Sokoban::init_map();
	Sokoban::init_texture();

	glutDisplayFunc(Sokoban::display);
	glutReshapeFunc(Sokoban::reshape);
	glutKeyboardFunc(Sokoban::keyboard);
	glutIdleFunc(idle);

	glutMainLoop();
}
