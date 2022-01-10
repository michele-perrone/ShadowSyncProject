#include "Particle3D.h"

Particle3D::Particle3D(){
    lifespan = 0;
    radius = 0;
}

void Particle3D::setup(ofNode& origin, float radius, float lifespan, glm::vec3 force) 
{
    position = origin.getPosition();
    this->radius = radius;
    this->lifespan = lifespan;
    this->force = force;
    myParticle.set(radius, 10);
    myParticle.setParent(origin);

 }

void Particle3D::update() {
    position += velocity;
    velocity += force;
    myParticle.move(velocity);
    lifespan -= this->death_rate;
   
}

void Particle3D::draw() {
    ofSetColor(10, 100, 250, lifespan);
    myParticle.draw();
}

bool Particle3D::isDead() {
    return (this->lifespan <= 0);
    //return false;
}
