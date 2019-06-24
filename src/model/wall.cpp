#include "model.h"
using namespace Sokoban;

WallCube::WallCube(const vector<int>& p) :
	Object(WALL_ID), position(p)
{

}

void WallCube::register_disp()
{
	glMatrixMode(GL_MODELVIEW);

	float ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);

	glPushMatrix();

	// enable texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[TextureID::Wall]);
	// set texture coordinary for every surface
	GLint borderPoint[4][2] = {
		{1, 1}, {1, 0}, {0, 0}, {0, 1}
	};
	GLfloat half = Sokoban::map.get_cube_len() / 4;
	GLfloat cubeVertex[6][4][3] = {
		{ { half,  half,  half}, {-half,  half,  half}, {-half, -half,  half}, { half, -half,  half} },
		{ { half,  half, -half}, { half, -half, -half}, {-half, -half, -half}, {-half,  half, -half} },
		{ {-half, -half,  half}, {-half, -half, -half}, {-half,  half, -half}, {-half,  half,  half} },
		{ { half,  half,  half}, { half, -half,  half}, { half, -half, -half}, { half,  half, -half} },
		{ { half, -half,  half}, { half, -half, -half}, {-half, -half, -half}, {-half, -half,  half} },
		{ {-half,  half,  half}, { half,  half,  half}, { half,  half, -half}, {-half,  half, -half} }
	};
	GLfloat normal[6][3] = {
		{ 0,  0,  1},
		{ 0,  0, -1},
		{-1,  0,  0},
		{ 1,  0,  0},
		{ 0, -1,  0},
		{ 0,  1,  0}
	};
	glPushMatrix();

	glLoadName(10000);
	glBegin(GL_QUADS);
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 4; j++)
		{
			glTexCoord2iv(borderPoint[j]);
			glNormal3fv(normal[i]);
			glVertex3fv(cubeVertex[i][j]);
		}
	}
	glEnd();

	glPopMatrix();

	// disable texture
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void WallCube::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(position[0], position[1], position[2]);

	glCallList(Sokoban::display_list[Wall_Disp]);

	glPopMatrix();
}
