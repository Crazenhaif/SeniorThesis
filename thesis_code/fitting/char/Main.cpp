#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <cmath>
#include "arrayconstructor.h"
#include "filestuff.h"
#include "constants.h"
#include "fitting.h"
#include "xorshift.h"

#define RUN 3	//run = 1 only density / run = 2 only energy / run = 3 both

using namespace std;

void pausenow();

//TO change from large to small data sets, change this directory
#define ID_DIRECTORY_NAME "/home/chris/Desktop/Thesis/source_files/id_directory_large.txt"
//#define ID_DIRECTORY_NAME "/home/chris/Desktop/Thesis/source_files/id_directory_small.txt"
#define PHI_DIRECTORY_NAME "/home/chris/Desktop/Thesis/source_files/DARKexp/phidirectory.txt"

int main(){
	FILE * idDirectoryFile = fopen(ID_DIRECTORY_NAME,"r");
	if(idDirectoryFile == NULL){cout <<"null directory" <<endl;}
	int numberOfHalos = getNumberOfRows(idDirectoryFile);					// count number of Halos
	FILE * whichPhi0 = fopen("/home/chris/Desktop/Thesis/output/fitting/density_fit/whichphi0.txt","w+");
/// ------- START FOR LOOP -------------///

	//all halos use numberOfHalos 
	//for one halo use n<1
	for(int n=0;n<numberOfHalos;n++){										// for each Halo do this:
		//cout <<"start itteration " <<n <<endl;	
		char * HaloID = readNextID(idDirectoryFile);							// read in Halo ID number
		cout <<"Halo ID is " <<HaloID <<endl;
		datasource_t ds = makeSourceFileNames(HaloID);							// create all source file names from ID number
//-----------------------------------------------------------------------------------//
//--------------------DENSITY FITTING------------------------------------------------//		
//-----------------------------------------------------------------------------------//
		if(RUN == 1 || RUN == 3){
			FILE * phi0DirectoryFile = fopen(PHI_DIRECTORY_NAME,"r");
			int numberOfPhi = getNumberOfRows(phi0DirectoryFile);
			char ** phi0Array = (char **) malloc(numberOfPhi * sizeof(char*));
			double * bestRMSArray = (double *) malloc(numberOfPhi*sizeof(double));
			memset(bestRMSArray, 0, sizeof(double) * numberOfPhi);
			int phi0tracker = 0;
			//cout <<"Begin Density Fitting" <<endl;
			while(phi0tracker < numberOfPhi){	//numberOfPhi when fitting runs 0 when fitting doesn't run
// ---------------open liliya's theory data files----------------//
				char * nextPhi = readNextID(phi0DirectoryFile);
				string currentPhi(nextPhi);
				phi0Array[phi0tracker] = (char*)currentPhi.c_str();
				//pausenow();
				FILE * phi0File = makePhi0FileName(phi0Array[phi0tracker]);
				int nrphi0 = getNumberOfRows(phi0File);			// count total number of rows of theory data
				Fit * thisFit = new Fit(nrphi0,NUMBER_OF_BINS, ds);
				
				int ncphi0 = getNumberOfColumns(phi0File);		// count total number of columns of theory data
				double ** phi0data = assembleArray(phi0File,nrphi0,ncphi0);	// generate 2D array of all data in file
				for(int i=0;i<nrphi0;i++){
					thisFit->phi0LogRadius[i] = phi0data[i][2];			// assign the radius data from the 2D array to the 1 D array
					thisFit->phi0LogDensity[i] = phi0data[i][3];			// assign the density data from the 2D array to the 1 D array
				}
				closeArray(phi0data,nrphi0);
				//pausenow();

//---------------------LOOP FOR 1000 STEPS -----------------------------//

				int stepCount = 0;
				while(stepCount < 1000){
					for(int i = 0; i<thisFit->nBins; i++){
						thisFit->scaledLogRadiusData[i] = thisFit->logRadiusData[i] + thisFit->totalRStep + thisFit->tempRStep;
						thisFit->scaledLogDensityData[i] = thisFit->logDensityData[i] + thisFit->totalDStep + thisFit->tempDStep;
					}
		
					thisFit->indexArray = thisFit->findMatchingPoints(thisFit->scaledLogRadiusData,thisFit->phi0LogRadius,thisFit->nBins,nrphi0); // call function and make index array

					for(int i =0; i<thisFit->nBins; i++){		//create the array of phi0 density values that correspond to our density values
						thisFit->matchedPhi0Density[i] = thisFit->phi0LogDensity[thisFit->indexArray[i]];
					}

					thisFit->RMS = thisFit->calculateRMS(thisFit->scaledLogDensityData,thisFit->numberPerBinArray,thisFit->phi0LogDensity,thisFit->indexArray,thisFit->nBins);
					free(thisFit->indexArray);
					if(stepCount == 0 || thisFit->makeStep(thisFit->RMS,thisFit->densityRMSArray[stepCount-1])){ 
						if(stepCount ==0 || thisFit->RMS < thisFit->minRMS){
							thisFit->minRMS = thisFit->RMS;
							thisFit->bestD = thisFit->totalDStep + thisFit->tempDStep;
							thisFit->bestR = thisFit->totalRStep + thisFit->tempRStep;
							for(int i = 0; i<thisFit->nBins; i++){
								thisFit->bestSLR[i] = thisFit->scaledLogRadiusData[i];
								thisFit->bestSLD[i] = thisFit->scaledLogDensityData[i];
								thisFit->bestPHI0[i] = thisFit->matchedPhi0Density[i];
							}
						}
						thisFit->densityRMSArray[stepCount] = thisFit->RMS; 
						thisFit->D_offset[stepCount] = thisFit->totalDStep + thisFit->tempDStep;
						thisFit->R_offset[stepCount] = thisFit->totalRStep + thisFit->tempRStep;
						thisFit->totalRStep = thisFit->totalRStep + thisFit->tempRStep;
						thisFit->totalDStep = thisFit->totalDStep + thisFit->tempDStep;
						stepCount++;
					}

					thisFit->tempRStep = MAX_R_STEP * sqrt(-2*log(xor128()))*cos(2*pi*xor128());
					thisFit->tempDStep = MAX_D_STEP * sqrt(-2*log(xor128()))*cos(2*pi*xor128());
				
				};						//END FITTING LOOP 

// ---------------------PRINT FITTING OUTPUT ----------------------------------//
				bestRMSArray[phi0tracker] = thisFit->minRMS;
				fittingOutput_t fitOut = thisFit->makeFittingFileNames(HaloID, phi0Array[phi0tracker]);

				thisFit->printFit(fitOut);
				fclose(phi0File);
				thisFit->closeFitFiles(fitOut);


				delete thisFit;
				//cout <<"phi0 = " <<phi0Array[phi0tracker] <<" fitting has completed." <<endl;

				phi0tracker++;	//increment phi0tracker so that we go on to the next phi0 value
				free(nextPhi);
				cout <<"phi 0 array thus far is :" <<endl;
				for(int l = 0; l<phi0tracker+1; l++){cout <<phi0Array[0] <<endl <<"phi0tracker is " <<phi0tracker <<endl;}
				cout <<"loop done" <<endl;
			}//end current phi0 loop, return to top to go to next phi0 value and continue fitting
			int bestPhi = 0;
			double bestRms;
			for(int i = 0; i<numberOfPhi; i++){
				cout <<"RMS for phi of " <<phi0Array[i] <<" is " <<bestRMSArray[i] <<endl;
				if(i==0 || bestRms > bestRMSArray[i]){
				bestPhi = i;
				bestRms = bestRMSArray[i];
				}
			}
			cout <<"best phi0 from density fit is " <<phi0Array[bestPhi] <<endl;
			fprintf(whichPhi0,"%s %s is the best phi0 / halo ID with an RMS of %f\n",phi0Array[bestPhi],HaloID,bestRms);
			free(phi0Array);
			free(bestRMSArray);
			fclose(phi0DirectoryFile);
			pausenow();
	}

//------------------------------------------------------------------------------//
//----------------------------Energy Fitting------------------------------------//
//------------------------------------------------------------------------------//

		if(RUN == 2 || RUN ==3){
			EnergyFit * energyFitting = new EnergyFit(NUMBER_OF_ENERGY_BINS,ds);
			int energySteps = 0;
			energyFitting->phi0Value = energyFitting->minEnergy;
			energyFitting->findLastBinToInclude(energyFitting->N);
			int numberOfAttemptedSteps = 0;
			while (energySteps < energyFitting->numberOfSteps){
				if(energyFitting->betaValue + energyFitting->tempBetaValueStep < 0){
					//energyFitting->RMS=energyFitting->calculateRMSN(energyFitting->energy,energyFitting->phi0Value + energyFitting->tempEnergyStep,energyFitting->totalLogNStep + energyFitting->tempLogNStep,energyFitting->N, energyFitting->betaValue + energyFitting->tempBetaValueStep);
					energyFitting->RMS=energyFitting->calculateRMSLogN();
					//cout <<"rms is " <<energyFitting->RMS <<endl;
					if(energySteps == 0 || energyFitting->makeStep(energyFitting->RMS,energyFitting->RMSArray[energySteps-1])){ 
				
							energyFitting->phi0Value = energyFitting->phi0Value + energyFitting->tempEnergyStep;
							energyFitting->betaValue = energyFitting->betaValue + energyFitting->tempBetaValueStep;
							energyFitting->scaleFactor = energyFitting->scaleFactor + energyFitting->tempScaleStep;
							energyFitting->RMSArray[energySteps] = energyFitting->RMS;
							energyFitting->BetaPath[energySteps] = energyFitting->betaValue;
							energyFitting->Phi0Path[energySteps] = energyFitting->phi0Value;
							energyFitting->ScaleFactorPath[energySteps] = energyFitting->scaleFactor;
					
							if(energySteps ==0 || energyFitting->RMS < energyFitting->minRMS){
								energyFitting->minRMS = energyFitting->RMS;
								energyFitting->bestBeta = energyFitting->betaValue;
								energyFitting->bestPhi = energyFitting->phi0Value;
								energyFitting->bestScale = energyFitting->scaleFactor;
							}

							energySteps++;
							//cout <<"step number " <<energySteps <<" taken" <<endl;
							//numberOfAttemptedSteps = 0;
						}
					else{
						//cout <<"step not taken" <<endl;
					}
				}
					energyFitting->tempEnergyStep = MAX_E_STEP * sqrt(-2*log(xor128()))*cos(2*pi*xor128());
					energyFitting->tempScaleStep = MAX_SCALE_STEP * sqrt(-2*log(xor128()))*cos(2*pi*xor128());
					energyFitting->tempBetaValueStep = MAX_BETA_STEP * sqrt(-2*log(xor128()))*cos(2*pi*xor128());
					numberOfAttemptedSteps++;
			};
			cout <<"Halo ID " <<HaloID <<" has best phi_0 = " <<energyFitting->bestPhi*energyFitting->bestBeta <<" from energy fitting" <<endl;
			energyFittingOutput_t energyOut = energyFitting->makeFittingFileNames(HaloID); //make the file path to write out to.
			energyFitting->printOutput(energyOut);
			energyFitting->closeEnergyFitFiles(energyOut);


	//End Energy Fitting

			delete energyFitting;


}
free(HaloID);
}	//end for loop - repeat for each halo
fclose(whichPhi0);
return 0;
}


void pausenow(){
double stop;
cout <<"paused, enter any number to continue." <<endl;
cin >>stop;
}

