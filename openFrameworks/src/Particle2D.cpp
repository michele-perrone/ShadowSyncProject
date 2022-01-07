#include "Particle2D.h"

//======================= 2D Particle ===============

Particle2D::Particle2D()
{
}

void Particle2D::setup(glm::vec2& origin, float radius, float lifespan, glm::vec2 force)
{
	position = origin;
	this->radius = radius;
	this->lifespan = lifespan;
	this->force = force;
	my2dParticle.set(origin, radius); //con un pointer
	my2dParticleColor = ofFloatColor::grey; //rn setup manually

}

void Particle2D::update()
{
	position += velocity;
	velocity += force;
	my2dParticle.move(velocity);
	lifespan -= this->death_rate;
}

/*
void Particle2D::follow_system_origin(float x_sys_origin, float y_sys_origin)
{
	this->position.x += x_sys_origin;
	this->position.y += y_sys_origin;
	my2dParticle.move(this->position); //otherwise, the particle position is not updated to the new origin! IDK how this works in 3D
	
}*/


void Particle2D::draw()
{
	//ofSetColor(my2dParticleColor, lifespan);
	my2dParticle.draw(my2dParticleColor, lifespan);
}

bool Particle2D::isDead()
{
	return (this->lifespan <=0);
	//return false;
}
