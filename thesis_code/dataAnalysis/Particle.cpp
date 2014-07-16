#ifndef PARTICLE_CPP
#define PARTICLE_CPP

#include <iostream>
#include "Particle.h"
using namespace std;

Particle::Particle(){
	velocity.x=0;
	velocity.y=0;
	velocity.z=0;
	normalizedVelocity.x=0;
	normalizedVelocity.y=0;
	normalizedVelocity.z=0;
	position.x=0;
	position.y=0;
	position.z=0;
	centeredPosition.x=0;
	centeredPosition.y=0;
	centeredPosition.z=0;
	potential=0;
	newPotential=0;
	radius=0;
	energy=0;
	kinetic=0;

};

/*
 * Method to center the position of a Particle in realtion to the particle 
 * that is designated as the center.
 *
 * position_t center: the position struct that is designated as the center.
 * also calculates the radius of the particle after it has been centered.
 */
void Particle::centerPosition(position_t center) {
	centeredPosition.x = position.x - center.x;
	centeredPosition.y = position.y - center.y;
	centeredPosition.z = position.z - center.z;
	radius = sqrt(centeredPosition.x*centeredPosition.x + centeredPosition.y*centeredPosition.y + centeredPosition.z*centeredPosition.z);
}

void Particle::normalizeVelocity(velocity_t haloVelocity){
	normalizedVelocity.x = velocity.x - haloVelocity.x;
	normalizedVelocity.y = velocity.y - haloVelocity.y;
	normalizedVelocity.z = velocity.z - haloVelocity.z;
	kinetic = (normalizedVelocity.x*normalizedVelocity.x + normalizedVelocity.y*normalizedVelocity.y + normalizedVelocity.z*normalizedVelocity.z)/2.0;
}

//this should not be used anymore
void Particle::removePotentialOffset(double maxPotential){
	newPotential = potential - maxPotential;
	cout <<"THIS SHOULD NOT HAPPEN" <<endl;
}

#endif