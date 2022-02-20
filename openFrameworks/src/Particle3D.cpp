#include "Particle3D.h"
#define RANDOM_FORCE_MAG 1
Particle3D::Particle3D()
{
}

void Particle3D::setup(glm::vec3& origin, float radius, float lifespan, glm::vec3 force) 
{
    velocity *= 0;
    position = origin;
   // std::cout << "Particle origin: " << position << std::endl;
    this->radius = radius;
    this->lifespan = lifespan;
    this->force = force;
    myParticle.set(radius, 10);
    myParticle.setGlobalPosition(position); //set ofSphere position 

    //color setup
    ofColor p_color = ofColor::aquamarine;
    my3dParticleColor = (p_color, this->lifespan);
    p_material.setDiffuseColor(my3dParticleColor);
 }

void Particle3D::update() {
    velocity += force;
    position += velocity;
    myParticle.move(velocity);
    lifespan -= death_rate;
}

void Particle3D::update(ofSpherePrimitive * attractor)
{
    target = attractor->getGlobalPosition();
    distance = target - position;
    float d_mag = glm::length(distance); //magnitude
    glm::normalize(distance);
    if (d_mag < 10)
    {
        float m = ofMap(d_mag, 0, 10, 0,  body_ps_max_speed);
        distance *= m;
    }
    else
    {
        distance *= body_ps_max_speed;
    }
    glm::vec3 steer = (distance - velocity); //(desired-velocity)*0.0001;
    float limit_steer = (glm::length(steer) / body_ps_max_force);
    steer.x = steer.x / limit_steer;
    steer.y = steer.y / limit_steer;
    //ADDING RANDOMNESS
    float rand[] = { ofRandom(-RANDOM_FORCE_MAG, RANDOM_FORCE_MAG), ofRandom(-RANDOM_FORCE_MAG , RANDOM_FORCE_MAG) , ofRandom(-RANDOM_FORCE_MAG, RANDOM_FORCE_MAG) };
    glm::vec3 random_force = glm::make_vec3(rand);
    steer += random_force;
    //applyforce(steer);
    force += steer;

    velocity += force;
    float limit_vel = (glm::length(velocity) / body_ps_max_speed);
    velocity.x = velocity.x / limit_vel;
    velocity.y = velocity.y / limit_vel;

    position += velocity;
    myParticle.move(velocity);
    //myParticle.setGlobalPosition(position);
    lifespan -= death_rate;

    force *= 0;

    my3dParticleColor = (my3dParticleColor, -lifespan); //updating color alpha
    p_material.setDiffuseColor(my3dParticleColor);

}

void Particle3D::draw() 
{
    p_material.begin();
    myParticle.draw();
    p_material.end();
}

bool Particle3D::isDead() 
{
    return (this->lifespan <= 6); //not to end black on decay cycle
    //return false;
}

void Particle3D::setMaxStuff(float ms, float mf)
{
    body_ps_max_force = mf;
    body_ps_max_speed = ms;
}
