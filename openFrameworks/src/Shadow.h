#pragma once
#include "ofMain.h"
#include "ParticleSystem2D.h"
#include "Circle.h"
#include "pose.h"
#include "Utils.h"

class Shadow
{
public:
	void setup(Pose* pose_shadow); 
	void draw();
	void moveCentroids();
	void moveJunctions();

	void updateParticleSystems();

	void getCentroidsPositions();
	bool isCentroidMoving(int idx_centroid);
	
	void updateSysMaxVals(float ms, float mf);
	void EmitterAttractorSetup();
	void setupEA(int e, int a);

private:
	vector<Circle> shadow_junctions;
	vector<ParticleSystem2D> particle_systems_2d;
	vector<ParticleSystem2D> particle_systems_2d_nm;
	glm::vec2 shadow_origin;
	float* sys_origin;
	Pose* pose;
	Circle past_pose_centroids[POSE_CENTROID_NUM];
};
