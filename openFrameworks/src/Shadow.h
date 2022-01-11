#pragma once
#include "ofMain.h"
#include "ParticleSystem2D.h"
#include "Circle.h"
#include "pose.h"

class Shadow
{
public:
	void setup(glm::vec2& origin);
	void setup(Pose* pose_shadow); 
	void draw();
	void move_centroid(float x_dir, float y_dir);
	void move_centroid();
	void move_junctions();

	void updateParticleSystems();

	glm::vec2& getOrigin();
	
private:
	Circle com_shadow; //cointains the shadow sys origin
	vector<Circle> shadow_junctions;
	vector<ParticleSystem2D> particle_systems_2d;
	glm::vec2 shadow_origin;
	float* sys_origin;
	vector <Circle*> shadow_attractors; //should follow the defined attractor(that is a junction) position
	Pose* pose;
};
