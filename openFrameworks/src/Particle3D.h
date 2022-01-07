#pragma once
#include "ofMain.h"

class Particle3D
{
public:
    Particle3D();
    void setup(ofNode& origin, float radius, float lifespan, glm::vec3 force);
    void update();
    void draw();
    bool isDead();
    float lifespan;

private:
    glm::vec3 position, velocity, force;
    float radius;
    ofSpherePrimitive myParticle;

    float death_rate = ofRandom(1, 5);
};