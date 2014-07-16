#ifndef HALO_H
#define HALO_H

#include <stdio.h>
#include "Particle.h"
#include <stdlib.h>
#include <iostream>


using namespace std;

struct datasource_t {		//source file pointers
	FILE * positionFile;	//positoin file
	FILE * velocityFile;	//velocity file
	FILE * potentialFile;	//potential file
};

struct outputfiles_t {			//output file pointers
	//FILE * newPositonsFile;		//position after subtraction of first particle's position (centering)
	//FILE * newVelocitiesFile;	//velocity after subtraction of average halo velocity	
	//FILE * newPotentialsFile;	//potential after subtraction of maximum potential 
	FILE * radiusFile;			//radius from center after postion centering
	FILE * densityFile;			//density distribution divided binned logrithmically with radius
	FILE * linEnergyBins;		//linear energy bin edges
	FILE * linRadBins;			//linear radius bin edges
	FILE * logRadBins;			//logrithmic radius bin edges
	//FILE * kinetic;				//kinetic energy of each particle
	//FILE * energy;				//total energy of each particle
	FILE * notes;				//notes about the details of the halo
	//FILE * sphericallyAveragedPotential;	//spherically averaged potential by linear radius bin
	FILE * cumulativeVirialRatio;			//cumulative virial ratio by linear radius bin
	FILE * energyBinsFile;					//number of particles per linearly binned energy bin
	FILE * numberPerBinRadiusFile;			//number of particles per logrithmically binned radius bin (for density)
};

class Halo {
public:

		// Instance Variables / properties of the halo
	char * ID;					//4 digit ID number
	int numberOfParticles;		//count total number of particles in the halo
	Particle ** particlesArray;	//where particle information will be stored
	position_t center;			//position object of the center of the halo (x, y, z)
	velocity_t haloVelocity;	//velocity object for the halo's average velocity (Vx, Vy, Vz)
	datasource_t datasource;	//where the data comes from (pointers to the file locations)
	double minRadius;			//record the radius of the particle closest to the center (will be zero since we use the first particle's location as the center)
	double maxRadius;			//radius of the particle furthest from the center
	double minEnergy;			//energy of the particle with minimum total energy
	double maxEnergy;			//energy of the particel with maximum total energy
	double maxPotential;		//particle with largest (least bound) potential (BEFORE SUBTRACTION)
	double minPotential;		//particle with smallest (most bound) potential (BEFORE SUBTRACTION)
	double finalVR;				//Virial ratio for the halo, from the cumulative virial ratio once all particles are included
	int numberOfBins;			//number of bins used in binning linearly and logrithmically (radius and energy) by default 500 - UPDATE: linear energy binning now uses 2000 bins
	int * numberPerBin;			//number of particles in the log radius bins, to be used to find the density distribution and again as a weight for RMS calculation
	double * densityBins;		//density distribution by log radius bin
	int * energyBins;			//number per linear energy bin
	double * linEnergyBinEdges;	//bin edges for the linear energy bins
	double * linRadiusBinEdges;	//bin edges for the linear radius bins
	double * logBinEdges;		//bin edges for the logrithmic radius bins
	double * cumulativeVR;		//cumulative VR per linear radius bin
	double * averagePotential;	//spherically averaged potential (after subtraction) per linear radius bin

		// Constructors
	Halo();
	Halo(datasource_t ds);

		// Destructors
	~Halo();

		
		// Member Functions.
	
	velocity_t averageVel();
	void centerPositions();
	void findMaxMinRadius();
	void findMaxPot();
	void normalizeVelocity();
	void removePotentialOffset();
	void getEnergy();
	void findMaxMinEnergy();
	void getCumulativeVR();
	void getSphericallyAveragedPotential();
	void printOutput(outputfiles_t);
	void printNewPosition(FILE * posFile);
	void printNewVelocity(FILE * velFile);
	void printNewPotential(FILE * potFile);
	void printRadius(FILE * radFile);
	void printDensity(FILE * densityFile);
	void printEnergyBins(FILE * energyBinsFile);
	void printLinEnergyBins(FILE * linEnergyBinsFile);
	void printLinRadBins(FILE * linRadBinsFile);
	void printLogRadBins(FILE * logRadBinsFile);
	void printEnergy(FILE * energyFile);
	void printNotes(FILE * notesFile);
	void printSphericallyAveragedPotential(FILE * sphericallyAveragedPotentialFile);
	void printCumulativeVirialRatio(FILE * CumulativeVirialRatioFile);
	void printKinetic(FILE * kineticFile);
	double findSubstructure();
	void printNumberPerRadiusBin(FILE * numberPerBinRadiusFile);

};



#endif
