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
char * readNextID(FILE * file);
FILE * makePhi0FileName(char * phi0);
datasource_t makeSourceFileNames(char * ID);
int countCharactersTil(char * thing, char end);
void closeFiles(datasource_t data);

#endif
