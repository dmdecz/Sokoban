#include "model.h"
using namespace Sokoban;

DstCube::DstCube(const vector<int>& p) :
	Object(DST_ID), position(p)
{

}

void DstCube::register_disp()
{
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[TextureID::DST]);

	// set texture coordinary for every surface
	GLint borderPoint[4][2] = {
		{1, 1}, {1, 0}, {0, 0}, {0, 1}
	};
	GLfloat half = Sokoban::map.get_cube_len() / 2 - 0.001;
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
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	glPopMatrix();
}

void DstCube::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(position[0], position[1], position[2]);

	glCallList(Sokoban::display_list[DST_Disp]);

	glPopMatrix();
}
