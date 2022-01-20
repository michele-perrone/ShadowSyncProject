#include "Particle2D.h"

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

void Particle2D::update()
{
	velocity += force;
	//float limit = glm::length(velocity) / MAXSPEED;
	//velocity.x = velocity.x / limit;
	//velocity.y = velocity.y / limit;

	position += velocity;
	my2dParticle.move(velocity); //either this or .set() for updating the particle center since its not passed by ref
	
	lifespan -= death_rate;
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


/*void Particle2D::update(Circle* attractor) //Mike's
{
    glm::vec2 target = attractor->center;
    glm::vec2 distance = target - position;
    float d_mag = glm::length(distance); //magnitude

    glm::vec2 steer = (distance - velocity); //(desired-velocity)*0.0001;
    float limit = (glm::length(steer) / MAXFORCE);
    steer.x = steer.x / limit;
    steer.y = steer.y / limit;

    force += steer;

    velocity += force + ofMap(d_mag, 0, 50, -MAXFORCE, MAXFORCE);
    position += velocity;
    my2dParticle.move(velocity);
    lifespan -= death_rate;

    // Damp the particle when it's near the attractor
    float damping = 1.3;
    if (d_mag < 25)
    {
        velocity /= damping;
    }
}*/


void Particle2D::draw()
{
    //ofSetColor(my2dParticleColor, lifespan);
    my2dParticle.draw(my2dParticleColor, lifespan + 50);
}

bool Particle2D::isDead()
{
    return (this->lifespan <=0);
    //return false;
}

void Particle2D::setMaxStuff(float ms, float mf)
{
    shadow_ps_max_force += mf;
    shadow_ps_max_speed += ms;

}