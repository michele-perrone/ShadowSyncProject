#include "Shadow.h"
#define NUMPARTICLES 100
#define P_LIFESPAN 50
#define P_RADIUS 1
#define SHADOW_RADIUS 0.5
#define SCALE_FACTOR 150
#define TRASL_VECTOR {0.5, -0.9}
#define SCALE_VECTOR {1, -1}


void Shadow::setup(Pose* pose_shadow)
{
	pose = pose_shadow;
	//sys_origin = pose->body_centroid; //pointer
	//shadow_origin = glm::make_vec2(sys_origin);

	//CENTROIDS are the first "POSE_CENTROID_NUM" JUNCTIONS
	for (int i = 0; i < POSE_JUNCTIONS + POSE_CENTROID_NUM; i++)
	{
		Circle temp_junction;
		temp_junction.setRadius(SHADOW_RADIUS);
		shadow_junctions.push_back(temp_junction);
	}
	
	//setup of the 2D particle systems 
	for (int i = 0; i < POSE_CENTROID_NUM; i++)
	{
		ParticleSystem2D temp_ps;
		temp_ps.setup(shadow_junctions[i].center, NUMPARTICLES, P_RADIUS, P_LIFESPAN);
		particle_systems_2d.push_back(temp_ps);

		//attractor setting: with this config, each ps it itself its attractor
		Circle* temp_attractor = &(shadow_junctions[i]); 
		shadow_attractors.push_back(temp_attractor);
		particle_systems_2d[i].setAttractors(shadow_attractors[i]);
	}

}


void Shadow::draw()
{
	
	for (int i = 0; i < shadow_junctions.size(); i++)
	{
		if (i < POSE_CENTROID_NUM)
			shadow_junctions[i].draw(ofFloatColor::red, 200); //centroids
		else
			shadow_junctions[i].draw(ofFloatColor::green, 200);//other junctions
	}
	
	
	for (int j = 0; j < particle_systems_2d.size(); j++)
	{
		particle_systems_2d[j].draw();
	}
	
}

void Shadow::moveCentroids()
{

	for (int i = 0; i < particle_systems_2d.size(); i++)
	{
		particle_systems_2d[i].moveOrigin(shadow_junctions[i].center); //each ps has its own centroid (which are the first CENTROID_NUM junctions)
	}

}

void Shadow::getCentroidsPositions()
{
	past_pose_centroids[FACE_CENTROID] = shadow_junctions[0]; 
	past_pose_centroids[BODY_CENTROID] = shadow_junctions[1];
	past_pose_centroids[LEFT_ARM_CENTROID] = shadow_junctions[2];
	past_pose_centroids[RIGHT_ARM_CENTROID] = shadow_junctions[3];
	past_pose_centroids[LEFT_LEG_CENTROID] = shadow_junctions[4];
	past_pose_centroids[RIGHT_ARM_CENTROID] = shadow_junctions[5];
}

bool Shadow::isCentroidMoving(int idx_centroid)
{
	if (shadow_junctions[idx_centroid].center != past_pose_centroids[idx_centroid].center)
		return true;
	else
		return false;
}



void Shadow::updateParticleSystems()
{
	for (int i = 0; i < particle_systems_2d.size(); i++)
		particle_systems_2d[i].update();
}

void Shadow::updateSysMaxVals(float ms, float mf)
{
	for (int i = 0; i < particle_systems_2d.size(); i++)
		particle_systems_2d[i].updateParticleMaxVals(ms, mf);
}

void Shadow::moveJunctions()
{
	glm::vec2 scale_vec(SCALE_VECTOR);
	scale_vec *= SCALE_FACTOR;
	glm::vec2 trasl_vec(TRASL_VECTOR);

	//centroids
	shadow_junctions[0].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->face_centroid)));
	shadow_junctions[1].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->body_centroid)));
	shadow_junctions[2].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_arm_centroid)));
	shadow_junctions[3].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_arm_centroid)));
	shadow_junctions[4].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_leg_centroid)));
	shadow_junctions[5].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_leg_centroid)));

	//face
	shadow_junctions[6].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->nose)));
	shadow_junctions[7].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_eye_inner)));
	shadow_junctions[8].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_eye)));
	shadow_junctions[9].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_eye_outer)));
	shadow_junctions[10].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_eye_inner)));
	shadow_junctions[11].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_eye)));
	shadow_junctions[12].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_eye_outer)));
	shadow_junctions[13].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_ear)));
	shadow_junctions[14].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_ear)));
	shadow_junctions[15].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->mouth_left)));
	shadow_junctions[16].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->mouth_right)));
	//shoulder
	shadow_junctions[17].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_shoulder)));
	shadow_junctions[18].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_shoulder)));
	//hip
	shadow_junctions[19].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_hip)));
	shadow_junctions[20].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_hip)));
	//left arm
	shadow_junctions[21].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_pinky)));
	shadow_junctions[22].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_index)));
	shadow_junctions[23].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_thumb)));
	shadow_junctions[24].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_elbow)));
	shadow_junctions[25].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_wrist)));
	//right harm
	shadow_junctions[26].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_pinky)));
	shadow_junctions[27].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_index)));
	shadow_junctions[28].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_thumb)));
	shadow_junctions[29].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_elbow)));
	shadow_junctions[30].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_wrist)));
	//left leg
	shadow_junctions[31].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_heel)));
	shadow_junctions[32].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_foot_index)));
	shadow_junctions[33].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_knee)));
	shadow_junctions[34].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->left_ankle)));
	//right leg
	shadow_junctions[35].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_heel)));
	shadow_junctions[36].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_foot_index)));
	shadow_junctions[37].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_knee)));
	shadow_junctions[38].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec2(pose->right_ankle)));

}







