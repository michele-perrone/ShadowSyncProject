#pragma once
#include "ofMain.h"

class Particle3D
{
public:
    Particle3D();
    void setup(glm::vec3& origin, float radius, float lifespan, glm::vec3 force);
    void draw();
    bool isDead();
    float lifespan;
    void update(ofSpherePrimitive* attractor);

    float body_ps_max_speed = 0;
    float body_ps_max_force = 0;

    void setMaxStuff(float ms, float mf);
    bool dead = false;

    void setSyncColor();
private:
    glm::vec3 position, velocity, force, target, distance;
    float radius;
    ofSpherePrimitive myParticle;
    ofColor my3dParticleColor;
    float death_rate = ofRandom(2, 5);
    ofMaterial p_material;
    bool sync_color = false;
};