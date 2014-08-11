#ifndef HALO_H
#define HALO_H

#include <stdio.h>
#include "Particle.h"
#include <stdlib.h>
#include <iostream>


using namespace std;

struct datasource_t {		//source file pointers
	FILE * positionFile;	//positoin file
	FILE * potentialFile;	//potential file
};

struct outputfiles_t {			//output file pointers
	//FILE * newPositonsFile;		//position after subtraction of first particle's position (centering)
	//FILE * radiusFile;			//radius from center after postion centering
	FILE * notes;				//notes about the details of the halo
	FILE * ratio;
};


class Halo {
public:

		// Instance Variables / properties of the halo
	char * ID;					//4 digit ID number
	int numberOfParticles;		//count total number of particles in the halo
	Particle ** particlesArray;	//where particle information will be stored
	position_t deepCenter;			//position object of the center of the halo (by deepest potential) (x, y, z)
	position_t centerOfMass;		//position object of the center of the halo (by averaging particle positions) (x, y, z)
	datasource_t datasource;	//where the data comes from (pointers to the file locations)
	double minRadius;			//record the radius of the particle closest to the center (will be zero since we use the first particle's location as the center)
	double maxRadius;			//radius of the particle furthest from the center
	double maxPotential;		//particle with largest (least bound) potential (BEFORE SUBTRACTION)
	double minPotential;		//particle with smallest (most bound) potential (BEFORE SUBTRACTION)
	double radiusCoM;
	double rCrit;			//radius where the contained average density is the critical density
	double ratio;			//ratio rCrit/maxRadius
	double radiusToDeepCenter;	//radius from CoM to deepest potential
	int numberWithinSmoothing;	//keep track of how many particles are within 2.8 kpc of CoM

		// Constructors
	Halo();
	Halo(datasource_t ds);

		// Destructors
	 ~Halo();

		
		// Member Functions.
	
	void findCoM();
	void centerPositions();
	void findMaxMinRadius();
	void findMaxPot();
	double findCritRadius(int resolution, double massMult, double critNum);
	void printOutput(outputfiles_t);
	void printNewPosition(FILE * posFile);
	void printRadius(FILE * radFile);
	void printNotes(FILE * notesFile);
	void printRatio(FILE * ratiofile);
	void fixPositions();
	

};



#endif
