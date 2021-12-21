#pragma once
#include "ofMain.h"

class Body {
public:
	Body();
	void setup();
	//void update();
	void draw();
	void move(float x_dir, float y_dir, float z_dir);
private:
	ofMaterial material;
	ofSpherePrimitive com; 
	ofSpherePrimitive body_junctions[5];
};