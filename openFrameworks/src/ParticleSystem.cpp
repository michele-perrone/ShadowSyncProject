#include "ParticleSystem.h"

void ParticleSystem::setup(ofNode& origin, int numParticles, int particleRadius, int particleLifespan)
{
    this->origin = origin;
    this->material.setDiffuseColor(ofFloatColor::red);
    this->numParticles = numParticles;
    this->particleRadius = particleRadius;
    this->particleLifespan = particleLifespan;

    float force_mag = 0.01;
    //particle system testing
    for (int j = 0; j < numParticles; j++)
    {
        //create a new particle
        Particle3D newParticle;
        //assign to it a random force
        float rand[] = { ofRandom(-force_mag, force_mag), ofRandom(-force_mag, force_mag) , ofRandom(-force_mag, force_mag) };
        glm::vec3 random_force = glm::make_vec3(rand);
        //setup of the particle
        newParticle.setup(origin, particleRadius, particleLifespan, random_force);
        //push back to the end of the vector
        particles.push_back(newParticle);
    }
}

void ParticleSystem::update()
{
    //update the particle system generated from / attracted to each junction
    for (int i = particles.size()-1; i >= 0; i--)
    {
        particles[i].update();
        //particles[i].lifespan -= 0.5;
        if (particles[i].isDead())
        {
            //delete dead particle
            particles.erase(particles.begin()+i);
            //add new particle
            addParticle();
        }
    }
}

void ParticleSystem::draw()
{
    material.begin();
    for (int i = 0; i < particles.size(); i++)
    {
        particles[i].draw();
    }
    material.end();
}

void ParticleSystem::setOrigin(int x, int y, int z)
{
    origin.setPosition(x, y, z);
}

void ParticleSystem::moveOrigin(int x_dir, int y_dir, int z_dir)
{
    glm::vec3 current_pos = origin.getPosition();
    origin.setPosition(current_pos.x + x_dir,
                       current_pos.y + y_dir,
                       current_pos.z + z_dir);
}

void ParticleSystem::setDestination(int x, int y, int z)
{
    destination.setPosition(x, y, z);
}

void ParticleSystem::moveDestination(int x_dir, int y_dir, int z_dir)
{
    glm::vec3 current_pos = destination.getPosition();
    destination.setPosition(current_pos.x + x_dir,
                            current_pos.y + y_dir,
                            current_pos.z + z_dir);
}

void ParticleSystem::addParticle()
{
    float force_mag = 0.01;
    //create a new particle
    Particle3D newParticle;
    //assign to it a random force
    float rand[] = { ofRandom(-force_mag, force_mag), ofRandom(-force_mag, force_mag), ofRandom(-force_mag, force_mag) };
    glm::vec3 random_force = glm::make_vec3(rand);
    //setup of the particle
    newParticle.setup(origin, particleRadius, particleLifespan, random_force);
    //push back to the end of the vector
    particles.push_back(newParticle);
}