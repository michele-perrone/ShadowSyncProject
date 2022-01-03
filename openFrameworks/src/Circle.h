#pragma once
#include "ofMain.h"

struct Circle
{ //centered arounf each node pos, received from OSC
	glm::vec2 center;
	float radius;

    void set(glm::vec2 origin, float r)
	{
		center.x = origin.x;
		center.y = origin.y;
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
		center.x += direction.x;
		center.y += direction.y;
	}
};
