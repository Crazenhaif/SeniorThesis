#ifndef PARTICLE_H
#define PARTICLE_H

#include <cmath>

struct velocity_t {
	double x;
	double y;
	double z;
};

struct position_t {
	double x;
	double y;
	double z;
};

class Particle {
public:

		// Instance Variables
	velocity_t velocity;
	velocity_t normalizedVelocity;
	position_t position;
	position_t centeredPosition;
	double potential;
	double newPotential;
	double radius;
	double energy;
	double kinetic;

		// Constructors
	Particle();

		// Member Functions.
	void centerPosition(position_t center);
	void normalizeVelocity(velocity_t velocity);
	void removePotentialOffset(double maxPotential);
};

#endif
