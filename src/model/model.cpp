#include "model.h"
#include "../view/view.h"

#include <vector>

namespace Sokoban {

	Object* map[10][10];
	vector<Object*> object_list;
	vector<float> eye = {0, 0, 8};
	vector<float> center = { 0, 0, 0 };

	void init_map()
	{
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				map[i][j] = new SolidCube({i, j, 0});
				object_list.push_back(map[i][j]);
			}
		}
	}

	void SolidCube::draw()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(position[0] - move[0], position[1] - move[1], position[2] - move[2]);
		move_once();

		// enable texture
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Texture::textures[Texture::TextureID::BOX]);
		// set texture coordinary for every surface
		GLint borderPoint[4][2] = {
			{1, 1}, {1, 0}, {0, 0}, {0, 1}
		};
		GLfloat width = 0.5;
		GLfloat half = width / 2;
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
			glBegin(GL_QUADS);
			for (int i = 0; i < 6; i++)
				for (int j = 0; j < 4; j++)
				{
					glTexCoord2iv(borderPoint[j]);
					glNormal3fv(normal[i]);
					glVertex3fv(cubeVertex[i][j]);
				}
			glEnd();
		glPopMatrix();

		// disable texture
		glDisable(GL_TEXTURE_2D);

		//glutSolidCube(1);
		glPopMatrix();
	}

}