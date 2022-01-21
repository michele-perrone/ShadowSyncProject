#include "Body.h"
#define NUMPARTICLES 30
#define P_LIFESPAN 200
#define P_RADIUS 1
#define BODY_RADIUS 2
#define SCALE_FACTOR 150
#define TRASL_VECTOR {0, -0.9, -0.5}
#define SCALE_VECTOR {1, -1, -0.8}

void Body::setup(Pose* pose_body)
{
    j_material.setDiffuseColor(ofColor::aqua);
    c_material.setDiffuseColor(ofColor::purple);
    pose = pose_body;
   // sys_origin = pose->body_centroid;
   // body_origin = glm::make_vec3(sys_origin);

    for (int i = 0; i < POSE_JUNCTIONS + POSE_CENTROID_NUM; i++) //fixed size!
    {
		ofSpherePrimitive temp_junction;
		temp_junction.set(BODY_RADIUS, 10);
        //temp_junction.setParent(com);
		body_junctions.push_back(temp_junction);
	}
    
    for (int i = 0; i < POSE_CENTROID_NUM; i++) 
    {
        ParticleSystem temp_ps;
        temp_ps.setup(body_junctions[i].getGlobalPosition(), NUMPARTICLES, P_RADIUS, P_LIFESPAN);
        particle_systems.push_back(temp_ps);

        //attractor setting: with this config, each ps it itself its attractor
        ofSpherePrimitive* temp_attractor = &(body_junctions[i]);
        body_attractors.push_back(temp_attractor);
        particle_systems[i].setAttractors(body_attractors[i]);
    }
  
}


void Body::draw()
{
    c_material.begin();
    for (int i = 0; i < POSE_CENTROID_NUM; i++)
        body_junctions[i].draw();
    c_material.end();
	

    j_material.begin(); //this will affect all the elements inside (like push/pop matrix)
    for (int i = POSE_CENTROID_NUM; i < body_junctions.size(); i++)
        body_junctions[i].draw();
    j_material.end();

    for (int i = 0; i < particle_systems.size(); i++)
    {
        particle_systems[i].draw();
    }
}

//moves the entire body rigidly
void Body::moveCentroids()
{
    for (int i = 0; i < particle_systems.size(); i++)
    {
        particle_systems[i].moveOrigin(body_junctions[i].getGlobalPosition());
      //  std::cout << "Move centroids: " << body_junctions[i].getGlobalPosition() << std::endl;
    }
}


void Body::getCentroidsPositions()
{
    past_pose_centroids[FACE_CENTROID] = body_junctions[0];
    past_pose_centroids[BODY_CENTROID] = body_junctions[1];
    past_pose_centroids[LEFT_ARM_CENTROID] = body_junctions[2];
    past_pose_centroids[RIGHT_ARM_CENTROID] = body_junctions[3];
    past_pose_centroids[LEFT_LEG_CENTROID] = body_junctions[4];
    past_pose_centroids[RIGHT_ARM_CENTROID] = body_junctions[5];
}

bool Body::isCentroidMoving(int idx_centroid)
{
    if (body_junctions[idx_centroid].getGlobalPosition() != past_pose_centroids[idx_centroid].getGlobalPosition())
        return true;
    else
        return false;
}

void Body::updateParticleSystems()
{
    for (int i = 0; i < particle_systems.size(); i++)
    {
        particle_systems[i].update();
    }
}

void Body::updateSysMaxVals(float ms, float mf)
{
    for (int i = 0; i < particle_systems.size(); i++)
        particle_systems[i].updateParticleMaxVals(ms, mf);
}

void Body::moveJunctions()
{
    //once the pose elems are updated via osc, we set the body junctions accordingly
    glm::vec3 scale_vec(SCALE_VECTOR);
    scale_vec *= SCALE_FACTOR;
    glm::vec3 trasl_vec(TRASL_VECTOR);


    //centroids
    body_junctions[0].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec3(pose->face_centroid)));
    body_junctions[1].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec3(pose->body_centroid)));
    body_junctions[2].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec3(pose->left_arm_centroid)));
    body_junctions[3].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec3(pose->right_arm_centroid)));
    body_junctions[4].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec3(pose->left_leg_centroid)));
    body_junctions[5].setGlobalPosition(scale_vec * (trasl_vec + glm::make_vec3(pose->right_leg_centroid)));
    
    //face
    body_junctions[6].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->nose)));
    body_junctions[7].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_eye_inner)));
    body_junctions[8].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_eye)));
    body_junctions[9].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_eye_outer)));
    body_junctions[10].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_eye_inner)));
    body_junctions[11].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_eye)));
    body_junctions[12].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_eye_outer)));
    body_junctions[13].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_ear)));
    body_junctions[14].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_ear)));
    body_junctions[15].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->mouth_left)));
    body_junctions[16].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->mouth_right)));
    //shoulder
    body_junctions[17].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_shoulder)));
    body_junctions[18].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_shoulder)));
    //hip
    body_junctions[19].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_hip)));
    body_junctions[20].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_hip)));
    //left arm
    body_junctions[21].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_pinky)));
    body_junctions[22].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_index)));
    body_junctions[23].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_thumb)));
    body_junctions[24].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_elbow)));
    body_junctions[25].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_wrist)));
    //right harm
    body_junctions[26].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_pinky)));
    body_junctions[27].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_index)));
    body_junctions[28].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_thumb)));
    body_junctions[29].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_elbow)));
    body_junctions[30].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_wrist)));
    //left leg
    body_junctions[31].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_heel)));
    body_junctions[32].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_foot_index)));
    body_junctions[33].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_knee)));
    body_junctions[34].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->left_ankle)));
    //right leg
    body_junctions[35].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_heel)));
    body_junctions[36].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_foot_index)));
    body_junctions[37].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_knee)));
    body_junctions[38].setGlobalPosition(scale_vec * (trasl_vec +glm::make_vec3(pose->right_ankle)));

}

