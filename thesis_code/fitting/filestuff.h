#ifndef FILE_HEADER
#define FILE_HEADER

#include "constants.h"
#include <stdlib.h>
#include <iostream>
using namespace std;
#include <stdio.h> 
#include <string.h>


struct datasource_t {			//source file pointers
	FILE * densityFile;			//density file
	FILE * numberperbinFile;	//number per log radius bin file
	FILE * logbinsFile;			//log radius bin edges file
	FILE * NFile;		//number per linear energy bin file
	FILE * linenergybinsFile;	//linear energy bin edges file
};

/// Prototypes
string readNextID(FILE * file);
FILE * makePhi0FileName(string phi0);
datasource_t makeSourceFileNames(string ID);
int countCharactersTil(char * thing, char end);
void closeFiles(datasource_t data);

#endif
