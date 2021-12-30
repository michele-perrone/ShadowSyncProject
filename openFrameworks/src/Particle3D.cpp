#include "Particle3D.h"

Particle3D::Particle3D(){
    lifespan = 0;
    radius = 0;
}

void Particle3D::setup(ofNode& origin, float rad, float lifesp, glm::vec3 f) 
{
    position = origin.getPosition();
    //std::cout << "setup particle position: " << position << std::endl;
    radius = rad;
    lifespan = lifesp;
    force = f;
    myParticle.set(radius, 10);
    myParticle.setParent(origin);
 
}

void Particle3D::update() {
    position += velocity;
    velocity += force;
    //acceleration *= 0;
    myParticle.move(velocity);
    //std::cout << "rel. particle position: " << position << std::endl;
}

void Particle3D::applyForce() {
    acceleration += force;
}

void Particle3D::draw() {
    ofSetColor(10, 100, 250, lifespan);
    myParticle.draw();
}

bool Particle3D::isDead() {
   // return (this->lifespan <= 0);
    return false;
}
