#include "arrayconstructor.h"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <cmath>
#include "extras.h"
#include "Particle.h"

/* Threading Headers */
#include <pthread.h>
#include <sys/types.h>

/* Threading Constants */
#define MAX_THREADS 4

#define MASS_MULTIPLIER 10 //multiply the particle mass by this factor (1 if low mass halo, 10 if high mass halo (corrects for 1/10 sampling rate)

#define ID_DIRECTORY_NAME "/home/user1/nolting/Desktop/datastuffs/id_directory2.txt" //location of halo IDs
typedef Particle* ParticleArray;
#define PARTICLE_MASS 6.885*pow(10.0,6.0)
#define GRAVITATIONAL_CONSTANT 4.301*pow(10.0,-9.0)
#define delimeter ' '
#define EPSILON 0.0028
//#define EPSILON 0.000001
#define PI 3.1415926536

extern "C" void * haloThread(void * HaloID);
static inline double sq(double x) {
	return x*x;
}

static inline double third(double x) {
	return x*x*x;
}

static inline double fourth(double x) {
	return x*x*x*x;
}

static inline double fifth(double x) {
	return x*x*x*x*x;
}

/* Global Threading Variables */
pthread_cond_t *	threadAvailable;
pthread_mutex_t *	numberOfThreadsMutex;
int			numberOfThreads = 0;


int main(){
	FILE * idDirectoryFile = fopen(ID_DIRECTORY_NAME,"r"); //find Halo IDs
	int numberOfHalos = getNumberOfRows(idDirectoryFile); //count number of halos
	char * HaloID;

	numberOfThreadsMutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	threadAvailable = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));

	pthread_mutex_init(numberOfThreadsMutex, NULL);		// Initialize mutex that is not yet locked.
	pthread_cond_init(threadAvailable, NULL);

	for(int n=0;n<numberOfHalos;n++){	//for each halo do this: (change to numberOfHalos later)
																	
		pthread_mutex_lock(numberOfThreadsMutex);		// Lock our mutex. CRITICAL SECTION START
		while (!(numberOfThreads < MAX_THREADS)) {
			pthread_cond_wait(threadAvailable, numberOfThreadsMutex);	// Sleep and wait for open thread.
		}
		
		numberOfThreads++;				// Increment our number of active threads.
		pthread_mutex_unlock(numberOfThreadsMutex);						// Release mutex lock.  CRITICAL SECTION END.
		
		HaloID = readNextID(idDirectoryFile);		//read halo ID
		
		pthread_t thread;
		pthread_create(&thread, NULL, &haloThread, HaloID);				// Break off our new thread to do the work.
		
	}		//end for loop, repeat for each halo.

	pthread_mutex_lock(numberOfThreadsMutex);
	while (numberOfThreads > 0) {}							// SPPIN
	pthread_mutex_unlock(numberOfThreadsMutex);

	//system("PAUSE");
	return 0;
}//end of program





extern "C" void * haloThread(void * id){
	char * HaloID = (char *)id;
	cout << "Analyzing halo with ID " << HaloID << endl;
	int numberOfParticles, nCols;
	double ** positionsArray;
	Particle ** particlesArray;
	FILE * output;
	double radius = 0;
	double u;

	FILE * positionFile = getPositionFile(HaloID);		//make the file path
	if (positionFile == NULL)
		printf("POSITION FILE NULL\n");
	numberOfParticles = getNumberOfRows(positionFile);	//count number of particles in file
	nCols = getNumberOfColumns(positionFile);			//get number of columns in file (3)
	particlesArray = new ParticleArray[numberOfParticles];	//make an array of particles
	for (int i = 0; i < numberOfParticles; i++){
		particlesArray[i] = new Particle();					//each location in the array is a particle (with positions)
	}
	positionsArray = assembleArray(positionFile,numberOfParticles,nCols);
	for (int i = 0; i<numberOfParticles;i++){
		particlesArray[i]->position.x = positionsArray[i][0];
		particlesArray[i]->position.y = positionsArray[i][1];
		particlesArray[i]->position.z = positionsArray[i][2];
		free(positionsArray[i]);
	}
	delete positionsArray;
       
	for(int i = 0; i < numberOfParticles; i++){

		for(int j = 0; j < numberOfParticles; j++){
				radius = sqrt(sq((particlesArray[i]->position.x)-(particlesArray[j]->position.x))+sq((particlesArray[i]->position.y)-(particlesArray[j]->position.y))+sq((particlesArray[i]->position.z)-(particlesArray[j]->position.z)));
				u = radius/EPSILON;
				if(u<0.5){
					particlesArray[i]->directPotential += GRAVITATIONAL_CONSTANT*PARTICLE_MASS*MASS_MULTIPLIER/EPSILON*(16.0/3.0*sq(u)-48.0/5.0*fourth(u)+32.0/5.0*fifth(u)-14.0/5.0);
				}
				else if(u<1){
					particlesArray[i]->directPotential += GRAVITATIONAL_CONSTANT*PARTICLE_MASS*MASS_MULTIPLIER/EPSILON*(1.0/(15.0*u)+32.0/3.0*sq(u)-16.0*third(u)+48.0/5.0*fourth(u)-32.0/15.0*fifth(u)-16.0/5.0);
				}
				else{
					particlesArray[i]->directPotential += GRAVITATIONAL_CONSTANT*PARTICLE_MASS*MASS_MULTIPLIER/EPSILON*(-1.0/u);
				}

		}
		if(i%100000==0){cout <<HaloID <<" - " <<i <<"th particle's potential calculated" <<endl;}
	}

	output = makeOutputFileName(HaloID);		//make output file

	//print output
	for(int i = 0; i<numberOfParticles; i++){
		fprintf(output,"%f%c\n",particlesArray[i]->directPotential,delimeter);
	}

	//free memory
	for (int i = 0; i < numberOfParticles; i++){
		delete particlesArray[i]; 
	}
	delete particlesArray;

	//close files
	fclose(positionFile);
	fclose(output);

		// Reduce our active threads count.
	pthread_mutex_lock(numberOfThreadsMutex);
	numberOfThreads--;
	pthread_mutex_unlock(numberOfThreadsMutex);
	pthread_cond_signal(threadAvailable);			// Signal to wake up main.

	cout <<"Halo " << HaloID <<" done." <<endl;

}
