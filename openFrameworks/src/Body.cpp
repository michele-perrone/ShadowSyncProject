#include "Body.h"
#define NUMPARTICLES 100
#define P_LIFESPAN 200
#define P_RADIUS 1


Body::Body() {

}

void Body::setup() {
	material.setDiffuseColor(ofFloatColor::aqua);
	com.set(3, 10); //center of mass

	for (int i = 0; i < 5; i++) {
		//vector of junctions
		ofSpherePrimitive temp_junction;
		temp_junction.set(2, 10);
		temp_junction.setParent(com);
		body_junctions.push_back(temp_junction);

	}
	//child nodes can move independetly
	body_junctions[0].move(0, 50, 0);	//head
	body_junctions[1].move(-30, 40, 0); //left shoulder
	body_junctions[2].move(30, 40, 0);  //right shoulder
	body_junctions[3].move(-20, -30, 0);//left hip (anca)
	body_junctions[4].move(20, -30, 0); //rigth hip
	
}

void Body::draw() {
	com.draw(); //not in same loop just for having a different (white, default) color for com

	material.begin(); //this will affect all the elements inside (like push/pop matrix)
	for (int i = 0; i < 5; i++)
		body_junctions[i].draw();
	material.end();

}

void Body::move(float x_dir, float y_dir, float z_dir)
{
	com.move(x_dir, y_dir, z_dir); //if a parent node moves --> all child nodes will follow
}

ofNode& Body::getOrigin()
{
	return Body::com;
}


//========================================= Particle system ======================================================

void Body::setupParticleSystem(ofNode& origin)
{
	float force_mag = 0.01;
	//particle system testing
	for (int j = 0; j < NUMPARTICLES; j++)
	{
		//create a new particle
		Particle3D newParticle;
		//assign to it a random force		
		float rand[] = { ofRandom(-force_mag, force_mag), ofRandom(-force_mag, force_mag) , ofRandom(-force_mag, force_mag) };
		glm::vec3 random_force = glm::make_vec3(rand);
		//setup of the particle
		newParticle.setup(origin, P_RADIUS, P_LIFESPAN, random_force);
		//push back to the end of the vector
		particles.push_back(newParticle);
	}
}

void Body::updateParticleSystem()
{
	//update the particle system generated from / attracted to each junction
	for (int i = particles.size()-1; i >= 0; i--) 
	{
		particles[i].update();
		//particles[i].lifespan -= 0.5;
		if (particles[i].isDead())
		{
			//delete dead particle
			particles.erase(particles.begin()+i-1);
			//add new particle
			//add new particle method (substitute in the dead one pos?)
		}
	}

	

}

void Body::drawParticleSystem()
{
	
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].draw();
	}

}