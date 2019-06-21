#pragma once

#include "../glut.h"

#include <cassert>
#include <vector>
#include <iostream>

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
		virtual void move_to(vector<int> end) = 0;
		virtual bool is_moving() = 0;
		virtual const vector<int>& get_position() const = 0;
	};

	extern Object* map[10][10];
	extern vector<Object*> object_list;
	extern vector<float> eye;
	extern vector<float> center;

	void init_map();

	class SolidCube : public Object
	{
	private:
		vector<int> position;
		vector<float> move;
		bool moving;

		void move_once()
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

	public:
		SolidCube(const vector<int>& p) : position(p)
		{
			move = { 0, 0, 0 };
		}
		virtual ~SolidCube() = default;
		virtual void draw();
		
		virtual bool is_moving()
		{
			return moving;
		}
		virtual void move_to(vector<int> end)
		{
			if (moving)
				return;
			assert(end.size() == 3);
			move[0] = (end[0] - position[0]) * 1.0;
			move[1] = (end[1] - position[1]) * 1.0;
			move[2] = (end[2] - position[2]) * 1.0;
			position = end;
			moving = true;
		}
		virtual const vector<int>& get_position() const
		{
			return position;
		}
	};

}