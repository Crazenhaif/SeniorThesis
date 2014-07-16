#include "BinData.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Functions


double * makeLinearBinEdges(double minimum,double maximum,int numberOfBins){
	double * binEdges = (double*)malloc(sizeof(double)*(numberOfBins+1));
	binEdges[0]=minimum;
	double range = maximum - minimum;
	double binWidth = range/(double)numberOfBins;
	for(int i=1;i<(numberOfBins+1);i++){
		binEdges[i]=binEdges[i-1]+binWidth;
	}

	return binEdges;
}

double * makeLogBinEdges(double minimum,double maximum,int numberOfBins){
	double * binEdges = (double*)malloc(sizeof(double)*(numberOfBins+1));
	binEdges[0]=minimum;
	double range = maximum - minimum;
	double multiplier = 0;
	for(int i=0; i<numberOfBins ;i++) {multiplier = multiplier + pow(1.01,i);}
	double initialStep = range/multiplier;
	for(int j=1;j<numberOfBins+1;j++){
		binEdges[j]=binEdges[j-1]+initialStep*pow(1.01,j-1);
	}


	return binEdges;
}

int * histogramNumberPerBin(double * data, int numberOfRows, double * binEdges, int numberOfBins){
	int * numberPerBin = (int *) malloc(sizeof(int)*numberOfBins);
	for(int n=0;n<numberOfBins;n++){numberPerBin[n]=0;}
	for(int i=0;i<numberOfRows;i++){
		for(int j=0;j<numberOfBins;j++){
			if(data[i]>binEdges[j] && data[i]<binEdges[j+1]){numberPerBin[j]++;}
		}
	}
	return numberPerBin;
}

int * binHaloData(Particle ** particlesArray, int numberOfParticles, double * binEdges, int numberOfBins){
	int * numberPerBin = (int *) malloc(sizeof(int)*numberOfBins);
	for(int n=0;n<numberOfBins;n++){numberPerBin[n]=0;}
	for(int i=0;i<numberOfParticles;i++){
		for(int j=0;j<numberOfBins;j++){
			if(particlesArray[i]->radius>binEdges[j] && particlesArray[i]->radius<binEdges[j+1]){numberPerBin[j]++;}
		}
	}
	return numberPerBin;

}

int * binHaloDataEnergy(Particle ** particlesArray, int numberOfParticles, double * binEdges, int numberOfBins){
	int * numberPerBin = (int *) malloc(sizeof(int)*numberOfBins);
	for(int n=0;n<numberOfBins;n++){numberPerBin[n]=0;}
	for(int i=0;i<numberOfParticles;i++){
		for(int j=0;j<numberOfBins;j++){
			if(particlesArray[i]->energy>binEdges[j] && particlesArray[i]->energy<binEdges[j+1]){numberPerBin[j]++;}
		}
	}
	return numberPerBin;

}