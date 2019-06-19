#include "model.h"

#include <vector>

namespace Sokoban {

	Object* map[10][10];
	vector<float> eye = {0, 0, 8};
	
	vector<float> center = { 0, 0, 0 };
	std::vector<float> asdf;
	void init_map()
	{
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				map[i][j] = new SolidCube;
			}
		}
	}

}