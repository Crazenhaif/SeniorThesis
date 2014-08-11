#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <cmath>
#include "arrayconstructor.h"
#include "filestuff.h"
#include "halo.h"
#include "Particle.h"
#include "constants.h"
#include "xorshift.h"

using namespace std;

static inline double sq(double x) {
	return x*x;
}

// ------------- definitions -------------------------//

//#define ID_DIRECTORY_NAME "/home/chris/Desktop/Thesis/source_files/id_directory_large.txt"
#define ID_DIRECTORY_NAME "/home/chris/Desktop/Thesis/source_files/id_directory_small.txt"
#define OUTNAME "/home/chris/Desktop/Thesis/output/equilibrium/outputfile.txt"
#define resolution 1000	//break it into *1000* linear radius bins
#define massMult 1	//should be 1 for the small mass halos and 10 for the large mass halos
#define critNum 200	//*200* times the critical density
	// =================== Main ====================

int main() {

	
	///*
	// ============ open directories and files ==============
	FILE * idDirectoryFile = fopen(ID_DIRECTORY_NAME,"r");
	FILE * outfile = fopen(OUTNAME,"w+");
	
	int numberOfHalos = getNumberOfRows(idDirectoryFile);			// count number of Halos

	/// --------------BEGIN FOR LOOP ----------------------//
	for(int n=0;n<numberOfHalos;n++){					// for each Halo do this:
		char * HaloID = readNextID(idDirectoryFile);			// read in Halo ID number
		datasource_t ds = makeSourceFileNames(HaloID);			// create all source file names from ID number
		outputfiles_t output = makeOutputFileNames(HaloID);
		//
		Halo * currentHalo = new Halo(ds);				// initialize halo with data input from files
		closeFiles(ds);	
		currentHalo->ID = HaloID;							// assign the ID number to the Halo class object
			
		// ============== center positions ==================
		currentHalo->findCoM();
		currentHalo->centerPositions();			//subtract the position of the center (center of mass) from each position to remove the offset.
								//also calculates the radius from the CoM for each particle
		
		currentHalo->findMaxMinRadius();  //find the minimum and maximum radius and assign their values for the halo		
		if(currentHalo->maxRadius > 2){cout <<"Halo ID " <<currentHalo->ID <<"has max radius " <<currentHalo->maxRadius <<endl;}

		// =============== find the critical radius (radius at which crit density is reached) ===============
		
		currentHalo->rCrit = currentHalo->findCritRadius(resolution, massMult, critNum);

		// =============== print the critical radius and the ratio rCrit/rMax =========================
		// =============== where rMax is the radius of the farthest particle from the CoM ==============
		currentHalo->radiusToDeepCenter = sqrt(sq(currentHalo->centerOfMass.x-currentHalo->deepCenter.x)+sq(currentHalo->centerOfMass.y-currentHalo->deepCenter.y)+sq(currentHalo->centerOfMass.z-currentHalo->deepCenter.z));
		currentHalo->ratio = currentHalo->rCrit/currentHalo->maxRadius;
//		cout <<"max r is " <<currentHalo->maxRadius <<endl;
//		cout <<"rCrit is " <<currentHalo->rCrit <<endl;
		currentHalo->printOutput(output);			//print all the output files
		

		fprintf(outfile,"%s %f %f %f %f \n", currentHalo->ID, currentHalo->rCrit, currentHalo->ratio, currentHalo->radiusToDeepCenter, currentHalo->maxRadius);
		closeFiles(output);
		
		int remaining = numberOfHalos - n - 1;
//		cout <<"Halo ID " <<HaloID <<"'s status has been determined. " <<remaining <<" Halos remaining." <<endl;						
		
		delete currentHalo;
	} //end for halo loop, repeat for each halo.
	fclose(outfile);
	fclose(idDirectoryFile);			//close ID directory File
	cout <<"Program has reached completion" <<endl;		//note that the final iteration has completed and the program will terminate

	return 0;
}

