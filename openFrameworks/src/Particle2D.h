#pragma once
#include "ofMain.h"
#include "Circle.h"



class Particle2D
{
public:
	Particle2D();
	void setup(glm::vec2& origin, float rad, float lifesp, glm::vec2 f);
	void update();
	void follow_system_origin(float x_sys_origin, float y_sys_origin);
	void draw();
	bool isDead();
	float lifespan;
private:
	glm::vec2 position, velocity, force;
	float radius;
	Circle my2dParticle; //counterpart of ofSpherePrimitive
	ofColor my2dParticleColor;

	float death_rate = ofRandom(1,5);
};