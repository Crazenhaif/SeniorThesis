#ifndef ARRAY_CONSTRUCTOR
#define ARRAY_CONSTRUCTOR

	// Included Files.
#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "constants.h"

using namespace std;
	
// Constants
#define BUFFER_SIZE 1024

	// Function Prototypes
int			getNumberOfColumns(FILE * file);
int			getNumberOfRows(FILE * file);
double *	getNextRowOfFile(FILE * file, int numberOfColumns);
double **	assembleArray(FILE * file, const int numberOfRows, const int numberOfColumns);
void		closeArray(double ** arrayToClose, int rows);
double **	assembleArray(const int numberOfRows, const int numberOfColumns);
int			printArray(double ** arrayToPrint, int numberOfRows, int numberOfColumns);
int			printArray(int * arrayToPrint, int numberOfRows);
int			printArray(double * arrayToPrint, int numberOfRows);
//int			printArrayToFile(FILE * file, double ** arrayToPrint, int numberOfRows, int numberOfColumns);
int			printArrayToFile(FILE * file, double * arrayToPrint, int numberOfRows);

double		findMin(double ** dataArray, int numberOfRows, int numberOfColumns);
double		findMax(double ** dataArray, int numberOfRows, int numberOfColumns);
double **	copyArray(double ** arrayToCopy, int numberOfRows, int numberOfColumns);



#endif
