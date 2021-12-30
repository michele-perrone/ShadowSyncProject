#pragma once
#include "ofMain.h"

class Particle3D
{
public:
    Particle3D();
    void setup(ofNode& origin, float rad, float lifesp, glm::vec3 f);
    void update();
    void draw();
    bool isDead();
    float lifespan;

private:
    glm::vec3 position, velocity, force;
    float radius;
    ofSpherePrimitive myParticle;
};