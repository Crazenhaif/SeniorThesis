#ifndef FITTING_H
#define FITTING_H


#include <stdlib.h>
#include <iostream>
using namespace std;
#include <stdint.h>
#include <stdio.h> 
#include <time.h>  
#include "arrayconstructor.h"
#include <cmath>
#include "xorshift.h"
#include <math.h>
#include "filestuff.h"


struct fittingOutput_t{
	FILE * RMSFile;
	FILE * D_offsetFile;
	FILE * R_offsetFile;
	FILE * scaledLogRadiusFile;
	FILE * phi0LogDensityFile;
	FILE * scaledLogDensityFile;
	FILE * finalFitFile;
};


class Fit {

//member variables
public:
	int nrphi0;
	int nBins;
	double * phi0LogDensity;	// good
	double * matchedPhi0Density;
	double * phi0LogRadius;		// good
	double * logDensityData;		// good
	double * scaledLogDensityData;
	double * logRadiusData;		//good
	double * scaledLogRadiusData;
	double * densityRMSArray;
	double * D_offset;
	double * R_offset;
	int * indexArray;
	double * numberPerBinArray;
	int numberOfSteps;
	double totalRStep;
	double totalDStep;
	double tempRStep;
	double tempDStep;
	double RMS;
	double minRMS;
	double bestD;
	double bestR;
	double * bestSLR;
	double * bestSLD;
	double * bestPHI0;
	double acceptance;

//constructor
Fit(int nrphi0, int nBins, datasource_t ds, double massmult);


//destructor
~Fit();

//member function
int * findMatchingPoints(double * radius, double * phi0Data, int rows, int numberOfPhi0Rows);
double calculateRMS(double * logDensity, double * numberPerBin, double * phi0LogDensity, int * indexArray, int nBins);
int makeStep(double RMS, double previousRMS);
void initialize(int nbins, int nsteps);
fittingOutput_t makeFittingFileNames(string ID, string phi0char);
void printFit(fittingOutput_t fitOut);
void printRMS(FILE * RMSFile);
void printD(FILE *  D_offsetFile);
void printR(FILE * R_offsetFile);
void printslr(FILE * scaledLogRadiusFile);
void printsld(FILE * scaledLogDensityFile);
void printphi0(FILE * phi0LogDensityFile);
void printFinal(FILE * fitFile);
void closeFitFiles(fittingOutput_t fitOut);
};

struct energyFittingOutput_t{
	FILE * RMSArrayFile;
	FILE * phi0PathFile;
	FILE * scaleFactorPathFile;
	FILE * betaPathFile;
	FILE * finalFile;

};



class EnergyFit {

public:
	int nBins;
	double * energy;
	double * logN;
	double * Phi0Path;
	double * ScaleFactorPath;
	double * BetaPath;
	double * RMSArray;
	double phi0Value;
	double betaValue;
	int numberOfSteps;
	double scaleFactor;
	double tempScaleStep;
	double tempEnergyStep;
	double tempBetaValueStep;
	double RMS;
	double minRMS;
	double bestBeta;
	double bestPhi;
	double bestScale;
	double lastBinToInclude;
	double minEnergy;
	double * N;
	double firstEnergyBin;
	double deltaE;
	int extraBins;
	double acceptance;
	double maxEnergyStep;
	double maxBetaStep;
	double maxScaleStep;

//constructor
EnergyFit(int nBins, datasource_t ds);

//destructor
~EnergyFit();


//functions
double calculateRMSLogN();
double calculateRMSN(double * energy, double phi0Value, double currentScaleFactor, double * numberPerBin,double betaValue);
int makeStep(double RMS, double previousRMS);
energyFittingOutput_t makeFittingFileNames(string ID);
void printOutput(energyFittingOutput_t energyOut);
void printRMS(FILE * RMSFile);
void printPhi0(FILE * phi0File);
void printScale(FILE * scaleFile);
void printBeta(FILE * betaFile);
void printFinal(FILE * fitFile);
void closeEnergyFitFiles(energyFittingOutput_t energyOut);
void findLastBinToInclude(double * numberPerBin);
};


#endif
