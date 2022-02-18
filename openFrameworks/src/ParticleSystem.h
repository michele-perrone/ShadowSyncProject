#pragma once
//object that handles a 3D particle system
#include "ofMain.h"
#include "Particle3D.h"

/*
* un metodo setOrigin(ofNode& origin); -> le particelle partono da pti diversi in un raggio random a partire dal node origin
* un metodo setDestintion(ofNode& destination); -> inizialmente traiettoria rettilinea, successivamente lungo altre curve 3D ~ elicoidali
* si deve poter cambiare destinazione in corso d'opera (es. movimenti troppo veloci -> destinazione: outer_space ~ random force)
*/

class ParticleSystem
{
public:
    void setup(glm::vec3 origin, int numParticles, int particleRadius, int particleLifespan);
    void update();
    void draw();
    void setMaterial(ofMaterial& material);
    void setOrigin(int x, int y, int z);
    void moveOrigin(glm::vec3 newOrigin);
    void setDestination(int x, int y, int z);
    void moveDestination(int x_dir, int y_dir, int z_dir);
    void addParticle();
    void decay();
    void updateParticleMaxVals(float ms, float mf);
    void setAttractor(ofSpherePrimitive* attractor);
    int origin_idx_in_body_junction_domain;
private:
    int numParticles, particleRadius, particleLifespan;
    glm::vec3 origin_PS;
    glm::vec3 destination;
    vector <Particle3D> particles;
    ofColor ps_color;
    ofSpherePrimitive* attractor;
};
