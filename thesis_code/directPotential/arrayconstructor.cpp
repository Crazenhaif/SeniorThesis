#ifndef ARRAY_CONSTRUCTOR_CPP
#define ARRAY_CONSTRUCTOR_CPP

#include "arrayconstructor.h"
#define delimeter ' '

int getNumberOfColumns(FILE * file) {
	FILE * tempFile = file;
	
	char line [BUFFER_SIZE];
		// Zero our memory.  memset(buffer, 0, sizeof(buffer);
	memset(line, 0, sizeof(line));
		// Get the line.
	fgets(line, BUFFER_SIZE, tempFile);

		// comparrison constants.
	const char endline = '\0';
	int columnTracker = 0;

	int n = 0;
	while (n <= strlen(line)) {
		if (line[n] == delimeter || line[n] == endline)
			if(line[n+1] != delimeter){++columnTracker;}
		n++;
	}


	rewind(tempFile);
	return columnTracker;
}

int getNumberOfRows(FILE * file) {
	int numberOfRows=0;
	char line [BUFFER_SIZE] = "\n";

	while(!feof(file)){
		strcpy(line,"\0");						//each time set the line to be blank
		fgets(line, BUFFER_SIZE, file);			//get the new line
		if(strlen(line) == 0 && feof(file)){	//break if we're at the end of the file
			break;
		}
		
		if(strlen(line)>1 && line[0]!='\n' && line[0]!='\0'){	//dont count blank lines
			numberOfRows++;
		}
	}

	rewind(file);
	return numberOfRows;
}

double * getNextRowOfFile(FILE * file, const int numberOfColumns) {	
	double * row = (double *)malloc(sizeof(double) * numberOfColumns);	// Array to hold return values.
	char ** stringArray = (char **)malloc(4 * numberOfColumns);			// Array to hold string values before conversion to floats.
	char line [BUFFER_SIZE];	
		// Get our line.
	do {
		fgets(line, BUFFER_SIZE, file);
	} while (line[0] == '\n');
		// Parse the line.
	int i = 0;
	int index = 0;
	int stringLength = 0;

	while (line[i] != '\0') {		// until EOF:
		if (line[i] == delimeter || line[i] == '\n') {	// If it is the delimeter or endline, it is the end of a clump.
			if(line[i-1] == delimeter){ i++; continue; }
			char * holder = (char *)malloc(stringLength + 1);		// Allocate space on the heap for the clump.
			strncpy(holder, &line[i-stringLength], stringLength);	// Copy the clump into that space.
			holder[stringLength] = '\0';							// Add a null-termination character (EOF, end of line)						
			stringArray[index] = holder;							// Copy the pointer to the clump into our array of clumps.
			stringLength = 0;										// reset our clump length
			index++;												// Increment index so we copy into the next spot in clump array
		} else if (line[i + 1] == '\0') {
			++i;													// Peeking so we need to include current character too.
			++stringLength;											// Length includes current character.
			char * holder = (char *)malloc(stringLength + 1);		// Same as above.
			strncpy(holder, &line[i-stringLength], stringLength);	// same as above.
			holder[stringLength] = '\0';							// Same as above.
			stringArray[index] = holder;							// Same as above.
			index++;												// increment index as before!!!!!
			break;
		} else {
			stringLength++;
		}
		i++;
	}
	
		// Convert the strings to doubles.
	for (int k = 0; k < index; k++) {
		row[k] = atof(stringArray[k]); // atof takes in a string (char *) and gives you a double.
	}
		// Free up our stringsArray
	for (int i = 0; i < index; i++) {
		free(stringArray[i]);
	}
	free(stringArray);
	return row;
}

double ** assembleArray(FILE * file, const int numberOfRows, const int numberOfColumns) {	
	rewind(file);
	
	double * row; // = (double *)malloc(sizeof(double) * numberOfColumns);
	double ** finalArray = new double* [numberOfRows];

	for (int n=0; n<numberOfRows;n++){
		row = getNextRowOfFile(file, numberOfColumns);
		finalArray[n]=row;
	}

	return finalArray;
}


int printArray(double ** arrayToPrint, int numberOfRows, int numberOfColumns){
	for(int n=0;n<numberOfRows;n++){
		for(int m=0; m<numberOfColumns;m++){
			cout <<arrayToPrint[n][m] <<delimeter;
		}
		cout <<endl;
	}
return 0;
}

int printArray(int * arrayToPrint, int numberOfRows){

	for(int n=0;n<numberOfRows;n++){
		cout <<arrayToPrint[n];
		cout <<endl;
	}

return 0;
}

int printArray(double * arrayToPrint, int numberOfRows){

	for(int n=0;n<numberOfRows;n++){
		cout <<arrayToPrint[n];
		cout <<endl;
	}

return 0;
}
/*
int printArrayToFile(FILE * file, double ** arrayToPrint, int numberOfRows, int numberOfColumns){

	for(int i=0;i<numberOfRows;i++){

		for(int j=0;j<numberOfColumns;j++){

			fprintf(file,"%f%c",arrayToPrint[i][j],delimeter);
			}
		fprintf(file,"\n");
		}

	return 0;
}
*/
int printArrayToFile(FILE * file, double * arrayToPrint, int numberOfRows){

	for(int i=0;i<numberOfRows;i++){

			fprintf(file,"%f%c",arrayToPrint[i],delimeter);
			fprintf(file,"\n");
		}

	return 0;
}

double findMin(double * dataArray, int numberOfRows){

	double tempMin = dataArray[0];
	int tempRow = 0;
	for(int i = 0; i<numberOfRows;i++){

			if(dataArray[i]<=tempMin){tempMin = dataArray[i]; tempRow=i;}

	}

	return tempMin;

}

double findMin(double ** dataArray, int numberOfRows, int numberOfColumns){

	double tempMin = dataArray[0][0];
	int tempRow = 0;
	int tempCol = 0;
	for(int i = 0; i<numberOfRows;i++){

		for(int j = 0; j<numberOfColumns; j++){

			if(dataArray[i][j]<=tempMin){tempMin = dataArray[i][j]; tempRow=i; tempCol=j;}

		}

	}

	return tempMin;

}

double findMax(double * dataArray, int numberOfRows){

	double tempMax = dataArray[0];
	int tempRow = 0;
	for(int i = 0; i<numberOfRows;i++){

			if(dataArray[i]>=tempMax){tempMax = dataArray[i]; tempRow=i;}

	}

	return tempMax;

}


double findMax(double ** dataArray, int numberOfRows, int numberOfColumns){

	double tempMax = dataArray[0][0];
	int tempRow = 0;
	int tempCol = 0;
	for(int i = 0; i<numberOfRows;i++){

		for(int j = 0; j<numberOfColumns; j++){

			if(dataArray[i][j]>=tempMax){tempMax = dataArray[i][j]; tempRow=i; tempCol=j;}

		}

	}


	return tempMax;

}

#endif