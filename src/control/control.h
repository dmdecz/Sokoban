#pragma once
#include <vector>
#include <math.h>

namespace Sokoban {
	using std::vector;

	extern float speed;

	void keyboard(unsigned char key, int  x, int y);

	void mouseClick(int button, int state, int x, int y);

	void mouseMotion(int x, int y);

	void player_move(vector<float> direction);

	template <class T>
	vector<T> cross(vector<T> v1, vector<T> v2) {
		vector<T> ret;
		ret.push_back(v1[1] * v2[2] - v1[2] * v2[1]);
		ret.push_back(v1[2] * v2[0] - v1[0] * v2[2]);
		ret.push_back(v1[0] * v2[1] - v1[1] * v2[0]);
		return ret;
	}

	template <class T>
	vector<T> normalize(vector<T> v) {
		vector<T> ret;
		float para = 0.0f;
		int size = v.size();
		for (int i = 0; i < size; i++) {
			para += v[i] * v[i];
		}
		para = sqrt(para);
		for (int i = 0; i < size; i++) {
			ret.push_back(v[i] / para);
		}
		return ret;
	}
}
