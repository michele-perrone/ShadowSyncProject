#include "Shadow.h"
#define Z 1
#define RAD 10
Shadow::Shadow(){

}

void Shadow::setup(float x, float y)
{
	//i decided to use a node for the inheritance property (setParent option, may used later on)
	//given the osc x,y coord -> set the node x,y. z is by def 1 (just a bit above the wall, dummy solution)
	com.setGlobalPosition(x, y, Z);

	//set circle obj centered wrt the node
    c.set(com.getX(), com.getY(), RAD);
}

void Shadow::draw()
{
	c.draw();
}

void Shadow::move(float x_dir, float y_dir)
{
	//osc messages are normalized btw 0:1
	//test setup, just multiply 100x
	com.setGlobalPosition(x_dir * 100, y_dir * 100, Z);
	c.set(com.getX(), com.getY(), RAD);
}
