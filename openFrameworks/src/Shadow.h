#pragma once
#include "ofMain.h"
#include "ParticleSystem2D.h"
#include "Circle.h"


class Shadow
{
public:
	Shadow();
	void setup(glm::vec2& origin); 
	void draw();
	void move(float x_dir, float y_dir);

	void updateParticleSystems();

	glm::vec2& getOrigin();
	
private:
	Circle com_shadow; //cointains the shadow sys origin
	vector<Circle> shadow_junctions;
	vector<ParticleSystem2D> particle_systems_2d;
	glm::vec2 shadow_origin;
};
