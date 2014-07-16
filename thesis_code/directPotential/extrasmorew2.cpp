#include "extrasmorew2.h"
#include <stdlib.h>
#include <iostream>
using namespace std;
#include <stdio.h> 
#include <fstream>

#define FILENAMESIZE 1000
#define STRING_SIZE 256

char * readNextID(FILE * filename){
	char * line = (char *)malloc(sizeof(char) * FILENAMESIZE);					//allocate memory for file name array

	do {
		fgets(line, FILENAMESIZE, filename);
		//if(feof(file)){cout <<"end of file reached" <<endl;} //decided this is not needed
	} while (line[0] == '\n');

	for(int n=0;n<FILENAMESIZE;n++){
		if(line[n]=='\n' || line[n]=='\r'){line[n]='\0';}				//remove newline character from file name
	}

	return line;
}


FILE * getPositionFile1(char * ID){
	char * txt = ".txt";
	char * positionfilename = (char *)malloc(STRING_SIZE);
	sprintf(positionfilename, "/home/user1/nolting/Desktop/datastuffs/morepos/pos.0.%s%s", ID, txt);
	FILE * position = fopen(positionfilename,"r");
	free(positionfilename);
	return position;
}

FILE * getPositionFile2(char * ID){
	char * txt = ".txt";
	char * positionfilename = (char *)malloc(STRING_SIZE);
	sprintf(positionfilename, "/home/user1/nolting/Desktop/datastuffs/morepos/pos.1e12.0.%s%s", ID, txt);
	FILE * position = fopen(positionfilename,"r");
	free(positionfilename);
	return position;
}


FILE * makeOutputFileName(char * ID){
	char * txt = ".txt";
	char * directPotentialfilename = (char *)malloc(STRING_SIZE);
	strcpy(directPotentialfilename, "/home/user1/nolting/Desktop/output/w2/directpotential/directPotential.");	//direct potential file path
	
	strcat(directPotentialfilename, ID);
	strcat(directPotentialfilename, txt);
	
	
	FILE * directPotentialFile = fopen(directPotentialfilename,"w+");
	
	free(directPotentialfilename);
	return directPotentialFile;
}

FILE * makeMassFileName(char * ID){
	char * txt = ".txt";
	char * massfilename = (char *)malloc(STRING_SIZE);
	strcpy(massfilename, "/home/user1/nolting/Desktop/output/w2/mass/mass.");	//direct potential file path
	
	strcat(massfilename, ID);
	strcat(massfilename, txt);
	
	
	FILE * massFile = fopen(massfilename,"w+");
	
	free(massfilename);
	return massFile;
}
