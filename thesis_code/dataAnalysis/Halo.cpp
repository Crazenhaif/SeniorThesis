#ifndef HALO_CPP
#define HALO_CPP

#include "halo.h"
#include "constants.h"
#include "arrayconstructor.h"
#include "BinData.h"

typedef Particle* ParticleArray;


Halo::Halo(datasource_t ds){
	datasource = ds;
	numberOfParticles = getNumberOfRows(datasource.positionFile);
	particlesArray = new ParticleArray[numberOfParticles];
	for (int i = 0; i < numberOfParticles; i++)
		particlesArray[i] = new Particle(); // (Particle*)malloc(sizeof(Particle)*numberOfParticles);
	

	double numberOfColumns = getNumberOfColumns(datasource.positionFile);							//count number of columns of data
	if(numberOfColumns != 3){cout <<"There are not three position coordinates" <<endl;}	//ensure we have 3 spacial coordinates
	double ** posArray = assembleArray(datasource.positionFile,numberOfParticles,numberOfColumns);
	double ** velArray = assembleArray(datasource.velocityFile,numberOfParticles,numberOfColumns);
	double ** potArray = assembleArray(datasource.potentialFile,numberOfParticles,numberOfColumns);
	for (int i = 0; i<numberOfParticles;i++){
		particlesArray[i]->position.x = posArray[i][0];
		particlesArray[i]->position.y = posArray[i][1];
		particlesArray[i]->position.z = posArray[i][2];
		particlesArray[i]->velocity.x = velArray[i][0];
		particlesArray[i]->velocity.y = velArray[i][1];
		particlesArray[i]->velocity.z = velArray[i][2];
		particlesArray[i]->potential = potArray[i][0];
	}
	cout <<"particles array done" <<endl;


	for(int i = 0; i<numberOfParticles;i++){	//free memory
		free(posArray[i]);
		free(velArray[i]);
		free(potArray[i]);
	}
	delete posArray;
	delete velArray;
	delete potArray;

	minPotential = particlesArray[0]->potential;
	int centerTracker = 0;
	for (int i = 1; i<numberOfParticles;i++){
		if(minPotential>particlesArray[i]->potential){
			minPotential=particlesArray[i]->potential;
			centerTracker = i;
		}
	}

	center = particlesArray[centerTracker]->position;
	
	numberOfBins = NUMBER_OF_BINS;
	ID = "xxxx";
	minRadius = 0;
	maxRadius =0;
	minEnergy = 0;
	maxEnergy = 0;
	maxPotential = 0;
	finalVR = 0;
	haloVelocity.x =0;
	haloVelocity.y =0;
	haloVelocity.z =0;
	densityBins=0;
	linRadiusBinEdges=0;
	linEnergyBinEdges=0;
	logBinEdges=0;
	cumulativeVR=0;
	averagePotential=0;

};

Halo::~Halo(){
	cout << "Destructor called." << endl;
	for (int i = 0; i < numberOfParticles; i++){
		delete particlesArray[i]; 
	}
	delete particlesArray;

	free(numberPerBin);
	free(densityBins);
	free(linEnergyBinEdges);
	free(linRadiusBinEdges);
	free(logBinEdges);
	free(cumulativeVR);
	free(averagePotential);

};


velocity_t Halo::averageVel(){
	velocity_t averageVelocity;
	double cutOff = CUT_OFF_VALUE;
	double tempSumX = 0;
	double tempSumY = 0;
	double tempSumZ = 0;
	int velCounter = 0;
	for(int n = 0; n < numberOfParticles; n++){
		if(particlesArray[n]->radius < maxRadius * cutOff){
			tempSumX = tempSumX + particlesArray[n]->velocity.x;
			tempSumY = tempSumY + particlesArray[n]->velocity.y;
			tempSumZ = tempSumZ + particlesArray[n]->velocity.z;
			velCounter++;
		}
	}
	
	averageVelocity.x=tempSumX/(velCounter);
	averageVelocity.y=tempSumY/(velCounter);
	averageVelocity.z=tempSumZ/(velCounter);
	
	return averageVelocity;
}

void Halo::centerPositions(){
	int i;
	for(i = 0; i< numberOfParticles;i++){
		particlesArray[i]->centerPosition(center);
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

void Halo::normalizeVelocity(){

	for(int i =0; i< numberOfParticles;i++){
		particlesArray[i]->normalizeVelocity(haloVelocity);
	}

}

void Halo::findMaxPot(){

	double tempMax = particlesArray[0]->potential;
	for(int i=0; i<numberOfParticles;i++){
		if (particlesArray[i]->potential>=tempMax){tempMax = particlesArray[i]->potential;}
	}
	maxPotential = tempMax;
}

//Potential offset removal should not be necessary anymore.
void Halo::removePotentialOffset(){

	for(int i =0; i< numberOfParticles;i++){
		particlesArray[i]->removePotentialOffset(maxPotential);
	}

}

void Halo::getEnergy(){

	for(int i = 0; i<numberOfParticles;i++){
		particlesArray[i]->energy = particlesArray[i]->kinetic + particlesArray[i]->potential;
	}

}


void Halo::findMaxMinEnergy(){
	double tempMin = particlesArray[0]->energy;
	double tempMax = particlesArray[0]->energy;
	for(int i=0; i<numberOfParticles;i++){
		if (particlesArray[i]->energy<=tempMin){tempMin = particlesArray[i]->energy;}
	}

	for(int i=0; i<numberOfParticles;i++){
		if (particlesArray[i]->energy>=tempMax){tempMax = particlesArray[i]->energy;}
	}

	minEnergy = tempMin;
	maxEnergy = tempMax;


}

void Halo::getCumulativeVR(){
	double * vr = (double*)malloc(sizeof(double)*numberOfBins);
	double currentPotential = 0;
	double currentKinetic = 0;

	for(int i = 0; i<numberOfBins; i++){
		for(int j = 0; j < numberOfParticles; j++){
			if(particlesArray[j]->radius>=linRadiusBinEdges[i] && particlesArray[j]->radius<=linRadiusBinEdges[i+1]){
				currentPotential = currentPotential + particlesArray[j]->potential;
				currentKinetic = currentKinetic + particlesArray[j]->kinetic;

			}
		}
		vr[i] = -currentKinetic/currentPotential;
	}
	cumulativeVR = vr;
	finalVR = vr[numberOfBins-1];
}

void Halo::getSphericallyAveragedPotential(){
	double * avePotByRadius = (double*)malloc(sizeof(double)*numberOfParticles);
	for(int i = 0; i < numberOfBins; i++){
		int counter = 0;
		double runningSum = 0;

		for(int j = 0; j < numberOfParticles; j++){
			if(particlesArray[j]->radius < linRadiusBinEdges[i+1] && particlesArray[j]->radius > linRadiusBinEdges[i]){
				runningSum = runningSum + particlesArray[j]->potential; counter++;
			}
		}

		if(counter!=0){avePotByRadius[i] = runningSum/counter;}
		else {avePotByRadius[i]=0;}
	}

	averagePotential = avePotByRadius;
}

void Halo::printOutput(outputfiles_t out){
	//printNewPosition(out.newPositonsFile);
	//printNewVelocity(out.newVelocitiesFile);
	//printNewPotential(out.newPotentialsFile);
	printRadius(out.radiusFile);
	printDensity(out.densityFile);
	printLinRadBins(out.linRadBins);
	printLinEnergyBins(out.linEnergyBins);
	printLogRadBins(out.logRadBins);
	//printEnergy(out.energy);
	printNotes(out.notes);
	//printSphericallyAveragedPotential(out.sphericallyAveragedPotential);
	printCumulativeVirialRatio(out.cumulativeVirialRatio);
	printEnergyBins(out.energyBinsFile);
	//printKinetic(out.kinetic);
	printNumberPerRadiusBin(out.numberPerBinRadiusFile);
}

void Halo::printNewPosition(FILE * posFile){

	for(int i=0;i<numberOfParticles;i++){
			fprintf(posFile,"%f%c%f%c%f%c",particlesArray[i]->centeredPosition.x,delimeter,particlesArray[i]->centeredPosition.y,delimeter,particlesArray[i]->centeredPosition.z,delimeter);
		fprintf(posFile,"\n");
	}


}

void Halo::printNewVelocity(FILE * velFile){

	for(int i=0;i<numberOfParticles;i++){
			fprintf(velFile,"%f%c%f%c%f%c",particlesArray[i]->normalizedVelocity.x,delimeter,particlesArray[i]->normalizedVelocity.y,delimeter,particlesArray[i]->normalizedVelocity.z,delimeter);
		fprintf(velFile,"\n");
	}

}

//this should not be used anymore
void Halo::printNewPotential(FILE * potFile){

	for(int i=0;i<numberOfParticles;i++){
		fprintf(potFile,"%f%c\n",particlesArray[i]->newPotential,delimeter);
		cout <<"THIS SHOULD NOT HAPPEN" <<endl;
	}

}

void Halo::printRadius(FILE * radFile){

	for(int i=0;i<numberOfParticles;i++){
		fprintf(radFile,"%f%c\n",particlesArray[i]->radius,delimeter);
	}

}


void Halo::printDensity(FILE * densityFile){

	for(int i=0;i<numberOfBins;i++){
		fprintf(densityFile,"%f%c\n",densityBins[i],delimeter);
	}

}

void Halo::printEnergyBins(FILE * energyBinsFile){

	for(int i=0;i<NUMBER_OF_ENERGY_BINS;i++){
		fprintf(energyBinsFile,"%d%c\n",energyBins[i],delimeter);
	}

}


void Halo::printLinRadBins(FILE * linRadBinsFile){

	for(int i=0;i<numberOfBins+1;i++){
		fprintf(linRadBinsFile,"%f%c\n",linRadiusBinEdges[i],delimeter);
	}

}


void Halo::printLinEnergyBins(FILE * linEnergyBinsFile){

	for(int i=0;i<NUMBER_OF_ENERGY_BINS+1;i++){
		fprintf(linEnergyBinsFile,"%f%c\n",linEnergyBinEdges[i],delimeter);
	}

}

void Halo::printLogRadBins(FILE * logRadBinsFile){
	
	for(int i=0;i<numberOfBins + 1;i++){
		fprintf(logRadBinsFile,"%f%c\n",logBinEdges[i],delimeter);
	}

}

void Halo::printEnergy(FILE * energyFile){

	for(int i=0;i<numberOfParticles;i++){
		fprintf(energyFile,"%f%c\n",particlesArray[i]->energy,delimeter);
	}

}

void Halo::printKinetic(FILE * kineticFile){

	for(int i=0;i<numberOfParticles;i++){
		fprintf(kineticFile,"%f%c\n",particlesArray[i]->kinetic,delimeter);
	}

}

void Halo::printNotes(FILE * notesFile){
	fprintf(notesFile,"number of particles = %d\n",numberOfParticles);
	fprintf(notesFile,"min radius = %f Mpc/h\n",minRadius);
	fprintf(notesFile,"max radius = %f Mpc/h\n",maxRadius);
	fprintf(notesFile,"Average X Velocity = %f km/s\nAverage Y Velocity = %f km/s\nAverage Z Velocity = %f km/s\n",haloVelocity.x,haloVelocity.y,haloVelocity.z);
	fprintf(notesFile,"maximum potential before subtraction = %f 10^10 Msun/h * (km/s)^2\n",maxPotential);
	fprintf(notesFile,"The final virial ratio is %f \n",finalVR);

}

void Halo::printSphericallyAveragedPotential(FILE * sphericallyAveragedPotentialFile){

	for(int i=0;i<numberOfBins;i++){
		fprintf(sphericallyAveragedPotentialFile,"%f%c\n",averagePotential[i],delimeter);
	}

}

void Halo::printCumulativeVirialRatio(FILE * vrFile){

	for(int i=0;i<numberOfBins;i++){
		fprintf(vrFile,"%f%c\n",cumulativeVR[i],delimeter);
	}

}


// won't use for now - no reason to not analyze substructures
double Halo::findSubstructure(){
	double indexOfSubstructure = 0;
	double dummySum;
	int dummyCounter;
	double numberOfBlocks = ceil(numberOfParticles/500.0);
	double * averageRadiusByBlock = (double*)malloc(sizeof(double)*numberOfBlocks);

	for(int i = 0; i < numberOfBlocks; i++){
		dummySum=0;
		dummyCounter = 0;
		for(int j = i*500; j < (i+1)*500; j++){
			if(j>=numberOfParticles){break;}
			dummySum=dummySum + particlesArray[j]->radius;
			dummyCounter++;
		}
		averageRadiusByBlock[i] = dummySum/dummyCounter;
	}

	double diff;
	for(int i = 1; i < numberOfBlocks; i++){
		diff = averageRadiusByBlock[i-1]-averageRadiusByBlock[i];
		if(diff > 0 && diff > 0.25*averageRadiusByBlock[i-1]){
			indexOfSubstructure = 500*i;
			break;
		}// make this into an array and print it out so i can plot it and compare it
	}

	return indexOfSubstructure;
}

void Halo::printNumberPerRadiusBin(FILE * numberPerBinRadiusFile){

	for(int i=0;i<numberOfBins;i++){
		fprintf(numberPerBinRadiusFile,"%d%c\n",numberPerBin[i],delimeter);
	}

}

#endif
