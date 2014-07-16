#ifndef FILE_HEADER
#define FILE_HEADER

#include "constants.h"
#include "halo.h"

/// Prototypes
char * readNextID(FILE * file);
FILE * makePhi0FileName(char * phi0);
datasource_t makeSourceFileNames(char * ID);
outputfiles_t makeOutputFileNames(char * ID);
int countCharactersTil(char * thing, char end);
void closeFiles(outputfiles_t out);
void closeFiles(datasource_t data);

//Functions


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
	//  /home/user1/nolting/Desktop/datastuffs/position/pos.0.1594.txt
	char * positionfilename = (char *)malloc(STRING_SIZE);
	//strcpy(positionfilename, "/home/user1/nolting/Desktop/datastuffs/position/pos.0.");			//file path large halos
		strcpy(positionfilename, "/home/user1/nolting/Desktop/datastuffs/position/pos.1e12.0.");	//file path small halos
	char * velocityfilename = (char *)malloc(STRING_SIZE);
	//strcpy(velocityfilename, "/home/user1/nolting/Desktop/datastuffs/velocity/vel.0.");
		strcpy(velocityfilename, "/home/user1/nolting/Desktop/datastuffs/velocity/vel.1e12.0.");
	char * potentialfilename = (char *)malloc(STRING_SIZE);
	strcpy(potentialfilename, "/home/user1/nolting/Desktop/output/w2/directpotential/directPotential.");	// Potential calculated using W2 kernal
	char * txt = ".txt";

	strcat(positionfilename, ID);
	strcat(positionfilename, txt);

	strcat(velocityfilename, ID);
	strcat(velocityfilename, txt);

	strcat(potentialfilename, ID);
	strcat(potentialfilename, txt);
	
	datasource_t ds;

	FILE * position = fopen(positionfilename,"r");
	FILE * velocity = fopen(velocityfilename,"r");
	FILE * potential = fopen(potentialfilename,"r");
if(position == NULL){cout <<"null position file" <<endl;}
if(velocity == NULL){cout <<"null velocity file" <<endl;}
if(potential == NULL){cout <<"null potential file" <<endl;}
	free(positionfilename);
	free(velocityfilename);
	free(potentialfilename);

	ds.positionFile = position;
	ds.potentialFile = potential;
	ds.velocityFile = velocity;

	return ds;
}



outputfiles_t makeOutputFileNames(char * ID){
	//  File locations will be of the form:
	//  C:/Users/Chris/Desktop/thesis_files/position_files/pos.0.1594.txt
	char * txt = ".txt";
	//char * newpositionfilename = (char *)malloc(STRING_SIZE);
	//strcpy(newpositionfilename, "C:/Users/Chris/Desktop/thesis_files/positionwrite/newpos.");	//position after centering
	//char * newvelocityfilename = (char *)malloc(STRING_SIZE);
	//strcpy(newvelocityfilename, "C:/Users/Chris/Desktop/thesis_files/velocitywrite/newvel.");	//velocity after subtraction of halo velocity
	//char * newpotentialfilename = (char *)malloc(STRING_SIZE);
	//strcpy(newpotentialfilename, "C:/Users/Chris/Desktop/thesis_files/potentialwrite/newpot.");	//potential after offset removal
	char * radiusfilename = (char *)malloc(STRING_SIZE);
	strcpy(radiusfilename, "/home/user1/nolting/Desktop/DataAnalysisProgram/output/radius/radius.");			//radius from center after position centering
	char * densityfilename = (char *)malloc(STRING_SIZE);
	strcpy(densityfilename, "/home/user1/nolting/Desktop/DataAnalysisProgram/output/density/density/density.");		//density distribution by log radius bin
	char * linenergybinsfilename = (char *)malloc(STRING_SIZE);
	strcpy(linenergybinsfilename, "/home/user1/nolting/Desktop/DataAnalysisProgram/output/bins/linenergybins/linenergybins.");	//linear energy bin edges
	char * linradbinsfilename = (char *)malloc(STRING_SIZE);
	strcpy(linradbinsfilename, "/home/user1/nolting/Desktop/DataAnalysisProgram/output/bins/linradbins/linradbins.");			//linear radius bin edges
	char * logbinsfilename = (char *)malloc(STRING_SIZE);
	strcpy(logbinsfilename, "/home/user1/nolting/Desktop/DataAnalysisProgram/output/bins/logbins/logbins.");				//log radius bin edges
	//char * energyfilename = (char *)malloc(STRING_SIZE);
	//strcpy(energyfilename, "C:/Users/Chris/Desktop/thesis_files/energywrite/energy.");			//total energy per particle
	char * notesfilename = (char *)malloc(STRING_SIZE);
	strcpy(notesfilename, "/home/user1/nolting/Desktop/DataAnalysisProgram/output/notes/notes.");			//notes on the halo
	//char * avepotfilename = (char *)malloc(STRING_SIZE);
	//strcpy(avepotfilename, "C:/Users/Chris/Desktop/thesis_files/avepot_files/avepot.");			//spherically average potential by linear radius bin
	char * vrfilename = (char *)malloc(STRING_SIZE);
	strcpy(vrfilename, "/home/user1/nolting/Desktop/DataAnalysisProgram/output/vr/vr.");					//cumulative VR by linear radius bin
	char * energyBinsfilename = (char *)malloc(STRING_SIZE);
	strcpy(energyBinsfilename, "/home/user1/nolting/Desktop/DataAnalysisProgram/output/N/N.");	//number per linear energy bin
	//char * kineticfilename = (char *)malloc(STRING_SIZE);
	//strcpy(kineticfilename, "C:/Users/Chris/Desktop/thesis_files/energywrite/kinetic.");		//kinetic energy per particle
	char * numberPerBinRadiusfilename = (char *)malloc(STRING_SIZE);
	strcpy(numberPerBinRadiusfilename, "/home/user1/nolting/Desktop/DataAnalysisProgram/output/density/numberPerBin/numberPerBin.");		//number per bin for the log radius bins (for density)

	//strcat(newpositionfilename, ID);
	//strcat(newpositionfilename, txt);
	//strcat(newvelocityfilename, ID);
	//strcat(newvelocityfilename, txt);
	//strcat(newpotentialfilename, ID);
	//strcat(newpotentialfilename, txt);
	strcat(radiusfilename, ID);
	strcat(radiusfilename, txt);
	strcat(densityfilename, ID);
	strcat(densityfilename, txt);
	strcat(linenergybinsfilename, ID);
	strcat(linenergybinsfilename, txt);
	strcat(logbinsfilename, ID);
	strcat(logbinsfilename, txt);
	//strcat(energyfilename, ID);
	//strcat(energyfilename, txt);
	strcat(notesfilename, ID);
	strcat(notesfilename, txt);
	//strcat(avepotfilename, ID);
	//strcat(avepotfilename, txt);
	strcat(vrfilename, ID);
	strcat(vrfilename, txt);
	strcat(energyBinsfilename, ID);
	strcat(energyBinsfilename, txt);
	//strcat(kineticfilename, ID);
	//strcat(kineticfilename, txt);
	strcat(linradbinsfilename, ID);
	strcat(linradbinsfilename, txt);
	strcat(numberPerBinRadiusfilename, ID);
	strcat(numberPerBinRadiusfilename, txt);

	outputfiles_t out;

	//FILE * newposition = fopen(newpositionfilename,"w+");
	//FILE * newvelocity = fopen(newvelocityfilename,"w+");
	//FILE * newpotential = fopen(newpotentialfilename,"w+");
	FILE * radius = fopen(radiusfilename,"w+");
	FILE * density = fopen(densityfilename,"w+");
	FILE * linenergybins = fopen(linenergybinsfilename,"w+");
	FILE * logbins = fopen(logbinsfilename,"w+");
	//FILE * energy = fopen(energyfilename,"w+");
	FILE * notes = fopen(notesfilename,"w+");
	//FILE * avepot = fopen(avepotfilename,"w+");
	FILE * vr = fopen(vrfilename,"w+");
	FILE * energyBins = fopen(energyBinsfilename,"w+");
	//FILE * kinetic = fopen(kineticfilename,"w+");
	FILE * linradbins = fopen(linradbinsfilename,"w+");
	FILE * numberperbin = fopen(numberPerBinRadiusfilename,"w+");
	//--
	//free(newpositionfilename);
	//free(newvelocityfilename);
	//free(newpotentialfilename);
	free(radiusfilename);
	free(densityfilename);
	free(linenergybinsfilename);
	free(logbinsfilename);
	//free(energyfilename);
	free(notesfilename);
	//free(avepotfilename);
	free(vrfilename);
	free(energyBinsfilename);
	//free(kineticfilename);
	free(linradbinsfilename);
	free(numberPerBinRadiusfilename);
	//--
	//out.newPositonsFile = newposition;
	//out.newVelocitiesFile = newvelocity;
	//out.newPotentialsFile = newpotential;
	out.radiusFile = radius;
	out.densityFile = density;
	out.linEnergyBins = linenergybins;
	out.linRadBins = linradbins;
	out.logRadBins = logbins;
	//out.energy = energy;
	out.notes = notes;
	//out.sphericallyAveragedPotential = avepot;
	out.cumulativeVirialRatio = vr;
	out.energyBinsFile = energyBins;
	//out.kinetic = kinetic;
	out.numberPerBinRadiusFile = numberperbin;

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
	//fclose(out.newVelocitiesFile);
	//fclose(out.newPotentialsFile);
	fclose(out.radiusFile);
	fclose(out.densityFile);
	fclose(out.linRadBins);
	fclose(out.linEnergyBins);
	fclose(out.logRadBins);
	//fclose(out.energy);
	fclose(out.notes);
	//fclose(out.sphericallyAveragedPotential);
	fclose(out.cumulativeVirialRatio);
	fclose(out.energyBinsFile);
	//fclose(out.kinetic);
	fclose(out.numberPerBinRadiusFile);

}

void closeFiles(datasource_t ds){
	
	fclose(ds.positionFile);
	fclose(ds.potentialFile);
	fclose(ds.velocityFile);

}

FILE * makePhi0FileName(char * phi0char){
	//FILE * phi0File = fopen("/home/user1/nolting/Desktop/Fitting/DARKexp/DARKexp.phi0.1.0.txt.","r");	
	
	char * phi0FileName = (char *)malloc(STRING_SIZE);
	strcpy(phi0FileName, "/home/user1/nolting/Desktop/Fitting/DARKexp/DARKexp.phi0.");
	char * txt = ".0.txt";

//	itoa(phi0value,phi0valuechar,10);

	strcat(phi0FileName, phi0char);
	strcat(phi0FileName, txt);

	FILE * phi0File = fopen(phi0FileName,"r");

	free(phi0FileName);
	return phi0File;
}

#endif
