#pragma once
#include "ofMain.h"
#include "Circle.h"



class Particle2D
{
public:
	Particle2D();
	void setup(glm::vec2& origin, float rad, float lifesp, glm::vec2 f);
	void draw();
	bool isDead();
	float lifespan;
    void update(Circle * attractor);

	//here just for convenience
	float shadow_ps_max_speed = 0;
	float shadow_ps_max_force = 0;

	void setMaxStuff(float ms, float mf);

	void setSyncColor();

private:
	glm::vec2 position, velocity, force, target, distance;
	float radius;
	Circle my2dParticle; //counterpart of ofSpherePrimitive
	ofColor my2dParticleColor;
	float death_rate = ofRandom(2,5);
	bool sync_color = false;

};
