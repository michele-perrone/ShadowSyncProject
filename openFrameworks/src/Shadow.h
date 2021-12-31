#pragma once
#include "ofMain.h"
#include <ParticleSystem2d.h>
#include <Circle.h>


class Shadow
{
public:
	Shadow();
	void setup(); 
	void draw();
	void move(float x_dir, float y_dir);

	void updateParticleSystems();

	glm::vec2& getOrigin();

private:
	Circle com_shadow;
	vector<Circle> shadow_junctions;
	vector<ParticleSystem2D> particle_systems_2d;
};
