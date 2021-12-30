#include "Body.h"
#define NUMPARTICLES 100
#define P_LIFESPAN 200
#define P_RADIUS 1


void Body::setup()
{
	material.setDiffuseColor(ofFloatColor::aqua);
	com.set(3, 10); //center of mass

    for (int i = 0; i < 5; i++)
    {
		//vector of junctions
		ofSpherePrimitive temp_junction;
		temp_junction.set(2, 10);
		temp_junction.setParent(com);
		body_junctions.push_back(temp_junction);
	}
    //children nodes can move independetly
	body_junctions[0].move(0, 50, 0);	//head
	body_junctions[1].move(-30, 40, 0); //left shoulder
	body_junctions[2].move(30, 40, 0);  //right shoulder
	body_junctions[3].move(-20, -30, 0);//left hip (anca)
	body_junctions[4].move(20, -30, 0); //rigth hip

    // Create all the particle systems and store them inside the vector.
    // (For now, we have just one particle system located a the center of mass)
    ParticleSystem com_particleSystem;
    com_particleSystem.setup(this->getOrigin(), NUMPARTICLES, P_RADIUS, P_LIFESPAN);
    particle_systems.push_back(com_particleSystem);
}

void Body::draw()
{
	com.draw(); //not in same loop just for having a different (white, default) color for com

    material.begin(); //this will affect all the elements inside (like push/pop matrix)
    for (int i = 0; i < body_junctions.size(); i++)
		body_junctions[i].draw();
    material.end();

    for (int i = 0; i < particle_systems.size(); i++)
    {
        particle_systems[i].draw();
    }
}

void Body::move(float x_dir, float y_dir, float z_dir)
{
    //if a parent node moves --> all child nodes will follow
    com.move(x_dir, y_dir, z_dir);

    //... and also the particle sources move
    for(int i; i < particle_systems.size(); i++)
    {
        particle_systems[i].moveOrigin(x_dir, y_dir, z_dir);
    }
}

ofNode& Body::getOrigin()
{
	return Body::com;
}

void Body::updateParticleSystems()
{
    for(int i; i < particle_systems.size(); i++)
    {
        particle_systems[i].update();
    }
}
