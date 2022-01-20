#include "ParticleSystem.h"
#define INIT_FORCE_MAG 0.001

void ParticleSystem::setup(glm::vec3 origin, int numParticles, int particleRadius, int particleLifespan)
{
    this->origin_PS = origin;
    this->numParticles = numParticles;
    this->particleRadius = particleRadius;
    this->particleLifespan = particleLifespan;

    //particle system testing
    for (int j = 0; j < numParticles; j++)
    {
        //create a new particle
        Particle3D newParticle;
        //assign to it a random force
        float rand[] = { ofRandom(-INIT_FORCE_MAG, INIT_FORCE_MAG), ofRandom(-INIT_FORCE_MAG , INIT_FORCE_MAG) , ofRandom(-INIT_FORCE_MAG, INIT_FORCE_MAG) };
        glm::vec3 random_force = glm::make_vec3(rand);
        //setup of the particle
        newParticle.setup(origin_PS, particleRadius, particleLifespan, random_force);
        particles.push_back(newParticle);
    }


}

void ParticleSystem::update()
{
    //update the particle system generated from / attracted to each junction
    for (int i = particles.size()-1; i >= 0; i--)
    {
        particles[i].update(this->attractors[0]); //hp: only ONE attractor for each ps
        if (particles[i].isDead())
        {
            //delete dead particle
            particles.erase(particles.begin()+i);
            //add new particle
            addParticle();
        }
    }
}

void ParticleSystem::updateParticleMaxVals(float ms, float mf)
{
    for (int i = particles.size() - 1; i >= 0; i--)
    {
        particles[i].setMaxStuff(ms, mf);
    }
}

void ParticleSystem::draw()
{
    for (int i = 0; i < particles.size(); i++)
    {
        particles[i].draw();
    }
}


void ParticleSystem::moveOrigin(glm::vec3 newOrigin)
{
    origin_PS = newOrigin;
}


void ParticleSystem::addParticle()
{
    //create a new particle
    Particle3D newParticle;
    //assign to it a random force
    float rand[] = { ofRandom(-INIT_FORCE_MAG, INIT_FORCE_MAG), ofRandom(-INIT_FORCE_MAG, INIT_FORCE_MAG), ofRandom(-INIT_FORCE_MAG, INIT_FORCE_MAG) };
    glm::vec3 random_force = glm::make_vec3(rand);
    //setup of the particle
    newParticle.setup(origin_PS, particleRadius, particleLifespan, random_force);
    //push back to the end of the vector
    particles.push_back(newParticle);
}

void ParticleSystem::setAttractors(ofSpherePrimitive* attractor)
{
    this->attractors.push_back(attractor);
}
