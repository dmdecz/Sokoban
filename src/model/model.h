#pragma once

#include "../glut.h"

#include <cassert>
#include <vector>

namespace Sokoban {
	using std::vector;

	template <class T>
	const vector<T> operator+(const vector<T>& v1, const vector<T>& v2)
	{
		assert(v1.size() == v2.size());
		vector<T> ret;
		int size = v1.size();
		for (int i = 0; i < size; i++)
			ret.push_back(v1[i] + v2[i]);
		return ret;
	}

	template <class T>
	const vector<T> operator-(const vector<T>& v1, const vector<T>& v2)
	{
		assert(v1.size() == v2.size());
		vector<T> ret;
		int size = v1.size();
		for (int i = 0; i < size; i++)
			ret.push_back(v1[i] - v2[i]);
		return ret;
	}

	class Object
	{
	public:
		virtual void draw() = 0;
	};

	extern Object* map[10][10];
	extern vector<float> eye;
	extern vector<float> center;

	void init_map();

	class SolidCube : public Object
	{
	public:
		virtual ~SolidCube() = default;
		virtual void draw()
		{
			glutSolidCube(0.5);
		}
	};

}