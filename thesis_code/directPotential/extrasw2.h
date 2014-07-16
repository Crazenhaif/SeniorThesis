#ifndef EXTRA_H
#define EXTRA_H
#include <fstream>
#include <string.h>

char * readNextID(FILE * file);

FILE * getPositionFile(char * ID);
FILE * makeOutputFileName(char * ID);
FILE * makeMassFileName(char * ID);

#endif
