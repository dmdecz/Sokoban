#include "model.h"

#include <vector>

namespace Sokoban {

	Map map;
	vector<float> eye;
	vector<float> direction;
	vector<float> up;
	vector<int> window_size;
	int windowHandle;
	float zoomAngle;
	float eye_h;

	// textures
	const int texture_count = 7;
	vector<GLuint> textures(texture_count);
	vector<bmpImage*> texture_images({
		new bmpImage("res/solidcube.bmp"),
		new bmpImage("res/floor.bmp"),
		new bmpImage("res/wall.bmp"),
		new bmpImage("res/dst.bmp"),
		new bmpImage("res/complete.bmp"),
		new bmpImage("res/sky.bmp"),
		new bmpImage("res/border.bmp")
	});

	// display list
	const int display_count = 5;
	vector<GLuint> display_list(display_count);

	// view
	bool light_mode;
	int sleep_cnt;

	void init_paras()
	{
		init_ctrl_paras();
		init_model_paras();
		init_view_paras();
	}

	void init_model_paras()
	{
		eye_h = 0.8;
		eye = Sokoban::map.real_position({ 0, 0, eye_h });
		direction = { 0, 0, -1 };
		up = { 0, 1, 0 };

		window_size = { 700, 700 };
		windowHandle = 0;

		zoomAngle = 60.0f;

	}

	void init_map()
	{
		Sokoban::map.reload();
	}

	void init_texture()
	{
		texture_images[DST]->setTransparent(127);

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
			
		glEndList();

		// floor
		glNewList(display_list[Floor_Disp], GL_COMPILE);
			Map::register_disp_floor();
		glEndList();

		// wall
		glNewList(display_list[Wall_Disp], GL_COMPILE);
			WallCube::register_disp();
		glEndList();

		// dst
		glNewList(display_list[DST_Disp], GL_COMPILE);
			DstCube::register_disp();
		glEndList();

		// complete
		glNewList(display_list[Complete_Disp], GL_COMPILE);
			//CompleteCube::register_disp();
		glEndList();
	}

	void screen_shot()
	{
		int width = window_size[0] % 4 ? (window_size[0] + 4 - window_size[0] % 4) : window_size[0];
		uchar* screen_data = new uchar[width * window_size[1] * 3];
		glReadPixels(0, 0, width, window_size[1], GL_RGB, GL_UNSIGNED_BYTE, screen_data);
		bmpImage screen(width, window_size[1], screen_data);
		screen.write_back("test.bmp");
		delete[] screen_data;
	}

}