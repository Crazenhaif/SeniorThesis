#ifndef FILE_STUFF_CPP
#define FILE_STUFF_CPP
#include "filestuff.h"
#include <stdlib.h>
#include <iostream>
using namespace std;
#include <stdio.h> 

char * readNextID(FILE * file){
	char * line = (char *)malloc(sizeof(char) * fileNameSize);					//allocate memory for file name array

	do {
		fgets(line, fileNameSize, file);
		//if(feof(file)){cout <<"end of file reached" <<endl;} //decided this is not needed
	} while (line[0] == '\n');

	for(int n=0;n<fileNameSize;n++){
		if(line[n]=='\n' || line[n]=='\r'){line[n]='\0';}					//remove newline character from file name
	}

	return line;
}


datasource_t makeSourceFileNames(char * ID){
	//  File locations will be of the form:
	//  C:/Users/Chris/Desktop/thesis_files/position_files/pos.0.1594.txt

	char * densityfilename = (char *)malloc(STRING_SIZE);
	strcpy(densityfilename, "/home/chris/Desktop/Thesis/output/dataAnalysis/density/density/density.");
	char * numberperbinfilename = (char *)malloc(STRING_SIZE);
	strcpy(numberperbinfilename, "/home/chris/Desktop/Thesis/output/dataAnalysis/density/numberPerBin/numberPerBin.");
	char * logbinsfilename = (char *)malloc(STRING_SIZE);
	strcpy(logbinsfilename, "/home/chris/Desktop/Thesis/output/dataAnalysis/bins/logbins/logbins.");
	char * Nfilename = (char *)malloc(STRING_SIZE);
	strcpy(Nfilename, "/home/chris/Desktop/Thesis/output/dataAnalysis/N/N.");
	char * linenergybinsfilename = (char *)malloc(STRING_SIZE);
	strcpy(linenergybinsfilename, "/home/chris/Desktop/Thesis/output/dataAnalysis/bins/linenergybins/linenergybins.");
	char * txt = ".txt";

	strcat(densityfilename, ID);
	strcat(densityfilename, txt);

	strcat(numberperbinfilename, ID);
	strcat(numberperbinfilename, txt);

	strcat(logbinsfilename, ID);
	strcat(logbinsfilename, txt);
	
	strcat(Nfilename, ID);
	strcat(Nfilename, txt);

	strcat(linenergybinsfilename, ID);
	strcat(linenergybinsfilename, txt);

	datasource_t ds;

	FILE * density = fopen(densityfilename,"r");
	FILE * numberPerBin = fopen(numberperbinfilename,"r");
	FILE * logRadBins = fopen(logbinsfilename,"r");
	FILE * N = fopen(Nfilename,"r");
	FILE * linEnergyBins = fopen(linenergybinsfilename,"r");

	free(densityfilename);
	free(numberperbinfilename);
	free(logbinsfilename);
	free(Nfilename);
	free(linenergybinsfilename);
	
	ds.densityFile = density;
	ds.numberperbinFile = numberPerBin;
	ds.logbinsFile = logRadBins;
	ds.NFile = N;
	ds.linenergybinsFile =linEnergyBins;

	return ds;
}


int countCharactersTil(char * thing, char end){
	int counter = 0;
	int location = 0;

	while(thing[location]!= end){
		counter++;
		location++;
	};

	return counter;
}

FILE * makePhi0FileName(char * phi0char){
	//FILE * phi0File = fopen("/home/user1/nolting/Desktop/Fitting/DARKexp/DARKexp.phi0.1.0.txt.","r");	
	
	char * phi0FileName = (char *)malloc(STRING_SIZE);
	strcpy(phi0FileName, "/home/chris/Desktop/Thesis/source_files/DARKexp/DARKexp.phi0.");
	char * txt = ".txt";

//	itoa(phi0value,phi0valuechar,10);

	strcat(phi0FileName, phi0char);
	strcat(phi0FileName, txt);

	FILE * phi0File = fopen(phi0FileName,"r");
	if(phi0File == NULL){cout <<"Phi_0 file not found" <<endl;}
	free(phi0FileName);
	return phi0File;
}


#endif
