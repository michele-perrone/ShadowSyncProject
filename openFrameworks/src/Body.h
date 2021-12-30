#pragma once
#include "Particle3D.h"
#include "ofMain.h"


class Body {
public:
	Body();
	void setup();
	void draw();
	void move(float x_dir, float y_dir, float z_dir);

	void setupParticleSystem(ofNode& origin);
	void updateParticleSystem();
	void drawParticleSystem();
	ofNode& getOrigin();

private:
	ofSpherePrimitive com;
	ofMaterial material; 
	vector <ofSpherePrimitive> body_junctions;
	vector <Particle3D> particles;

};