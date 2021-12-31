#pragma once
#include "ofMain.h"
#include "Particle2D.h"

class ParticleSystem2D
{
public:
	void setup(glm::vec2& origin, int numParticles, int ParticleRadius, int particleLifespan);
	void update();
	void draw();
	void setOrigin(int x, int y);
	void moveOrigin(int x_dir, int y_dir);
	void setDestination(int x, int y);
	void moveDestination(int x_dir, int y_dir);
	void addParticle();

private:
	int numParticles, particleRadius, particleLifespan;
	glm::vec2 origin;
	glm::vec2 destination;

	vector <Particle2D> particles;
};
