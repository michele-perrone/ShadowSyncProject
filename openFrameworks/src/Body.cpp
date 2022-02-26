#include "Body.h"
#define NUMPARTICLES 30
#define NUMPARTICLESNM 20
#define P_LIFESPAN 200
#define P_RADIUS 2
#define BODY_RADIUS 2
#define SCALE_FACTOR 150
#define TRASL_VECTOR {0, -0.9, -0.5}
#define SCALE_VECTOR {1, -1, -0.4}

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
    
    /*for (int i = 0; i < POSE_CENTROID_NUM; i++)
    {
        ParticleSystem temp_ps;
        temp_ps.setup(body_junctions[i].getGlobalPosition(), NUMPARTICLES, P_RADIUS, P_LIFESPAN);
        particle_systems.push_back(temp_ps);
        particle_systems[i].setAttractor(&(body_junctions[i])); //ps attracted by itself, pose moving 

    }*/
    
    EmitterAttractorSetup();
  
}


void Body::draw()
{
    /*
    c_material.begin();
    for (int i = 0; i < POSE_CENTROID_NUM; i++)
        body_junctions[i].draw();
    c_material.end();
	

    j_material.begin(); //this will affect all the elements inside (like push/pop matrix)
    for (int i = POSE_CENTROID_NUM; i < body_junctions.size(); i++)
        body_junctions[i].draw();
    j_material.end();*/
    
   /* for (int i = 0; i < particle_systems.size(); i++)
        particle_systems[i].draw();*/
    
    
    for (int j = 0; j < particle_systems_nm.size(); j++)
        particle_systems_nm[j].draw();
        
}

//moves the entire body rigidly
void Body::moveCentroids()
{
    /*for (int i = 0; i < particle_systems.size(); i++)
    {
        particle_systems[i].moveOrigin(body_junctions[i].getGlobalPosition());
      //  std::cout << "Move centroids: " << body_junctions[i].getGlobalPosition() << std::endl;
    }*/
    for (int j = 0; j < particle_systems_nm.size(); j++)
        particle_systems_nm[j].moveOrigin(body_junctions[particle_systems_nm[j].origin_idx_in_body_junction_domain].getGlobalPosition());
}


void Body::savePastCentroidsPositions()
{
    past_pose_centroids[FACE_CENTROID] = body_junctions[0];
    past_pose_centroids[BODY_CENTROID] = body_junctions[1];
    past_pose_centroids[LEFT_ARM_CENTROID] = body_junctions[2];
    past_pose_centroids[RIGHT_ARM_CENTROID] = body_junctions[3];
    past_pose_centroids[LEFT_LEG_CENTROID] = body_junctions[4];
    past_pose_centroids[RIGHT_LEG_CENTROID] = body_junctions[5];
}

bool Body::isCentroidMoving(int idx_centroid)
{
    if (body_junctions[idx_centroid].getGlobalPosition() != past_pose_centroids[idx_centroid].getGlobalPosition())
        return true;
    else
        return false;
}

bool Body::areCentroidMoving()
{
    for (int i = 0; i < POSE_CENTROID_NUM; i++)
    {
        if (body_junctions[i].getGlobalPosition() != past_pose_centroids[i].getGlobalPosition())
        {
            isCentroidMovingFlag10Frames = 0;
            return true;
        }
    }
    isCentroidMovingFlag10Frames += 1;  //for this frame, the centroids position is fixed
    if (isCentroidMovingFlag10Frames >= 10)
        return false;
    else
        return true;
}

void Body::updateParticleSystems()
{
    //for (int i = 0; i < POSE_CENTROID_NUM; i++)
    //    particle_systems[i].update();
    
    //MOVING VS NON-MOVING POSE HP
    /*if (areCentroidMoving()) //activate centroids PS, where EMI = ATT
    {
        std::cout << "centroid is moving" << std::endl;
        for (int i = 0; i < POSE_CENTROID_NUM; i++)
            particle_systems[i].update();
        for (int j = 0; j < particle_systems_nm.size(); j++)
        {
            particle_systems_nm[j].decay();
        }
    }
    else    //activate other PS, where EMI != ATT
    {
        std::cout << "centroid is NOT moving" << std::endl;*/
        for (int j = 0; j < particle_systems_nm.size(); j++)
            particle_systems_nm[j].update();
        /*for (int i = 0; i < POSE_CENTROID_NUM; i++)
        {
            particle_systems[i].decay();
        }*/

    
        
}

void Body::updateSysMaxVals(float ms, float mf)
{
    /*for (int i = 0; i < particle_systems.size(); i++)
        particle_systems[i].updateParticleMaxVals(ms, mf);*/
    for (int j = 0; j < particle_systems_nm.size(); j++)
        particle_systems_nm[j].updateParticleMaxVals(ms, mf);
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
    //right arm
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

void Body::EmitterAttractorSetup()
{
//remember: the first 6 body_junctions are = centroids -> if A/E is a centroid, modify accordingly the idx
    //head
    setupEA(LEFT_EAR, RIGHT_EAR);
    setupEA(NOSE, BODY_CENTROID - POSE_CENTROID_NUM);

    //chest
    setupEA(BODY_CENTROID - POSE_CENTROID_NUM, LEFT_SHOULDER);
    setupEA(BODY_CENTROID - POSE_CENTROID_NUM, RIGHT_SHOULDER);
    setupEA(BODY_CENTROID - POSE_CENTROID_NUM, LEFT_HIP);
    setupEA(BODY_CENTROID - POSE_CENTROID_NUM, RIGHT_HIP);

    //left arm
    setupEA(LEFT_SHOULDER, LEFT_ELBOW);
    setupEA(LEFT_ELBOW, LEFT_WRIST);
    
    //right arm
    setupEA(RIGHT_SHOULDER, RIGHT_ELBOW);
    setupEA(RIGHT_ELBOW, RIGHT_WRIST);

    //left leg
    setupEA(LEFT_HIP, LEFT_KNEE);
    setupEA(LEFT_KNEE, LEFT_HEEL);

    //right leg
    setupEA(RIGHT_HIP, RIGHT_KNEE);
    setupEA(RIGHT_KNEE, RIGHT_HEEL);
}


void Body::setupEA(int e , int a)
{
    ParticleSystem temp_ps;
    temp_ps.setup(body_junctions[POSE_CENTROID_NUM + e].getGlobalPosition(), NUMPARTICLESNM, P_RADIUS, P_LIFESPAN);
    particle_systems_nm.push_back(temp_ps);
    particle_systems_nm[particle_systems_nm.size() - 1].origin_idx_in_body_junction_domain = POSE_CENTROID_NUM + e; //setup its origin, see moveCentroids()
    particle_systems_nm[particle_systems_nm.size() - 1].setAttractor(&(body_junctions[POSE_CENTROID_NUM + a]));
}

