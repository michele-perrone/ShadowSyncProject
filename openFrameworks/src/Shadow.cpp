#include "Shadow.h"
#define NUMPARTICLES 100
#define P_LIFESPAN 150
#define P_RADIUS 1
#define SHADOW_RADIUS 2
#define SCALE_FACTOR 150
#define TRASL_VECTOR {0, -0.9}
#define SCALE_VECTOR {1, -1}


void Shadow::setup(glm::vec2& origin)
{
	shadow_origin = origin;
	com_shadow.set(origin, SHADOW_RADIUS);
	
	for (int i = 0; i < 5; i++) { //add junctions of the shadow
		Circle temp_junction;
		temp_junction.set(origin * 0.5* i , SHADOW_RADIUS); //just a random position
		shadow_junctions.push_back(temp_junction);
	}

	//setup of the 2D particle systems 
	ParticleSystem2D com_particleSystem;
	com_particleSystem.setup(this->getOrigin(), NUMPARTICLES, P_RADIUS, P_LIFESPAN); //passing shadow_origin by ref
	particle_systems_2d.push_back(com_particleSystem);

	//attractor setup testing
	int num_attractors = 1;
	//for (int i = 0; i < num_attractors; i++)
	//{	//Circle* temp_attractor = &(shadow_junctions[i]); //pointer to the junction def. as attractor
	shadow_attractors.push_back(&(shadow_junctions[1]));
	//}
	//for each ps i have to define an attractor
	particle_systems_2d[0].setAttractors(shadow_attractors[0]);
	
}

void Shadow::setup(Pose* pose_shadow)
{
	//impossible to set here the shadow elems, since Circle.set() requires the position (not yet received via OSC)
	this->pose = pose_shadow;
	sys_origin = pose->body_centroid; //pointer
	shadow_origin = glm::make_vec2(sys_origin);
	com_shadow.setRadius(SHADOW_RADIUS); //method that sets only the radius ~sphere.set
	for (int i = 0; i < POSE_JUNCTIONS; i++)
	{
		Circle temp_junction;
		temp_junction.setRadius(SHADOW_RADIUS);
		shadow_junctions.push_back(temp_junction);
	}

	//setup of the 2D particle systems 
	ParticleSystem2D com_particleSystem;
	com_particleSystem.setup(this->getOrigin(), NUMPARTICLES, P_RADIUS, P_LIFESPAN); //passing shadow_origin by ref
	particle_systems_2d.push_back(com_particleSystem);

	//attractor setup testing
	int num_attractors = 1;
	for (int i = 0; i < num_attractors; i++)
	{	
		Circle* temp_attractor = &(shadow_junctions[i]); //pointer to the junction def. as attractor
		shadow_attractors.push_back(temp_attractor);

		particle_systems_2d[0].setAttractors(shadow_attractors[i]);
	}
	

}

glm::vec2& Shadow::getOrigin()
{
	return shadow_origin;
}

void Shadow::draw()
{
	com_shadow.draw(ofFloatColor::black, 200);
	for (int i = 0; i < shadow_junctions.size(); i++)
	{
		shadow_junctions[i].draw(ofFloatColor::green, 200);
	}
	
	
	for (int j = 0; j < particle_systems_2d.size(); j++)
	{
		particle_systems_2d[j].draw();
	}
	
}

void Shadow::move_centroid(float x_dir, float y_dir)
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

	//testing if the attractor movement is followed by the particles
	float moving_a[] = { 2 * x_dir, x_dir / 4 };
	glm::vec2 moving_att = glm::make_vec2(moving_a);
	this->shadow_junctions[1].move(moving_att);

}

void Shadow::move_centroid()
{
	glm::vec2 scale_vec(SCALE_VECTOR);
	scale_vec *= SCALE_FACTOR;
	glm::vec2 trasl_vec(TRASL_VECTOR);
	com_shadow.setGlobalPosition(scale_vec *(trasl_vec + glm::make_vec2(pose->body_centroid)));
	//std::cout << "com: " << com_shadow.center << std::endl;
	//move particle Sys
	shadow_origin = scale_vec * (trasl_vec + glm::make_vec2(sys_origin));
	std::cout << "Shadow origin: " << shadow_origin << std::endl;
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

void Shadow::move_junctions()
{
	glm::vec2 scale_vec(SCALE_VECTOR);
	scale_vec *= SCALE_FACTOR;
	glm::vec2 trasl_vec(TRASL_VECTOR);

	//face
	shadow_junctions[0].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->nose)));
	shadow_junctions[1].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_eye_inner)));
	shadow_junctions[2].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_eye)));
	shadow_junctions[3].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_eye_outer)));
	shadow_junctions[4].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_eye_inner)));
	shadow_junctions[5].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_eye)));
	shadow_junctions[6].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_eye_outer)));
	shadow_junctions[7].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_ear)));
	shadow_junctions[8].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_ear)));
	shadow_junctions[9].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->mouth_left)));
	shadow_junctions[10].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->mouth_right)));
	//shoulder
	shadow_junctions[11].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_shoulder)));
	shadow_junctions[12].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_shoulder)));
	//hip
	shadow_junctions[13].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_hip)));
	shadow_junctions[14].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_hip)));
	//left arm
	shadow_junctions[15].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_pinky)));
	shadow_junctions[16].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_index)));
	shadow_junctions[17].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_thumb)));
	shadow_junctions[18].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_elbow)));
	shadow_junctions[19].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_wrist)));
	//right harm
	shadow_junctions[20].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_pinky)));
	shadow_junctions[21].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_index)));
	shadow_junctions[22].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_thumb)));
	shadow_junctions[23].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_elbow)));
	shadow_junctions[24].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_wrist)));
	//left leg
	shadow_junctions[25].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_heel)));
	shadow_junctions[26].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_foot_index)));
	shadow_junctions[27].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_knee)));
	shadow_junctions[28].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_ankle)));
	//right leg
	shadow_junctions[29].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_heel)));
	shadow_junctions[30].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_foot_index)));
	shadow_junctions[31].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_knee)));
	shadow_junctions[32].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_ankle)));

}







