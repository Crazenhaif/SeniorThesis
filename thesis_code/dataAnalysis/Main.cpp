#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <cmath>
#include "arrayconstructor.h"
#include "filestuff.h"
#include "BinData.h"
#include "halo.h"
#include "Particle.h"
#include "constants.h"
#include "xorshift.h"

using namespace std;

	// ================ Prototypes =================

double * radiusToDensity(int * numberPerBinRadius, double * binEdges, int numberOfBins);
//double ** getEnergy(int numberOfParticles, Velocity velocities, double ** potential);
//double * sphericallyAveragedPotential(double ** potential, double ** radius, double * binEdges, int numberOfParticles, int numberOfBins);
//double * virialRatio(double * binEdges, double ** radius, double ** newPotential, Velocity velocities, int numberOfBins, int numberOfParticles);

// ------------- definitions -------------------------//

//TO change from large to small data sets, change this directory, the MASS_MULTIPLIER, and the functions in makeSourceFileNames in filestuff.h
//#define ID_DIRECTORY_NAME "/home/user1/nolting/Desktop/datastuffs/id_directorylarge.txt"
#define ID_DIRECTORY_NAME "/home/user1/nolting/Desktop/datastuffs/id_directorysmall.txt"
#define MASS_MULTIPLIER 1 //change this to 1 for small halo range and 10 for large halo range

	// =================== Main ====================

int main() {

	
	///*
	// ============ open directories and files ==============
	FILE * idDirectoryFile = fopen(ID_DIRECTORY_NAME,"r");
	
	int numberOfHalos = getNumberOfRows(idDirectoryFile);					// count number of Halos

	/// --------------BEGIN FOR LOOP ----------------------///
	for(int n=0;n<numberOfHalos;n++){										// for each Halo do this:
		cout <<"start itteration " <<n <<endl;
		char * HaloID = readNextID(idDirectoryFile);							// read in Halo ID number
		datasource_t ds = makeSourceFileNames(HaloID);							// create all source file names from ID number
		Halo * currentHalo = new Halo(ds);										// initialize halo with data input from files
		closeFiles(ds);	
		currentHalo->ID = HaloID;												// assign the ID number to the Halo class object
		outputfiles_t output = makeOutputFileNames(currentHalo->ID);			// create the outputfiles from the ID number
	
//------------------POSITIONS-----------------------//
		
		// ============== center positions ==================
		currentHalo->centerPositions();			//subtract the position of the center (deepest potential) from each position to remove the offset.
												// this also calculates the radius of each particle after centering the positions
		
		//double substructure;					//find location in file where substructure begins
		//substructure = currentHalo->findSubstructure();
		
		currentHalo->findMaxMinRadius();  //find the minimum and maximum radius and assign their values for the halo		
		cout <<"Position done" <<endl;
//------------------VELOCITIES-----------------------//
		// ============== velocity offset removal =============

		currentHalo->haloVelocity = currentHalo->averageVel();				//find average velocity and assign it for the halo
		currentHalo->normalizeVelocity();									//subtract out this velocity offset from each particle
		// ALSO: this calculates and sets the particle's kinetic energy
		cout <<"Velocity done" <<endl;
//------------------POTENTIALS-----------------------//
		// ============= potential offset removal ============
		currentHalo->findMaxPot();											// find max potential and assign it for the halo
		
		//currentHalo->removePotentialOffset();								// subtract this max potential from each particle's potential effectively normalizing so the max potential is zero
		//^^This is no longer needed now that I'm using the potential values calculated using the W2 kernal
		cout <<"Potential done" <<endl;
// ----------------ENERGY --------------------------//
		
	// Now that both kinetic and potential energy are set:
		currentHalo->getEnergy();	//set energy for each particle = kinetic + potential (after offset removal)
		currentHalo->findMaxMinEnergy();	//find and set the min and max energies
		currentHalo->linEnergyBinEdges = makeLinearBinEdges(currentHalo->minEnergy,currentHalo->maxEnergy,NUMBER_OF_ENERGY_BINS);
		currentHalo->energyBins = binHaloDataEnergy(currentHalo->particlesArray,currentHalo->numberOfParticles,currentHalo->linEnergyBinEdges,NUMBER_OF_ENERGY_BINS);
		cout <<"Energy done" <<endl;
// -----------------DENSITY-------------------------//
		currentHalo->logBinEdges = makeLogBinEdges(currentHalo->minRadius,currentHalo->maxRadius,currentHalo->numberOfBins);
		currentHalo->numberPerBin = binHaloData(currentHalo->particlesArray,currentHalo->numberOfParticles,currentHalo->logBinEdges,currentHalo->numberOfBins);
		currentHalo->densityBins = radiusToDensity(currentHalo->numberPerBin,currentHalo->logBinEdges,currentHalo->numberOfBins);
		cout <<"Density done" <<endl;
// ------------------Virial Ratio ----------------- //
		currentHalo->linRadiusBinEdges = makeLinearBinEdges(currentHalo->minRadius,currentHalo->maxRadius,currentHalo->numberOfBins);
		currentHalo->getCumulativeVR(); //find and calculate the virial ratio by linear bin, store in array to print. Store final VR value.
		cout <<"VR done" <<endl;
// ----------------Spherically averaged potential -------------//
//		currentHalo->getSphericallyAveragedPotential();
//		cout <<"Average Potential done" <<endl;
// -----------------PRINT OUTPUT ----------------------------//
		currentHalo->printOutput(output);			//print all the output files
	
		cout <<"data iteration " <<n <<" has completed" <<endl;						
		//tell user when the data manipulation part is done, and that the fitting will begin, as the program runs.
		
		closeFiles(output);		//close halo output files
		delete currentHalo;
	} //end for halo loop, repeat for each halo.
	
	fclose(idDirectoryFile);			//close ID directory File
	cout <<"Program has reached completion" <<endl;		//note that the final iteration has completed and the program will terminate
	//system("PAUSE");	// System pause command to keep program window from closing.
	return 0;
}



double * radiusToDensity(int * numberPerBinRadius, double * binEdges, int numberOfBins){
	double * densityBins = (double *) malloc(sizeof(double)*numberOfBins);
	memset(densityBins, 0, sizeof(densityBins)*numberOfBins);
	double currentRadius = 0;
	double currentStep = 0;

	for(int i=0;i<numberOfBins;i++){
		currentRadius = (binEdges[i]+binEdges[i+1])/2.0;
		currentStep = binEdges[i+1]-binEdges[i];
		densityBins[i] = numberPerBinRadius[i] * 6.885 * MASS_MULTIPLIER *pow(10.0,6.0)/(4*pi*pow(currentRadius,2)*currentStep);
	}

	return densityBins;
}
