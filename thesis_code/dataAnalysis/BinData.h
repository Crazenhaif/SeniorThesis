#ifndef BIN_HEADER
#define BIN_HEADER

#include "halo.h"


// Eventually put this stuff in Halo.h and Halo.cpp

// Prototypes

double * makeLinearBinEdges(double minimum, double maximum,int numberOfBins);
double * makeLogBinEdges(double minimum,double maximum,int numberOfBins);
int * histogramNumberPerBin(double * data, int numberOfRows, double * binEdges, int numberOfBins);
int * binHaloData(Particle ** particlesArray, int numberOfParticles, double * binEdges, int numberOfBins);
int * binHaloDataEnergy(Particle ** particlesArray, int numberOfParticles, double * binEdges, int numberOfBins);

#endif