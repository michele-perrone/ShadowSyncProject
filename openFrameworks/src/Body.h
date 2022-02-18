#pragma once
#include "ofMain.h"
#include "ParticleSystem.h"
#include "pose.h"
#include "Utils.h"

class Body
{
public:
	void setup(Pose* pose_body);
	void draw();
	void moveCentroids();
	void moveJunctions();

    void updateParticleSystems();

	void savePastCentroidsPositions();
	bool isCentroidMoving(int idx_centroid);
	bool areCentroidMoving();

	void updateSysMaxVals(float ms, float mf);
	void EmitterAttractorSetup();
	void setupEA(int e, int a);
	
private:
    vector <ofSpherePrimitive> body_junctions;
    vector <ParticleSystem> particle_systems;	//movement
	vector <ParticleSystem> particle_systems_nm; //non movevement
	glm::vec3 body_origin;
	float* sys_origin;
	Pose* pose;
	ofSpherePrimitive past_pose_centroids[POSE_CENTROID_NUM];
	ofMaterial j_material;
	ofMaterial c_material;
	int isCentroidMovingFlag10Frames = 0;

};
