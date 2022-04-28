#include "Particle2D.h"
#define RANDOM_FORCE_MAG 1
//======================= 2D Particle ===============

Particle2D::Particle2D()
{
}

void Particle2D::setup(glm::vec2& origin, float radius, float lifespan, glm::vec2 force)
{
	velocity *= 0; //setup velocity
	position = origin;
	this->radius = radius;
	this->lifespan = lifespan;
	this->force = force;
	my2dParticle.set(origin, radius); 
	my2dParticleColor = ofColor::black; 

}


void Particle2D::update(Circle * attractor)
{
	target = attractor->center;
    distance = target - position;
    float d_mag = glm::length(distance); //magnitude
    glm::normalize(distance);
    if (d_mag < 10)
    {
        float m = ofMap(d_mag, 0, 10, 0, shadow_ps_max_speed);
        distance *= m;
    }
    else 
    {
        distance *= shadow_ps_max_speed;
    }
    glm::vec2 steer = (distance - velocity); //(desired-velocity)*0.0001;
    float limit_steer = (glm::length(steer) / shadow_ps_max_force);
    steer.x = steer.x / limit_steer;
    steer.y = steer.y / limit_steer;
    //ADDING RANDOMNESS
    float rand[] = { ofRandom(-RANDOM_FORCE_MAG, RANDOM_FORCE_MAG), ofRandom(-RANDOM_FORCE_MAG , RANDOM_FORCE_MAG) };
    glm::vec2 random_force = glm::make_vec2(rand);
    steer += random_force;
    //applyforce(steer);
    force += steer;

    velocity += force;
    float limit_vel = (glm::length(velocity) / shadow_ps_max_speed);
    velocity.x = velocity.x / limit_vel;
    velocity.y = velocity.y / limit_vel;

    position += velocity;
    my2dParticle.move(velocity);
    lifespan -= death_rate;

    force *= 0;
}



void Particle2D::draw()
{
    //ofSetColor(my2dParticleColor, lifespan);
    my2dParticle.draw(my2dParticleColor, lifespan + 150);
}

bool Particle2D::isDead()
{
    return (this->lifespan <=6);
    //return false;
}

void Particle2D::setMaxStuff(float ms, float mf)
{
    shadow_ps_max_force = mf;
    shadow_ps_max_speed = ms;

}

void Particle2D::setSyncColor()
{
    this->sync_color = true;
    ofColor p_color = ofColor::aquamarine;
    this->my2dParticleColor = p_color;

}