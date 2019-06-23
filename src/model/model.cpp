#include "model.h"

#include <vector>

namespace Sokoban {

	Map map(10, 10);
	vector<float> eye;
	vector<float> direction;
	vector<float> up;
	vector<int> window_size;
	int windowHandle;
	float zoomAngle;

	// textures
	const int texture_count = 1;
	enum TextureID {
		BOX
	};
	vector<GLuint> textures(texture_count);
	vector<bmpImage*> texture_images({
		new bmpImage("res/Crack.bmp"),
	});

	// display list
	const int display_count = 2;
	enum DisplayID {
		BOX_Disp, Floor_Disp
	};
	vector<GLuint> display_list(display_count);

	void init_paras()
	{
		init_ctrl_paras();
		init_model_paras();
		init_view_paras();
	}

	void init_model_paras()
	{
		//map = Map(1, 10);

		eye = { 0.0f, 0.0f, 10.0f };
		direction = { 0, 0, -1 };
		up = { 0, 1, 0 };

		window_size = { 640, 640 };
		windowHandle = 0;

		zoomAngle = 45.0f;
	}

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

	void init_texture()
	{
		glGenTextures(texture_count, textures.data());
		// init texture for each image
		for (int i = 0; i < texture_count; i++)
		{
			texture_images[i]->initTexture(textures[i]);
		}
	}

	void init_display_list()
	{
		// get list ID
		display_list[0] = glGenLists(display_count);
		for (int i = 1; i < display_count; i++)
			display_list[i] = display_list[i - 1] + 1;

		// solid cube
		glNewList(display_list[BOX_Disp], GL_COMPILE);
			SolidCube::register_disp();
		glEndList();

		// floor
		glNewList(display_list[Floor_Disp], GL_COMPILE);
			Map::register_disp_floor();
		glEndList();
	}

	void Map::register_disp_floor()
	{
		float half = Sokoban::map.cube_len / 2;
		glPushMatrix();
		glTranslatef(0, 0, -half);

		// enable texture
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[TextureID::BOX]);
		// set texture coordinary
		GLint borderPoint[4][2] = {
			{1, 1}, {1, 0}, {0, 0}, {0, 1}
		};
		
		// set real position vertex
		GLfloat Vertex[4][3] = {
			{ half,  half, 0}, { half, -half, 0}, {-half, -half, 0}, {-half,  half, 0}
		};
		GLfloat normal[3] = { 0, 0, 1 };

		glBegin(GL_QUADS);
		for (int k = 0; k < 4; k++)
		{
			glTexCoord2iv(borderPoint[k]);
			glNormal3fv(normal);
			glVertex3fv(Vertex[k]);
		}
		glEnd();

		// disable texture
		glDisable(GL_TEXTURE_2D);

		glPopMatrix();
	}

	void Map::drawFloor() const
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		
		for(int i = 0; i < size[1]; i++)
			for (int j = 0; j < size[0]; j++)
			{
				glPushMatrix();
				glTranslatef(j, i, 0);

				glCallList(Sokoban::display_list[Floor_Disp]);

				glPopMatrix();
			}

		glPopMatrix();
	}

	void SolidCube::register_disp()
	{
		glMatrixMode(GL_MODELVIEW);

		float ambient[]	= { 0.2f, 0.0f, 0.0f, 1.0f };
		float specular[]= { 1.0f, 0.0f, 0.0f, 1.0f };
		float diffuse[]	= { 1.0f, 0.0f, 0.0f, 1.0f };

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

		glBegin(GL_QUADS);
		for (int i = 0; i < 6; i++)
			for (int j = 0; j < 4; j++)
			{
				//glTexCoord2iv(borderPoint[j]);
				glNormal3fv(normal[i]);
				glVertex3fv(cubeVertex[i][j]);
			}
		glEnd();

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

		glCallList(Sokoban::display_list[BOX_Disp]);

		glPopMatrix();
	}

}