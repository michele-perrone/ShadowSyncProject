#pragma once
#include "ofMain.h"
#include "ParticleSystem.h"
#include "pose.h"

class Body
{
public:
	void setup(Pose* pose_body);
	void draw();
	void move_centroid(float x_dir, float y_dir, float z_dir);
	void move_centroid();
	void move_junctions();
    void updateParticleSystems();

	ofNode& getOrigin();

private:
	ofSpherePrimitive com;
	ofMaterial material; 
    vector <ofSpherePrimitive> body_junctions;
    vector <ParticleSystem> particle_systems;
	Pose* pose;
};
