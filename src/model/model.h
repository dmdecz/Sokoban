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
	extern vector<float> eye;
	extern vector<float> direction;
	extern vector<float> up;
	extern vector<int> window_size;
	extern int windowHandle;
	extern float zoomAngle;

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

	template <class T>
	const vector<T> operator*(const vector<T> &v1, const float f)
	{
		vector<T> ret;
		int size = v1.size();
		for (int i = 0; i < size; i++)
			ret.push_back(v1[i] * f);
		return ret;
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
		Object**** map_data;
		vector<int> size;
		vector<float> o;
		vector<float> x;
		vector<float> y;
		vector<float> z;
		float cube_len;

	public:
		Map(int x_size, int y_size, int z_size = 1): size({x_size, y_size, z_size})
		{
			cube_len = 1;
			map_data = new Object***[size[2]];
			for (int i = 0; i < size[2]; i++) {
				map_data[i] = new Object * *[size[1]];
				for (int j = 0; j < size[1]; j++) {
					map_data[i][j] = new Object * [size[0]];
					for (int k = 0; k < size[0]; k++) {
						map_data[i][j][k] = new SolidCube(this, { k, j, i });
					}
				}
			}
			
			o = { 0, 0, 0 };
			x = { 1, 0, 0 };
			y = { 0, 0, 1 };
			z = { 0, -1, 0 };
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
		void set_object(Object* object, int x, int y, int z)
		{
			map_data[z][y][x] = object;
		}
		Object* get_object(int x, int y, int z) const
		{
			return map_data[z][y][x];
		}
		void draw() const
		{
			glPushMatrix();
			float matrix[] = {
				x[0], y[0], z[0], o[0],
				x[1], y[1], z[1], o[1],
				x[2], y[2], z[2], o[2],
				0, 0, 0, 1
			};
			glMultMatrixf(matrix);
			drawFloor();
			for (int i = 0; i < size[0]; i++) {
				for (int j = 0; j < size[1]; j++) {
					for (int k = 0; k < size[2]; k++) {
						if (map.get_object(i, j, k)) {
							map.get_object(i, j, k)->draw();
						}
					}
				}
			}
			glPopMatrix();
		}
		~Map()
		{
			for (int i = 0; i < size[2]; i++) {
				for (int j = 0; j < size[1]; j++) {
					for (int k = 0; k < size[0]; k++) {
						delete map_data[i][j][k];
					}
					delete[] map_data[i][j];
				}
				delete[] map_data[i];
			}
			delete[] map_data;
		}

		void drawFloor() const;
		float get_cube_len() const
		{
			return cube_len;
		}
	};

}