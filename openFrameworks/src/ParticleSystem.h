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
    void setup(ofNode& origin, int numParticles, int particleRadius, int particleLifespan);
    void update();
    void draw();
    void setMaterial(ofMaterial& material);
    void setOrigin(int x, int y, int z);
    void moveOrigin(int x_dir, int y_dir, int z_dir);
    void setDestination(int x, int y, int z);
    void moveDestination(int x_dir, int y_dir, int z_dir);

private:
    int numParticles, particleRadius, particleLifespan;
    ofMaterial material;
    ofNode origin;
    ofNode destination;

    vector <Particle3D> particles;
};
