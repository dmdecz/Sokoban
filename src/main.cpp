#include "view/view.h"
#include "model/model.h"
#include "control/control.h"
#include "glut.h"
#include "windows.h"

#include <iostream>

namespace Sokoban
{
	void Sokoban(int *argc, char **argv);
}

int main(int argc, char **argv)
{
	ShowCursor(false);
	Sokoban::Sokoban(&argc, argv);
}

void Sokoban::Sokoban(int *argc, char** argv)
{
	// glut init
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	// init paras
	Sokoban::init_paras();
	cout << window_size << endl;

	// init window
	glutInitWindowSize(Sokoban::window_size[0], Sokoban::window_size[1]);
	Sokoban::windowHandle = glutCreateWindow("Sokoban");

	// init map and texture
	Sokoban::init_texture();
	Sokoban::init_map();
	Sokoban::init_display_list();

	// register events
	glutDisplayFunc(Sokoban::display);
	glutReshapeFunc(Sokoban::reshape);
	glutWarpPointer(Sokoban::window_size[0] / 2, Sokoban::window_size[1] / 2);
	glutMouseFunc(Sokoban::mouseClick);
	glutMotionFunc(Sokoban::mouseMotion);
	glutPassiveMotionFunc(Sokoban::mouseMotion);
	glutKeyboardFunc(Sokoban::keyboard);
	glutIdleFunc(idle);

	// begin main loop
	glutMainLoop();
}
