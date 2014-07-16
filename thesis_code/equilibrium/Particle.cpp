#ifndef PARTICLE_CPP
#define PARTICLE_CPP

#include <iostream>
#include "Particle.h"
using namespace std;

static inline double sq(double x) {
	return x*x;
}

Particle::Particle(){
	position.x=0;
	position.y=0;
	position.z=0;
	centeredPosition.x=0;
	centeredPosition.y=0;
	centeredPosition.z=0;
	potential=0;
	radius=0;

};

/*
 * Method to center the position of a Particle in realtion to the particle 
 * that is designated as the center.
 *
 * position_t center: the position struct that is designated as the center.
 * also calculates the radius of the particle after it has been centered.
 */
void Particle::centerPosition(position_t centerOfMass) {
	centeredPosition.x = position.x - centerOfMass.x;
	centeredPosition.y = position.y - centerOfMass.y;
	centeredPosition.z = position.z - centerOfMass.z;
	radius = sqrt(sq(centeredPosition.x) + sq(centeredPosition.y) + sq(centeredPosition.z));
	
}

#endif
