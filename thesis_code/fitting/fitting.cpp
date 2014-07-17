#include <iostream>
#include <stdio.h>
#include <cmath>
#include <stdlib.h>
#include "xorshift.h"
#include <string.h>
#include "fitting.h"
#include "filestuff.h"

using namespace std;

Fit::Fit(int nRows, int numberOfBins, datasource_t ds, double massmult){
	nrphi0 = nRows;
	nBins = numberOfBins;

	numberOfSteps=NUMBER_OF_STEPS;
			
	phi0LogDensity = (double*)malloc(sizeof(double)*nrphi0);
	memset(phi0LogDensity, 0, sizeof(double) * nrphi0);

	matchedPhi0Density = (double*)malloc(sizeof(double)*nBins);
	memset(matchedPhi0Density, 0, sizeof(double) * nBins);
	
	phi0LogRadius = (double*)malloc(sizeof(double)*nrphi0);
	memset(phi0LogRadius, 0, sizeof(double) * nrphi0);
		
	logDensityData = (double*)malloc(sizeof(double)*nBins);
	memset(logDensityData, 0, sizeof(double) * nBins);
	
	scaledLogDensityData = (double*)malloc(sizeof(double)*nBins);
	memset(scaledLogDensityData, 0, sizeof(double) * nBins);
	
	logRadiusData = (double*)malloc(sizeof(double)*nBins);
	memset(logRadiusData, 0, sizeof(double) * nBins);
		
	scaledLogRadiusData = (double*)malloc(sizeof(double)*nBins);
	memset(scaledLogRadiusData, 0, sizeof(double) * nBins);
	
	densityRMSArray = (double*)malloc(sizeof(double)*numberOfSteps);
	memset(densityRMSArray, 0, sizeof(double) * numberOfSteps);
	
	D_offset = (double*)malloc(sizeof(double)*numberOfSteps);
	memset(D_offset, 0, sizeof(double) * numberOfSteps);
	
	R_offset = (double*)malloc(sizeof(double)*numberOfSteps);
	memset(R_offset, 0, sizeof(double) * numberOfSteps);
		
	numberPerBinArray = (double*)malloc(sizeof(double)*nBins);
	memset(numberPerBinArray, 0, sizeof(double) * nBins);
		
	bestSLR = (double*)malloc(sizeof(double)*nBins);
	memset(bestSLR, 0, sizeof(double) * nBins);
	
	bestSLD = (double*)malloc(sizeof(double)*nBins);
	memset(bestSLD, 0, sizeof(double) * nBins);

	bestPHI0 = (double*)malloc(sizeof(double)*nBins);
	memset(bestPHI0, 0, sizeof(double) * nBins);

	double ** density = assembleArray(ds.densityFile,nBins,1);
	double ** numberPerBin = assembleArray(ds.numberperbinFile,nBins,1);
	double ** logRadBins = assembleArray(ds.logbinsFile, nBins+1, 1);
	for(int i = 0; i< nBins; i++){
		if(density[i][0]!=0){
			logDensityData[i]=log10(DENSITY_SCALE_FACTOR*massmult*density[i][0]);	// scale, "log", then assign data from 2D array to 1D array
			}
		else {logDensityData[i]=0;}
		// scale the center of each bin by some factor so the data overlaps, then assign the log of that value to the 1D array
		logRadiusData[i]=log10(RADIUS_SCALE_FACTOR*(logRadBins[i][0]+logRadBins[i+1][0])/2.0);

		numberPerBinArray[i] = numberPerBin[i][0];
	}

	closeArray(density,nBins);	//free memory
	closeArray(numberPerBin,nBins);
	closeArray(logRadBins,nBins);

	totalRStep=0;
	totalDStep=0;
	tempRStep=0;
	tempDStep=0;
	RMS=0;
	minRMS=0;
	bestR=0;
	bestD=0;
}


Fit::~Fit(){

	free(phi0LogDensity);
	free(matchedPhi0Density);
	free(phi0LogRadius);
	free(logDensityData);
	free(scaledLogDensityData);
	free(logRadiusData);
	free(scaledLogRadiusData);
	free(densityRMSArray);
	free(D_offset);
	free(R_offset);
	free(numberPerBinArray);
	free(bestSLR);
	free(bestSLD);
	free(bestPHI0);

}


int * Fit::findMatchingPoints(double * logradius, double * radphi0, int rows, int numberOfPhi0Rows){
	//double fake;
	int * index = (int*)malloc(sizeof(int)*rows);		//one array point for each data point (each bin)
	double currentClosest = radphi0[0];		// Radius we are looking at in big array.
	int currentIndex=0;						// Tracks position in big array
	
//---------------HOW TO FIND THE MATCHING POINTS------------------------------------//
	//find the simulation log radius's matching log radius in the phi0 values
	for(int i=0; i<rows;i++){ //	for each point for our log radii (from simulation)
		for(int j=currentIndex; j<numberOfPhi0Rows;j++){ // loop through the phi0 values until we find the next closest value
			//cout <<j <<" " <<logradius[i] <<" " <<radphi0[j] <<endl;
			//cin >>fake;
			if(abs(logradius[i]-radphi0[j])<=abs(logradius[i]-currentClosest)){currentClosest = radphi0[j]; currentIndex = j;} //if the difference between them is less than previously, set currentClosest to the new phi0lograd point.
			if(abs(logradius[i]-radphi0[j])>abs(logradius[i]-currentClosest)){break;} //break when we reach the closest value
		}
		index[i]=currentIndex; //set this value to the next entry in the array
	}	// repeat for each other radius value


	//for(int e = 0; e<rows; e++){cout <<index[e] <<endl;}
	//cin >>fake;
	return index; //return array
}

double Fit::calculateRMS(double * logDensity, double * numberPerBin, double * phi0LogDensity, int * indexArray, int nBins){
	double RMS=0;
	double sumContribution =0;
	double sumWeight = 0;
	double contribution=0;
	double weight = 0;
	int index = 0;
	for(int i = 0; i<nBins; i++){
		index = indexArray[i]-1;		//what is the index of the phi0LogDensity array that corrosponds to the next value in LogDensity
		contribution = pow(abs(logDensity[i]-phi0LogDensity[index]),2);
		weight = sqrt(numberPerBin[i]);
		//cout <<"number Per bin is " <<numberPerBin[i] <<endl;	///<<<<-------------THIS IS THE PROBLEM
		//cout <<"contribution is  " <<contribution <<" and weight is " <<weight <<endl;
		//system("PAUSE");
		if(numberPerBin[i] !=0){
			sumContribution=sumContribution+contribution*weight; 
			sumWeight = sumWeight + weight;
			//if(i%50==0){cout <<sumContribution <<" " <<sumWeight <<endl; system("PAUSE");}
		}
	}
	RMS = sqrt((sumContribution)/(sumWeight));

	return RMS;
}

int Fit::makeStep(double RMS, double previousRMS){		//decide whether or not to make the proposed step in offset space
	//cout <<"RMS = " <<RMS <<" and Previous RMS = " <<previousRMS <<endl;
	
	if( RMS < previousRMS ) {
		//cout <<" better rms " <<endl; 
		return 1; 
	}

	else if(xor128() < exp(-pow(RMS/previousRMS,2))) {
		//cout <<"made step anyway " <<endl; 
		return 1;
	}
	else { 
		//cout << " return 0 " <<endl; 
		return 0;
	}

}

fittingOutput_t Fit::makeFittingFileNames(string ID, string phi0){
	char * txt = ".txt";
	//char * phi0char = (char *)malloc(STRING_SIZE);
	//itoa(currentPhi0,phi0char,10);
	char * phi0char = (char*)malloc(sizeof(char)*4);
	strncpy ( phi0char, phi0.c_str(), 4 );
	strcat(phi0char,".phi0.");

	char * RMSfilename = (char *)malloc(STRING_SIZE);
	strcpy(RMSfilename, "/home/chris/Desktop/Thesis/output/fitting/density_fit/RMS/RMS.");
	strcat(RMSfilename, phi0char);
	strcat(RMSfilename, ID.c_str());
	strcat(RMSfilename, txt);
	
	char * D_offsetfilename = (char *)malloc(STRING_SIZE);
	strcpy(D_offsetfilename, "/home/chris/Desktop/Thesis/output/fitting/density_fit/D_offset/D_offset.");
	strcat(D_offsetfilename, phi0char);
	strcat(D_offsetfilename, ID.c_str());
	strcat(D_offsetfilename, txt);

	char * R_offsetfilename = (char *)malloc(STRING_SIZE);
	strcpy(R_offsetfilename, "/home/chris/Desktop/Thesis/output/fitting/density_fit/R_offset/R_offset.");
	strcat(R_offsetfilename, phi0char);
	strcat(R_offsetfilename, ID.c_str());
	strcat(R_offsetfilename, txt);

	char * scaledLogRadiusfilename = (char *)malloc(STRING_SIZE);
	strcpy(scaledLogRadiusfilename, "/home/chris/Desktop/Thesis/output/fitting/density_fit/slr/scaledLogRadius.");
	strcat(scaledLogRadiusfilename, phi0char);
	strcat(scaledLogRadiusfilename, ID.c_str());
	strcat(scaledLogRadiusfilename, txt);

	char * phi0LogDensityfilename = (char *)malloc(STRING_SIZE);
	strcpy(phi0LogDensityfilename, "/home/chris/Desktop/Thesis/output/fitting/density_fit/phi0/phi0.");
	strcat(phi0LogDensityfilename, phi0char);
	strcat(phi0LogDensityfilename, ID.c_str());
	strcat(phi0LogDensityfilename, txt);

	char * scaledLogDensityfilename = (char *)malloc(STRING_SIZE);
	strcpy(scaledLogDensityfilename, "/home/chris/Desktop/Thesis/output/fitting/density_fit/sld/scaledLogDensity.");
	strcat(scaledLogDensityfilename, phi0char);
	strcat(scaledLogDensityfilename, ID.c_str());
	strcat(scaledLogDensityfilename, txt);

	char * finalFitfilename = (char *)malloc(STRING_SIZE);
	strcpy(finalFitfilename, "/home/chris/Desktop/Thesis/output/fitting/density_fit/finalfit/finalFitValues.");
	strcat(finalFitfilename, phi0char);
	strcat(finalFitfilename, ID.c_str());
	strcat(finalFitfilename, txt);


	fittingOutput_t fitOut;

	FILE * RMSFile = fopen(RMSfilename,"w+");
	FILE * D_offsetFile = fopen(D_offsetfilename,"w+");
	FILE * R_offsetFile = fopen(R_offsetfilename,"w+");
	FILE * scaledLogRadiusFile = fopen(scaledLogRadiusfilename,"w+");
	FILE * phi0LogDensityFile = fopen(phi0LogDensityfilename,"w+");
	FILE * scaledLogDensityFile = fopen(scaledLogDensityfilename,"w+");
	FILE * finalFitFile = fopen(finalFitfilename, "w+");

	free(RMSfilename);
	free(D_offsetfilename);
	free(R_offsetfilename);
	free(scaledLogRadiusfilename);
	free(phi0LogDensityfilename);
	free(scaledLogDensityfilename);
	free(phi0char);
	free(finalFitfilename);

	fitOut.RMSFile = RMSFile;
	fitOut.D_offsetFile = D_offsetFile;
	fitOut.R_offsetFile = R_offsetFile;
	fitOut.scaledLogRadiusFile = scaledLogRadiusFile;
	fitOut.phi0LogDensityFile = phi0LogDensityFile;
	fitOut.scaledLogDensityFile = scaledLogDensityFile;
	fitOut.finalFitFile = finalFitFile;

	return fitOut;
}


void Fit::printFit(fittingOutput_t fitOut){

	printRMS(fitOut.RMSFile);
	printD(fitOut.D_offsetFile);
	printR(fitOut.R_offsetFile);
	printslr(fitOut.scaledLogRadiusFile);
	printsld(fitOut.scaledLogDensityFile);
	printphi0(fitOut.phi0LogDensityFile);
	printFinal(fitOut.finalFitFile);

}

void Fit::printRMS(FILE * RMSFile){
	for(int i=0;i<numberOfSteps;i++){
			fprintf(RMSFile,"%f%c\n",densityRMSArray[i],delimeter);
	}
}

void Fit::printD(FILE * D_offsetFile){
	for(int i=0;i<numberOfSteps;i++){
			fprintf(D_offsetFile,"%f%c\n",D_offset[i],delimeter);
	}
}

void Fit::printR(FILE * R_offsetFile){
	for(int i=0;i<numberOfSteps;i++){
			fprintf(R_offsetFile,"%f%c\n",R_offset[i],delimeter);
	}
}



void Fit::printslr(FILE * slrFile){
	for(int i=0;i<nBins;i++){
			fprintf(slrFile,"%f%c\n",bestSLR[i],delimeter);
	}
}

void Fit::printsld(FILE * sldFile){
	for(int i=0;i<nBins;i++){
			fprintf(sldFile,"%f%c\n",bestSLD[i],delimeter);
	}
}

void Fit::printphi0(FILE * phi0File){
	for(int i=0;i<nBins;i++){
			fprintf(phi0File,"%f%c\n",bestPHI0[i],delimeter);
	}
}

void Fit::printFinal(FILE * finalFile){
	fprintf(finalFile,"%f is the minimum RMS value%c\n",minRMS,delimeter);
	fprintf(finalFile,"%f is the best R offset%c\n",bestR,delimeter);
	fprintf(finalFile,"%f is the best D offset%c\n",bestD,delimeter);
}

void Fit::closeFitFiles(fittingOutput_t fitOut){
	fclose(fitOut.D_offsetFile);
	fclose(fitOut.phi0LogDensityFile);
	fclose(fitOut.RMSFile);
	fclose(fitOut.R_offsetFile);
	fclose(fitOut.scaledLogDensityFile);
	fclose(fitOut.scaledLogRadiusFile);
	fclose(fitOut.finalFitFile);

}


EnergyFit::EnergyFit(int numberOfBins, datasource_t ds){

	nBins = numberOfBins;
	numberOfSteps=NUMBER_OF_STEPS;

	double ** NArray = assembleArray(ds.NFile,nBins,1);
	double ** energyBinEdgesArray = assembleArray(ds.linenergybinsFile,nBins+1,1);

	N = (double*)malloc(sizeof(double)*nBins);	//number per linear energy bin
	logN = (double*)malloc(sizeof(double)*nBins);	// log base 10 of the number of particles in the energy bin
	energy = (double*)malloc(sizeof(double)*nBins);	// energy is the center of the bin

	for(int i = 0; i < nBins; i++){
		N[i] = NArray[i][0];
		if(N!=0){logN[i] = (double)log10((double)N[i]);} else{logN[i] = 0;}
		energy[i] = (energyBinEdgesArray[i][0]+energyBinEdgesArray[i+1][0])/2.0;
	}
	
	for(int i = 0; i < nBins; i++){
		free(NArray[i]);
		free(energyBinEdgesArray[i]);
	}
	delete NArray;
	delete energyBinEdgesArray;
	
	minEnergy = energy[0];

	RMSArray = (double*)malloc(sizeof(double)*numberOfSteps);
	memset(RMSArray, 0, sizeof(double) * numberOfSteps);

	Phi0Path = (double*)malloc(sizeof(double)*numberOfSteps);
	memset(Phi0Path, 0, sizeof(double) * numberOfSteps);
	
	ScaleFactorPath = (double*)malloc(sizeof(double)*numberOfSteps);
	memset(ScaleFactorPath, 0, sizeof(double) * numberOfSteps);

	BetaPath = (double*)malloc(sizeof(double)*numberOfSteps);
	memset(BetaPath, 0, sizeof(double) * numberOfSteps);

	phi0Value = 0;
	betaValue = -1.0/100000.0;
	scaleFactor = 1;
	tempScaleStep = 0;
	tempEnergyStep = 0;
	tempBetaValueStep = 0;
	RMS = 0;
	minRMS = 0;
	bestBeta = 0;
	bestPhi = 0;
	bestScale = 0;
	lastBinToInclude = 0;
	firstEnergyBin = energy[0];
	deltaE = energy[1]-energy[0];
}


EnergyFit::~EnergyFit(){

	free(energy);
	free(logN);
	free(Phi0Path);
	free(ScaleFactorPath);
	free(BetaPath);
	free(RMSArray);
	free(N);

}


// fitting for N
double EnergyFit::calculateRMSN(double * energy, double phi0Value, double logNStep, double * numberPerBin, double betaValue){
	double RMS=0;
	double binsIncluded = 0;
	double sumContribution =0;
	double contribution=0;
	double err = 0;
	double theoryValue;

	int n = 1;
	while(phi0Value<firstEnergyBin - n*deltaE){n++;};	//count how far the theoretical fit goes past the data points
	n=n-1;	//n is now the number of extra bins we need to make (that will be compared to data points of 0)
	extraBins = n;

	double * newBinCenters = (double*)malloc(sizeof(double)*(n+lastBinToInclude));
	double * newNumberPerBin = (double*)malloc(sizeof(double)*(n+lastBinToInclude));

	for(int i = n; i<n+lastBinToInclude; i++){
		newBinCenters[i] = energy[i-n];					//define bin centers where there is data
		newNumberPerBin[i] = numberPerBin[i-n];}		//put data in new array
	for(int i = 1; i<n+1; i++){
		newBinCenters[n-i]=energy[0]-i*deltaE;			//define bin centers where the data is 0;
		newNumberPerBin[n-i] = 0;
	}	

	for(int i = 0; i<n+lastBinToInclude; i++){
		theoryValue = (exp(betaValue*(phi0Value-newBinCenters[i]))-1.0)*pow(10,logNStep);
		contribution = pow(abs(newNumberPerBin[i]-theoryValue),2);
		err = sqrt(theoryValue);
		sumContribution = sumContribution + contribution/pow(err,2);  // [(N_data - N_th)/err]^2
		binsIncluded++;

	}
	
	RMS = sqrt(sumContribution/(binsIncluded));

	free(newBinCenters);
	free(newNumberPerBin);

	return RMS;

}


double EnergyFit::calculateRMSLogN(){
	double RMSvalue=0;
	double binsIncluded = 0;
	double sumContribution =0;
	double sumWeight = 0;
	double contribution=0;
	double theoryValue;
	double var;
	double p = phi0Value + tempEnergyStep;
	double b = betaValue + tempBetaValueStep;
	double a = scaleFactor + tempScaleStep;
	//cout <<"log10(" <<a <<"*(exp(" <<b <<"*(" <<p <<"-" <<energy[0] <<")) - 1.0))" <<endl;
	for(int i = 0; i<lastBinToInclude; i++){
		theoryValue = log10(a*(exp(b*(p - energy[i]))-1.0));
		contribution = pow(abs(logN[i]-theoryValue),2);
		//cout <<theoryValue <<" " <<logN[i] <<" " <<contribution <<" " <<N[i] <<" " <<energy[i] <<endl;
		//if(i%10==0){cin >>var;}
		//if(isnan(theoryValue)){cout <<"NaN: " <<"log10(" <<a <<"*(exp(" <<b <<"*(" <<p <<"-" <<energy[i] <<")) - 1.0))" <<endl;}
		if(N[i] != 0 && energy[i] < 0 && theoryValue > 0){
			sumContribution=sumContribution+contribution;
			binsIncluded++;
		}
	}
	RMSvalue = sqrt((sumContribution)/(binsIncluded));
	//cin >>var; 
	return RMSvalue;
}



int EnergyFit::makeStep(double RMS, double previousRMS){		//decide whether or not to make the proposed step in offset space
	//cout <<"RMS = " <<RMS <<" and Previous RMS = " <<previousRMS <<endl;
	if( RMS < previousRMS ) {
		//cout <<" better rms " <<endl; 
		return 1; 
	}
	else if(xor128() < exp(-pow(RMS/previousRMS,2))) {
		//cout <<"made step anyway " <<endl; 
		return 1;
	}
	else { 
		//cout << " return 0 " <<endl; 
		return 0;
	}
}

energyFittingOutput_t EnergyFit::makeFittingFileNames(string ID){

	//FILE * RMSArrayFile;
	//FILE * phi0PathFile;
	//FILE * scaleFactorPathFile;

	char * txt = ".txt";

	char * RMSfilename = (char *)malloc(STRING_SIZE);
	strcpy(RMSfilename, "/home/chris/Desktop/Thesis/output/fitting/energy_fit/RMS/RMS.");
	strcat(RMSfilename, ID.c_str());
	strcat(RMSfilename, txt);
	char * phi0pathfilename = (char *)malloc(STRING_SIZE);
	strcpy(phi0pathfilename, "/home/chris/Desktop/Thesis/output/fitting/energy_fit/phi0/phi0.");
	strcat(phi0pathfilename, ID.c_str());
	strcat(phi0pathfilename, txt);
	char * scalepathfilename = (char *)malloc(STRING_SIZE);
	strcpy(scalepathfilename, "/home/chris/Desktop/Thesis/output/fitting/energy_fit/scale/scale.");
	strcat(scalepathfilename, ID.c_str());
	strcat(scalepathfilename, txt);
	char * betapathfilename = (char *)malloc(STRING_SIZE);
	strcpy(betapathfilename, "/home/chris/Desktop/Thesis/output/fitting/energy_fit/beta/beta.");
	strcat(betapathfilename, ID.c_str());
	strcat(betapathfilename, txt);
	char * finalfilename = (char *)malloc(STRING_SIZE);
	strcpy(finalfilename, "/home/chris/Desktop/Thesis/output/fitting/energy_fit/final_values/finalvalues.");
	strcat(finalfilename, ID.c_str());
	strcat(finalfilename, txt);


	energyFittingOutput_t energyFitOut;

	FILE * RMSFile = fopen(RMSfilename,"w+");
	FILE * phi0File = fopen(phi0pathfilename,"w+");
	FILE * scaleFile = fopen(scalepathfilename,"w+");	
	FILE * betaFile = fopen(betapathfilename,"w+");	
	FILE * finalFile = fopen(finalfilename,"w+");

	free(RMSfilename);
	free(phi0pathfilename);
	free(scalepathfilename);
	free(betapathfilename);
	free(finalfilename);
		
	energyFitOut.RMSArrayFile = RMSFile;
	energyFitOut.phi0PathFile = phi0File;
	energyFitOut.scaleFactorPathFile = scaleFile;	
	energyFitOut.betaPathFile = betaFile;
	energyFitOut.finalFile = finalFile;

	return energyFitOut;

}

void EnergyFit::printOutput(energyFittingOutput_t energyOut){
	
	printRMS(energyOut.RMSArrayFile);
	printPhi0(energyOut.phi0PathFile);
	printScale(energyOut.scaleFactorPathFile);
	printBeta(energyOut.betaPathFile);
	printFinal(energyOut.finalFile);

}

void EnergyFit::printRMS(FILE * RMSFile){
	for(int i=0;i<numberOfSteps;i++){
			fprintf(RMSFile,"%f%c\n",RMSArray[i],delimeter);
	}
}

void EnergyFit::printPhi0(FILE * phi0File){
	for(int i=0;i<numberOfSteps;i++){
			fprintf(phi0File,"%f%c\n",Phi0Path[i],delimeter);
	}
}

void EnergyFit::printScale(FILE * scaleFile){
	for(int i=0;i<numberOfSteps;i++){
			fprintf(scaleFile,"%f%c\n",ScaleFactorPath[i],delimeter);
	}
}

void EnergyFit::printBeta(FILE * betaFile){
	for(int i=0;i<numberOfSteps;i++){
			fprintf(betaFile,"%.10f%c\n",BetaPath[i],delimeter);
	}
}

void EnergyFit::printFinal(FILE * finalFile){
	int x = lastBinToInclude;
	fprintf(finalFile,"%f min RMS%c\n",minRMS,delimeter);
	fprintf(finalFile,"%f best Phi%c\n",bestPhi,delimeter);
	fprintf(finalFile,"%f best Beta%c\n",bestBeta,delimeter);
	fprintf(finalFile,"%f best Scale%c\n",bestScale,delimeter);
	fprintf(finalFile,"%d last Bin Included with energy %f%c\n",x,energy[x],delimeter);
	fprintf(finalFile,"final fit function is N(E) = 10^(%f)*(exp(%f*(%f - E)) - 1) %c\n",bestScale,bestBeta,bestPhi,delimeter);
	fprintf(finalFile,"number of extra bins is %d%c\n",extraBins,delimeter);
}

void EnergyFit::closeEnergyFitFiles(energyFittingOutput_t energyOut){
	fclose(energyOut.RMSArrayFile);
	fclose(energyOut.phi0PathFile);
	fclose(energyOut.scaleFactorPathFile);
	fclose(energyOut.betaPathFile);
	fclose(energyOut.finalFile);
}

void EnergyFit::findLastBinToInclude(double * numberPerBin){
	double tempMax = 0;
	
	for(int i=0; i<nBins; i++){
		if(numberPerBin[i]>tempMax){
			tempMax = numberPerBin[i];
			lastBinToInclude = i;
		}
	}
}
