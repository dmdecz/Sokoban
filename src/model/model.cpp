#include "model.h"

#include <vector>

namespace Sokoban {

	Map map(10, 10);
	vector<float> eye = {0.0f, 0.0f, 10.0f};
	vector<float> direction = { 0, 0, -1 };
	vector<float> up = { 0, 1, 0 };
	vector<int> window_size = { 640, 640 };
	int windowHandle;
	float zoomAngle = 45.0f;

	// textures
	const int texture_count = 1;
	enum TextureID {
		BOX
	};
	vector<GLuint> textures(texture_count);
	vector<bmpImage*> texture_images({
		new bmpImage("res/Crack.bmp"),
		});

	void init_map()
	{
		//for (int i = 0; i < 10; i++)
		//{
		//	for (int j = 0; j < 10; j++)
		//	{
		//		map[i][j] = new SolidCube(map, {i, 0, -j});
		//		object_list.push_back(map[i][j]);
		//	}
		//}
	}

	void Map::drawFloor()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		// enable texture
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[TextureID::BOX]);
		// set texture coordinary
		GLint borderPoint[4][2] = {
			{1, 1}, {1, 0}, {0, 0}, {0, 1}
		};
		double unitWidth = 1;
		double half = unitWidth / 2;
		// set real position vertex
		GLfloat Vertex[4][3] = {
			{ half,  half, 0}, { half, -half, 0}, {-half, -half, 0}, {-half,  half, 0}
		};
		GLfloat normal[3] = { 0, 0, 1 };

		for(int i=0; i<10; i++)
			for (int j = 0; j < 10; j++)
			{
				glPushMatrix();
				vector<float> real_position = this->real_position({ float(i), float(j), 0 });
				glTranslatef(real_position[0], real_position[1], real_position[2]);

				glBegin(GL_QUADS);
				for(int k=0; k<4; k++)
				{
					glTexCoord2iv(borderPoint[k]);
					glNormal3fv(normal);
					glVertex3fv(Vertex[k]);
				}
				glEnd();
				glPopMatrix();
			}

		// disable texture
		glDisable(GL_TEXTURE_2D);

		glPopMatrix();
	}

	void SolidCube::draw()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		vector<float> real_position = map->real_position({ position[0] - move[0], position[1] - move[1], position[2] - move[2] });
		glTranslatef(real_position[0], real_position[1], real_position[2]);
		move_once();

		//// enable texture
		//glEnable(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D, textures[TextureID::BOX]);
		//// set texture coordinary for every surface
		//GLint borderPoint[4][2] = {
		//	{1, 1}, {1, 0}, {0, 0}, {0, 1}
		//};
		//GLfloat width = 0.5;
		//GLfloat half = width / 2;
		//GLfloat cubeVertex[6][4][3] = {
		//	{ { half,  half,  half}, {-half,  half,  half}, {-half, -half,  half}, { half, -half,  half} },
		//	{ { half,  half, -half}, { half, -half, -half}, {-half, -half, -half}, {-half,  half, -half} },
		//	{ {-half, -half,  half}, {-half, -half, -half}, {-half,  half, -half}, {-half,  half,  half} },
		//	{ { half,  half,  half}, { half, -half,  half}, { half, -half, -half}, { half,  half, -half} },
		//	{ { half, -half,  half}, { half, -half, -half}, {-half, -half, -half}, {-half, -half,  half} },
		//	{ {-half,  half,  half}, { half,  half,  half}, { half,  half, -half}, {-half,  half, -half} }
		//};
		//GLfloat normal[6][3] = {
		//	{ 0,  0,  1},
		//	{ 0,  0, -1},
		//	{-1,  0,  0},
		//	{ 1,  0,  0},
		//	{ 0, -1,  0},
		//	{ 0,  1,  0}
		//};
		//glPushMatrix();
		//	glBegin(GL_QUADS);
		//	for (int i = 0; i < 6; i++)
		//		for (int j = 0; j < 4; j++)
		//		{
		//			glTexCoord2iv(borderPoint[j]);
		//			glNormal3fv(normal[i]);
		//			glVertex3fv(cubeVertex[i][j]);
		//		}
		//	glEnd();
		//glPopMatrix();

		//// disable texture
		//glDisable(GL_TEXTURE_2D);

		glutSolidCube(0.5);
		glPopMatrix();
	}

}