#pragma once
#include "ofMain.h"

struct Circle
{ //centered arounf each node pos, received from OSC
	ofVec2f center;
	float radius;
    void set(float xc, float yc, float r)
    {
		center.x = xc;
		center.y = yc;
		radius = r;
	}
    void draw()
    {
		ofSetColor(0, 0, 0);
		ofFill();
		ofDrawCircle(center, radius);
	}
};

class Shadow
{
public:
	Shadow();
	void setup(float x, float y); // ~ stick to the wall
	void draw();
	void move(float x_dir, float y_dir);

private:
	ofNode com;
	Circle c;
	vector <ofNode> shadow_junctions;
};
