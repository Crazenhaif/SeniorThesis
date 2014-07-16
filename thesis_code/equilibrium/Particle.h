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
	position_t centeredPosition;
	double potential;
	double radius;		//radius from the center of mass

		// Constructors
	Particle();

		// Member Functions.
	void centerPosition(position_t center);
	void removePotentialOffset(double maxPotential);
};

#endif
