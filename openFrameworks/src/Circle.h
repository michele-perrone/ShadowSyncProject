#pragma once
#include <ofMain.h>

struct Circle
{ //centered arounf each node pos, received from OSC
	glm::vec2 center;
	float radius;

	void set(float xc, float yc, float r)
	{
		center.x = xc;
		center.y = yc;
		radius = r;
	}

	void draw(ofColor color, int lifespan)
	{
		ofSetColor(color, lifespan);
		ofFill();
		ofDrawCircle(center, radius);
	}

	void move(glm::vec2 direction)
	{
		this->center.x += direction.x;
		this->center.y += direction.y;
	}
};