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

private:
	Circle com_shadow; //cointains the shadow sys origin
	vector<Circle> shadow_junctions;
	vector<ParticleSystem2D> particle_systems_2d;
	glm::vec2 shadow_origin;
	float* sys_origin;
	vector <Circle*> shadow_attractors; //should follow the defined attractor(that is a junction) position
	Pose* pose;
	Circle past_pose_centroids[POSE_CENTROID_NUM];
};
