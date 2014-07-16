#ifndef PARTICLE_H
#define PARTICLE_H

#include <cmath>

struct position_t {
	double x;
	double y;
	double z;
};

class Particle {
public:

		// Instance Variables
	position_t position;
	double directPotential;

		// Constructors
	Particle();
		// Member Functions.
};

#endif
