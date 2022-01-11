#include "Body.h"
#define NUMPARTICLES 100
#define P_LIFESPAN 200
#define P_RADIUS 1
#define SCALE_FACTOR 150
#define TRASL_VECTOR {0, -1.5, -0.5}
#define SCALE_VECTOR {1, -1, -1}



void Body::setup(Pose* pose_body)
{
    this->pose = pose_body;
	
    material.setDiffuseColor(ofFloatColor::aqua);
    //for (int j=0; j< POSE_CENTROIDS; j++) //set up all the centroids, parents of their junctions (?)
	com.set(3, 10); //center of mass = body_centroid of pose

    for (int i = 0; i < POSE_JUNCTIONS; i++) //centroid already set in order to call .setParent method
    {
		//vector of junctions
		ofSpherePrimitive temp_junction;
		temp_junction.set(2, 10);
        //temp_junction.setParent(com);
		body_junctions.push_back(temp_junction);
	}
    //children nodes can move independetly
	/*body_junctions[0].move(0, 50, 0);	//head
	body_junctions[1].move(-30, 40, 0); //left shoulder
	body_junctions[2].move(30, 40, 0);  //right shoulder
	body_junctions[3].move(-20, -30, 0);//left hip (anca)
	body_junctions[4].move(20, -30, 0); //rigth hip
    */
    // Create all the particle systems and store them inside the vector.
    // (For now, we have just one particle system located a the center of mass)
    /*ParticleSystem com_particleSystem;
    com_particleSystem.setup(this->getOrigin(), NUMPARTICLES, P_RADIUS, P_LIFESPAN);
    particle_systems.push_back(com_particleSystem);*/
}

ofNode& Body::getOrigin()
{
    return com;
}

void Body::draw()
{
	com.draw(); //not in same loop just for having a different (white, default) color for com

    material.begin(); //this will affect all the elements inside (like push/pop matrix)
    for (int i = 0; i < body_junctions.size(); i++)
		body_junctions[i].draw();
    material.end();

    /*for (int i = 0; i < particle_systems.size(); i++)
    {
        particle_systems[i].draw();
    }*/
}

//moves the entire body rigidly
void Body::move_centroid(float x_dir, float y_dir, float z_dir)
{
    //if a parent node moves --> all child nodes will follow
    com.move(x_dir, y_dir, z_dir);

    //... and also the particle sources move
    /*for (int i = 0; i < particle_systems.size(); i++)
    {
        particle_systems[i].moveOrigin(x_dir, y_dir, z_dir);
    }*/
}

void Body::move_centroid()
{
    com.setGlobalPosition(SCALE_FACTOR * glm::make_vec3(pose->body_centroid));
}

void Body::move_junctions()
{
    //once the pose elems are updated via osc, we set the body junctions accordingly
    glm::vec3 scale_vec(SCALE_VECTOR);
    scale_vec *= SCALE_FACTOR;
    glm::vec3 trasl_vec(TRASL_VECTOR);

    //face
    body_junctions[0].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->nose)));
    body_junctions[1].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_eye_inner)));
    body_junctions[2].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_eye)));
    body_junctions[3].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_eye_outer)));
    body_junctions[4].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_eye_inner)));
    body_junctions[5].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_eye)));
    body_junctions[6].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_eye_outer)));
    body_junctions[7].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_ear)));
    body_junctions[8].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_ear)));
    body_junctions[9].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->mouth_left)));
    body_junctions[10].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->mouth_right)));
    //shoulder
    body_junctions[11].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_shoulder)));
    body_junctions[12].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_shoulder)));
    //hip
    body_junctions[13].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_hip)));
    body_junctions[14].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_hip)));
    //left arm
    body_junctions[15].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_pinky)));
    body_junctions[16].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_index)));
    body_junctions[17].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_thumb)));
    body_junctions[18].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_elbow)));
    body_junctions[19].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_wrist)));
    //right harm
    body_junctions[20].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_pinky)));
    body_junctions[21].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_index)));
    body_junctions[22].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_thumb)));
    body_junctions[23].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_elbow)));
    body_junctions[24].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_wrist)));
    //left leg
    body_junctions[25].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_heel)));
    body_junctions[26].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_foot_index)));
    body_junctions[27].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_knee)));
    body_junctions[28].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_ankle)));
    //right leg
    body_junctions[29].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_heel)));
    body_junctions[30].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_foot_index)));
    body_junctions[31].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_knee)));
    body_junctions[32].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_ankle)));

}


void Body::updateParticleSystems()
{
    for(int i = 0; i < particle_systems.size(); i++)
    {
        particle_systems[i].update();
    }
}
