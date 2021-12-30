#pragma once
#include "Particle3D.h"
#include "ofMain.h"


class Body {
public:
	Body();
	void setup();
	//void update();
	void draw();
	void move(float x_dir, float y_dir, float z_dir);
	void setupParticleSystem(ofNode& origin);
	void updateParticleSystem();
	void drawParticleSystem();
	ofNode& getOrigin();

private:
	ofSpherePrimitive com;
	ofMaterial material; 
	//ofSpherePrimitive body_junctions[5];
	vector <ofSpherePrimitive> body_junctions;
	vector <Particle3D> particles;
};