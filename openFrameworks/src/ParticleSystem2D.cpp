#include "ParticleSystem2D.h"
#define INIT_FORCE_MAG 0.001
//======================= 2D particle system =======

void ParticleSystem2D::setup(glm::vec2& shadowOrigin, int numParticles, int particleRadius, int particleLifespan)
{
	this->origin_PS2D = shadowOrigin;
	this->numParticles = numParticles;
	this->particleRadius = particleRadius;
	this->particleLifespan = particleLifespan;

	//float force_mag = 0.001;
	//particle system testing
	for (int j = 0; j < numParticles; j++)
	{
		//create a new particle
		Particle2D newParticle;
		//assign to it a random force
		float rand[] = { ofRandom(-INIT_FORCE_MAG, INIT_FORCE_MAG), ofRandom(-INIT_FORCE_MAG, INIT_FORCE_MAG) };
		glm::vec2 random_force = glm::make_vec2(rand);
		//setup of the particle
		newParticle.setup(origin_PS2D, particleRadius, particleLifespan, random_force); //passing origin to the particle by ref 
		//push back to the end of the vector
		particles.push_back(newParticle);
	}
}

void ParticleSystem2D::update()
{
	//update the particle system generated from / attracted to each junction
	for (int i = particles.size() - 1; i >= 0; i--)
    {
        particles[i].update(this->attractors[0]); //hp: only ONE attractor for each ps
		if (particles[i].isDead())
		{
			//delete dead particle
			particles.erase(particles.begin() + i );
			//add new particle
			addParticle();
		}
	}

}

void ParticleSystem2D::updateParticleMaxVals(float ms, float mf)
{
	for (int i = particles.size() - 1; i >= 0; i--)
	{
		particles[i].setMaxStuff(ms, mf);
	}

}

void ParticleSystem2D::draw()
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].draw();
	}
}

void ParticleSystem2D::setOrigin(int x, int y)
{
	origin_PS2D.x = x;
	origin_PS2D.y = y;
}

void ParticleSystem2D::moveOrigin(glm::vec2& newOrigin)
{

	origin_PS2D = newOrigin;
	
}

void ParticleSystem2D::setDestination(int x, int y)
{
	destination.x = x;
	destination.y = y;
}

void ParticleSystem2D::moveDestination(int x_dir, int y_dir)
{
	destination.x += x_dir;
	destination.y += y_dir;
}

void ParticleSystem2D::addParticle() //QUI la (nuova) origine del PS deve essere passata alla particella
{
	
	//create a new particle
	Particle2D newParticle;
	//assign to it a random force
	float rand[] = { ofRandom(-INIT_FORCE_MAG, INIT_FORCE_MAG), ofRandom(-INIT_FORCE_MAG, INIT_FORCE_MAG) };
	glm::vec2 random_force = glm::make_vec2(rand);
	//setup of the particle
	newParticle.setup(origin_PS2D, particleRadius, particleLifespan, random_force); //passing origin by ref
	//push back to the end of the vector
	particles.push_back(newParticle);
	
}

void ParticleSystem2D::setAttractors(Circle* attractor)
{
	this->attractors.push_back(attractor);
}
