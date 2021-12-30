#pragma once
//object that handles a 3D particle system
#include "ofMain.h"
#include "Particle3D.h"

/*
* un metodo setOrigin(ofNode& origin); -> le particelle partono da pti diversi in un raggio random a partire dal node origin
* un metodo setDestintion(ofNode& destination); -> inizialmente traiettoria rettilinea, successivamente lungo altre curve 3D ~ elicoidali
* si deve poter cambiare destinazione in corso d'opera (es. movimenti troppo veloci -> destinazione: outer_space ~ random force)
*/
