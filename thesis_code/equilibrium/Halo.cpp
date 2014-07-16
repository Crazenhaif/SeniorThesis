#ifndef HALO_CPP
#define HALO_CPP

#include "halo.h"
#include "constants.h"
#include "arrayconstructor.h"
#include <cmath>

typedef Particle* ParticleArray;

static inline double cubed(double x) {
	return x*x*x;
}

Halo::Halo(datasource_t ds){
	datasource = ds;
	numberOfParticles = getNumberOfRows(datasource.positionFile);
	particlesArray = new ParticleArray[numberOfParticles];
	for (int i = 0; i < numberOfParticles; i++)
		particlesArray[i] = new Particle(); // (Particle*)malloc(sizeof(Particle)*numberOfParticles);
	

	double numberOfColumns = getNumberOfColumns(datasource.positionFile);							//count number of columns of data
	if(numberOfColumns != 3){cout <<"There are not three position coordinates" <<endl; system("PAUSE");}	//ensure we have 3 spacial coordinates
	double ** posArray = assembleArray(datasource.positionFile,numberOfParticles,numberOfColumns);
	double ** potArray = assembleArray(datasource.potentialFile,numberOfParticles,numberOfColumns);
	for (int i = 0; i<numberOfParticles;i++){
		particlesArray[i]->position.x = posArray[i][0];
		particlesArray[i]->position.y = posArray[i][1];
		particlesArray[i]->position.z = posArray[i][2];
		particlesArray[i]->potential = potArray[i][0];
	}


	for(int i = 0; i<numberOfParticles;i++){	//free memory
		free(posArray[i]);
		free(potArray[i]);
	}
	delete posArray;
	delete potArray;


//find the deepest potential location
	minPotential = particlesArray[0]->potential;
	int centerTracker = 0;
	for (int i = 1; i<numberOfParticles;i++){
		if(minPotential>particlesArray[i]->potential){
			minPotential=particlesArray[i]->potential;
			centerTracker = i;
		}
	}

	deepCenter = particlesArray[centerTracker]->position;	//center based on deepest potential
	
	ID = "xxxx";
	minRadius = 0;
	maxRadius =0;
	maxPotential = 0;

};

Halo::~Halo(){
	for (int i = 0; i < numberOfParticles; i++){
		delete particlesArray[i]; 
	}
	delete particlesArray;

};


void Halo::findCoM(){
	double xsum = 0;
	double ysum = 0;
	double zsum = 0;

	for(int i = 0; i < numberOfParticles; i++){
		xsum = xsum + particlesArray[i]->position.x;
		ysum = ysum + particlesArray[i]->position.y;
		zsum = zsum + particlesArray[i]->position.z;
	}
	centerOfMass.x = xsum/numberOfParticles;
	centerOfMass.y = ysum/numberOfParticles;
	centerOfMass.z = zsum/numberOfParticles;
	
}

void Halo::centerPositions(){
	int i;
	for(i = 0; i< numberOfParticles;i++){
		particlesArray[i]->centerPosition(centerOfMass);
	}

}

void Halo::findMaxMinRadius(){
	double tempMin = particlesArray[0]->radius;
	double tempMax = particlesArray[0]->radius;
	for(int i=0; i<numberOfParticles;i++){
		if (particlesArray[i]->radius<=tempMin){tempMin = particlesArray[i]->radius;}
	}

	for(int i=0; i<numberOfParticles;i++){
		if (particlesArray[i]->radius>=tempMax){tempMax = particlesArray[i]->radius;}
	}

	minRadius = tempMin;
	maxRadius = tempMax;
}


void Halo::findMaxPot(){

	double tempMax = particlesArray[0]->potential;
	for(int i=0; i<numberOfParticles;i++){
		if (particlesArray[i]->potential>=tempMax){tempMax = particlesArray[i]->potential;}
	}
	maxPotential = tempMax;
}

double Halo::findCritRadius(int resolution, double massMult, double critNum){
	int start = resolution/100;
	double density = 0;
	rCrit = 0;
	double tempSum;
	double particleMass = 6885000.0 * 1.989 * pow(10,33); //particle mass in grams/h
	double critDensity = 1.88*pow(10,-29); //critical density in gm * h^2/cm^3
	double MpcToCm = 3.0856*pow(10,24); //conversion factor from Mpc to cm
	double currentRadius = 0;
	double radiusStep = maxRadius/double(resolution);
	for(int i = start; i<resolution; i++){
		tempSum = 0;
		currentRadius = radiusStep*i;
		for(int j = 0; j<numberOfParticles; j++){
			if(particlesArray[j]->radius<currentRadius){
				tempSum += massMult*particleMass;
			}
			
		}
		density = tempSum/(4.0/3.0*pi*cubed(currentRadius*MpcToCm));
		if(density < critNum*critDensity){
			rCrit = currentRadius;
			break;
		}
		else{rCrit = currentRadius;}
		if(i == resolution - 1){cout <<"critical density never reached" <<endl;}
	}
	
	return rCrit;
}

void Halo::printOutput(outputfiles_t out){
	//printNewPosition(out.newPositonsFile);
	//printRadius(out.radiusFile);
	printNotes(out.notes);
	printRatio(out.ratio);
}

void Halo::printNewPosition(FILE * posFile){

	for(int i=0;i<numberOfParticles;i++){
			fprintf(posFile,"%f%c%f%c%f%c",particlesArray[i]->centeredPosition.x,delimeter,particlesArray[i]->centeredPosition.y,delimeter,particlesArray[i]->centeredPosition.z,delimeter);
		fprintf(posFile,"\n");
	}


}

void Halo::printRadius(FILE * radFile){

	for(int i=0;i<numberOfParticles;i++){
		fprintf(radFile,"%f%c\n",particlesArray[i]->radius,delimeter);
	}

}

void Halo::printNotes(FILE * notesFile){
	fprintf(notesFile,"number of particles = %d\n",numberOfParticles);
	fprintf(notesFile,"min radius = %f Mpc/h\n",minRadius);
	fprintf(notesFile,"max radius = %f Mpc/h\n",maxRadius);
	fprintf(notesFile,"Critical radius is %f Mpc/h from the CoM, which is %f of the largest radius \n", rCrit, ratio);
	fprintf(notesFile,"The radius from the CoM to the DeepCenter is %f Mpc/h, which is %f of the virial radius \n", radiusToDeepCenter, radiusToDeepCenter/rCrit);

}

void Halo::printRatio(FILE * ratioFile){

	fprintf(ratioFile,"%s %f",ID,ratio);
}

#endif
