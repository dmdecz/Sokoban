#include "model.h"
using namespace Sokoban;

EmptyCube::EmptyCube(const vector<int>& p) :
	Object(EMPTY_ID), position(p)
{

}

void EmptyCube::register_disp()
{
	// draw nothing
}

void EmptyCube::draw()
{
	// draw nothing
}
