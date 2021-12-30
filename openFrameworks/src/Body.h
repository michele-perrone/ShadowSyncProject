#pragma once
#include "ofMain.h"
#include "ParticleSystem.h"


class Body
{
public:
	void setup();
	void draw();
	void move(float x_dir, float y_dir, float z_dir);

    void updateParticleSystems();

	ofNode& getOrigin();

private:
	ofSpherePrimitive com;
	ofMaterial material; 
    vector <ofSpherePrimitive> body_junctions;
    vector <ParticleSystem> particle_systems;
};
