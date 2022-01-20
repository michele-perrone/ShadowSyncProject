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

	void getCentroidsPositions();
	bool isCentroidMoving(int idx_centroid);

	void updateSysMaxVals(float ms, float mf);
	
private:
    vector <ofSpherePrimitive> body_junctions;
    vector <ParticleSystem> particle_systems;
	glm::vec3 body_origin;
	float* sys_origin;
	vector <ofSpherePrimitive*> body_attractors;
	Pose* pose;
	ofSpherePrimitive past_pose_centroids[POSE_CENTROID_NUM];
	ofMaterial j_material;
	ofMaterial c_material;
};
