#pragma once

#include "../glut.h"
#include "../bmp.h"

#include <cassert>
#include <vector>
#include <iostream>
using namespace std;

namespace Sokoban
{
	class Object;
	class Map;
	extern Map map;
	extern vector<Object*> object_list;
	extern vector<float> eye;
	extern vector<float> center;
	extern vector<int> window_size;

	// textures
	extern const int texture_count;
	extern enum TextureID;
	extern vector<GLuint> textures;
	extern vector<bmpImage*> texture_images;

	void init_map();

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

	template <class T>
	ostream& operator<<(ostream& out, const vector<T>& v)
	{
		if (v.empty()) {
			out << "[ ]";
			return out;
		}
		out << "[ " << v[0];
		for (int i = 1; i < v.size(); i++) {
			out << ", " << v[i];
		}
		out << " ]";
		return out;
	}

	class Object
	{
	public:
		virtual void bind_map(Map *map) = 0;
		virtual void draw() = 0;
		virtual bool is_movable() = 0;
		virtual void move_to(vector<int> end) = 0;
		virtual bool is_moving() = 0;
		virtual const vector<int>& get_position() const = 0;
	};

	class SolidCube : public Object
	{
	private:
		vector<int> position;
		vector<float> move;
		bool moving;
		Map *map;

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
		SolidCube(Map *map, const vector<int>& p) : position(p)
		{
			move = { 0, 0, 0 };
			bind_map(map);
		}
		virtual ~SolidCube() = default;
		virtual void bind_map(Map *map) { this->map = map; }
		virtual void draw();
		virtual bool is_movable() { return true; }
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
			//map->set_object(this, end[0], end[1]);
			moving = true;
		}
		virtual const vector<int>& get_position() const
		{
			cout << position << endl;
			return position;
		}
	};

	class Map
	{
	private:
		Object*** map_data;
		int width;
		int height;
		vector<float> o;
		vector<float> x;
		vector<float> y;
		vector<float> z;

	public:
		Map(int width, int height): width(width), height(height)
		{
			map_data = new Object * *[height];
			for (int i = 0; i < height; i++) {
				map_data[i] = new Object * [width];
				for (int j = 0; j < width; j++) {
					map_data[i][j] = new SolidCube(this, { i, j, 0 });
					object_list.push_back(map_data[i][j]);
					cout << object_list << endl;
				}
			}
			o = { 0, 0, 0 };
			x = { 1, 0, 0 };
			y = { 0, 0, -1 };
			z = { 0, 1, 0 };
		}
		const vector<float> real_position(const vector<float>& position) const
		{
			assert(position.size() == 3);
			vector<float> ret(3);
			ret[0] = o[0] + position[0] * x[0] + position[1] * y[0] + position[2] * z[0];
			ret[1] = o[1] + position[0] * x[1] + position[1] * y[1] + position[2] * z[1];
			ret[2] = o[2] + position[0] * x[2] + position[1] * y[2] + position[2] * z[2];
			return ret;
		}
		void set_object(Object* object, int x, int y)
		{
			map_data[x][y] = object;
		}
		Object* get_object(int x, int y) const
		{
			return map_data[x][y];
		}
		~Map()
		{
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					delete map_data[i][j];
				}
				delete[] map_data[i];
			}
			delete[] map_data;
		}
	};

}