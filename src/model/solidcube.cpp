#include "model.h"
using namespace Sokoban;

SolidCube::SolidCube(const vector<int>& p):
	Object(CUBE_ID), position(p)
{
	move = { 0, 0, 0 };
}

void SolidCube::move_once()
{
	if (fabs(move[0]) > 1e-4)
		move[0] += (move[0] < 0) ? 0.01f : -0.01f;
	if (fabs(move[1]) > 1e-4)
		move[1] += (move[1] < 0) ? 0.01f : -0.01f;
	if (fabs(move[2]) > 1e-4)
		move[2] += (move[2] < 0) ? 0.01f : -0.01f;
	if (fabs(move[0]) < 1e-4 && fabs(move[1]) < 1e-4 && fabs(move[2]) < 1e-4)
		moving = false;
}

void SolidCube::move_to(vector<int> end)
{
	if (moving)
		return;
	assert(end.size() == 3);
	Sokoban::map.set_object(nullptr, position[0], position[1], 0);
	move[0] = (end[0] - position[0]) * 1.0;
	move[1] = (end[1] - position[1]) * 1.0;
	move[2] = (end[2] - position[2]) * 1.0;
	position = end;
	Sokoban::map.set_object(this, position[0], position[1], 0);
	moving = true;
}

void SolidCube::draw_directly()
{
	glMatrixMode(GL_MODELVIEW);

	float ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);

	glPushMatrix();

	// enable texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[TextureID::BOX]);
	// set texture coordinary for every surface
	GLint borderPoint[4][2] = {
		{1, 1}, {1, 0}, {0, 0}, {0, 1}
	};
	GLfloat half = Sokoban::map.get_cube_len() / 2;
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

	
	for (int i = 0; i < 6; i++) {
		GLuint name = (position[1] * Sokoban::map.get_size()[0] + position[0]) * 6 + i;
		glLoadName(name);
		glBegin(GL_QUADS);
		for (int j = 0; j < 4; j++)
		{
			glTexCoord2iv(borderPoint[j]);
			glNormal3fv(normal[i]);
			glVertex3fv(cubeVertex[i][j]);
		}
		glEnd();
	}

	glPopMatrix();

	// disable texture
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void SolidCube::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(position[0] - move[0], position[1] - move[1], position[2] - move[2]);
	move_once();

	//glCallList(Sokoban::display_list[BOX_Disp]);
	draw_directly();

	glPopMatrix();
}
