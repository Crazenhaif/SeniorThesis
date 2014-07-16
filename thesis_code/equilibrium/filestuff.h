#ifndef FILE_HEADER
#define FILE_HEADER

#include "constants.h"
#include "halo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// Prototypes
char * readNextID(FILE * file);
FILE * makePhi0FileName(int phi0value);
datasource_t makeSourceFileNames(char * ID);
outputfiles_t makeOutputFileNames(char * ID);
int countCharactersTil(char * thing, char end);
void closeFiles(outputfiles_t out);
void closeFiles(datasource_t data);


#define FILENAMESIZE 1000


//Functions


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


datasource_t makeSourceFileNames(char * ID){
	//  When doing the second set, change the strcpy function.
	//  
	char * positionfilename = (char *)malloc(STRING_SIZE);
	char * txt = ".txt";
	//sprintf(positionfilename, "/home/chris/Desktop/Thesis/source_files/position/pos.0.%s%s", ID, txt);  //large mass range
	sprintf(positionfilename, "/home/chris/Desktop/Thesis/source_files/position/pos.1e12.0.%s%s", ID, txt);  //small mass range
		//OLD VERSION:	strcpy(positionfilename, "/home/user1/nolting/Desktop/datastuffs/position/pos.0.");
	char * potentialfilename = (char *)malloc(STRING_SIZE);
	sprintf(potentialfilename, "/home/chris/Desktop/Thesis/output/w2/directpotential/directPotential.%s%s", ID, txt);	
	//OLD VERSION:	strcpy(potentialfilename, "/home/user1/nolting/Desktop/output/w2/directpotential/directPotential.");	// Potential calculated using W2 kernal
	

	//strcat(positionfilename, ID);
	//strcat(positionfilename, txt);

	//strcat(potentialfilename, ID);
	//strcat(potentialfilename, txt);
	
	datasource_t ds;
	
	FILE * position = fopen(positionfilename,"r");
	if(position==NULL){cout <<"Position file null" <<endl;}
	FILE * potential = fopen(potentialfilename,"r");
	if(potential==NULL){cout <<"Potential file null" <<endl;}

	free(positionfilename);
	free(potentialfilename);

	ds.positionFile = position;
	ds.potentialFile = potential;

	return ds;
}



outputfiles_t makeOutputFileNames(char * ID){
	//  File locations will be of the form:
	//  C:/Users/Chris/Desktop/thesis_files/position_files/pos.0.1594.txt
	char * txt = ".txt";
	char * notesfilename = (char *)malloc(STRING_SIZE);
	strcpy(notesfilename, "/home/chris/Desktop/Thesis/output/equilibrium/notes/notes.");			//notes on the halo

	strcat(notesfilename, ID);
	strcat(notesfilename, txt);

	char * ratiofilename = (char *)malloc(STRING_SIZE);
	strcpy(ratiofilename, "/home/chris/Desktop/Thesis/output/equilibrium/ID_ratio/id_ratio.");			//notes on the halo

	strcat(ratiofilename, ID);
	strcat(ratiofilename, txt);

	outputfiles_t out;

	FILE * notes = fopen(notesfilename,"w+");
	FILE * ratio = fopen(ratiofilename,"w+");
	//--
	free(notesfilename);
	free(ratiofilename);
	//--
	out.notes = notes;
	out.ratio = ratio;

	return out;
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

void closeFiles(outputfiles_t out){
	
	//fclose(out.newPositonsFile);
	//fclose(out.radiusFile);
	fclose(out.notes);
	fclose(out.ratio);
}

void closeFiles(datasource_t ds){
	
	fclose(ds.positionFile);
	fclose(ds.potentialFile);

}

#endif
