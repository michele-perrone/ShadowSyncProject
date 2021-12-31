#include "Shadow.h"
#define NUMPARTICLES 1000
#define P_LIFESPAN 200
#define P_RADIUS 1
#define SHADOW_RADIUS 2

Shadow::Shadow(){

}

void Shadow::setup(glm::vec2& origin)
{
	shadow_origin = origin;
	com_shadow.set(origin, SHADOW_RADIUS);
	
	for (int i = 0; i < 5; i++) { //add junctions of the shadow
		Circle temp_junction;
		temp_junction.set(origin * i * 5, SHADOW_RADIUS); //in a random position
		shadow_junctions.push_back(temp_junction);
	}
	//setup of the 2D particle systems 
	ParticleSystem2D com_particleSystem;
	com_particleSystem.setup(this->getOrigin(), NUMPARTICLES, P_RADIUS, P_LIFESPAN); //passing shadow_origin by ref
	particle_systems_2d.push_back(com_particleSystem);

}

glm::vec2& Shadow::getOrigin()
{
	return shadow_origin;
}

void Shadow::draw()
{
	com_shadow.draw(ofFloatColor::black, 200);
	//color = ofFloatColor::green;
	for (int i = 0; i < shadow_junctions.size(); i++)
	{
		shadow_junctions[i].draw(ofFloatColor::black, 200);
	}
	
	
	for (int j = 0; j < particle_systems_2d.size(); j++)
	{
		particle_systems_2d[j].draw();
	}
	
}

void Shadow::move(float x_dir, float y_dir)
{
	//osc messages are normalized btw 0:1
	//test setup, just multiply 100x
	//com.setGlobalPosition(x_dir * 100, y_dir * 100, Z);
	//c.set(com.getX(), com.getY(), RAD);

	float moving_dir[] = { x_dir, y_dir };
	glm::vec2 moving_v = glm::make_vec2(moving_dir);

	//moving com
	com_shadow.move(moving_v);
	shadow_origin += moving_v;


	for (int i = 0; i < particle_systems_2d.size(); i++)
	{
		particle_systems_2d[i].moveOrigin(shadow_origin);
	}

}

void Shadow::updateParticleSystems()
{
	for (int i = 0; i < particle_systems_2d.size(); i++)
		particle_systems_2d[i].update();
}









