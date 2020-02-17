/*********************
**  Color Palette generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"
//You don't need to call this function but it helps you understand how the arguments are passed in
void usage(char* argv[])
{
	printf("Incorrect usage: Expected arguments are %s <inputfile> <outputfolder> <width> <heightpercolor>", argv[0]);
}

//Creates a color palette image for the given colorfile in outputfile. Width and heightpercolor dictates the dimensions of each color. Output should be in P3 format
int P3colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile)
{
	//YOUR CODE HERE

	FILE *filePtr = fopen(outputfile, "w");

	if (filePtr == NULL) {
      printf("Could not open file");
      return 1;
  }

	int* lenColorfilePtr = (int*) malloc(sizeof(int));
	if (lenColorfilePtr == NULL) {
			printf("Could not allocate space for ptr");
			return 1;
	}

	uint8_t** colorPtr;
	int i, j, k;
	uint8_t n1, n2, n3;

	colorPtr = FileToColorMap(colorfile, lenColorfilePtr);

	int lenColorfile = *lenColorfilePtr;

	if (width < 1 || heightpercolor < 1 || lenColorfile < 1 ) {
		fclose(filePtr);
		free(lenColorfilePtr);
		free(colorPtr);
		return 1;
	}

	fprintf(filePtr, "P3 %i %i %u\n", width, heightpercolor * lenColorfile, 255);

	//Need to iterate over colorPtr
	//Need to iterate over each array/pointer within colorPtr
	//Need to consider width and heighpercolor
	//The first heightpercolor rows should be colored entirely with the first color in colorfile.
	for (k = 0; k < lenColorfile; k++) { //len of color files
		for (i = 0; i < heightpercolor; i++) { //heightpercolor rows for each color
			for (j = 0; j < width; j++) { //repeat width times

				n1 = colorPtr[k][0], n2 = colorPtr[k][1], n3 = colorPtr[k][2];
				fprintf(filePtr, "%hhu %hhu %hhu", n1, n2, n3);

				if (j != width - 1)
					fprintf(filePtr, " ");
			}
			fprintf(filePtr, "\n");
		}
	}

	free(lenColorfilePtr);
	i = 0;
	for (; i < lenColorfile; i++) {
		free(colorPtr[i]);
	}
	free(colorPtr);


	fclose(filePtr);
	return 0;
}

//Same as above, but with P6 format
int P6colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile) {
	FILE *filePtr = fopen(outputfile, "w");

	if (filePtr == NULL) {
      printf("Could not open file");
      return 1;
  }

	int* lenColorfilePtr = (int*) malloc(sizeof(int));

	if (lenColorfilePtr == NULL) {
			printf("Could not allocate space for ptr");
			return 1;
	}

	uint8_t** colorPtr;
	int i, j, k;

	colorPtr = FileToColorMap(colorfile, lenColorfilePtr);
	int lenColorfile = *lenColorfilePtr;

	if (width < 1 || heightpercolor < 1 || lenColorfile < 1 ) {
		fclose(filePtr);
		free(lenColorfilePtr);
		free(colorPtr);
		return 1;
	}

	fprintf(filePtr, "P6 %i %i %u\n", width, heightpercolor * lenColorfile, 255);

	for (k = 0; k < lenColorfile; k++) { //len of color files
		for (i = 0; i < heightpercolor; i++) { //heightpercolor rows for each color
			for (j = 0; j < width; j++) { //repeat width times
				//fprintf(filePtr, "%u %u %u", colorPtr[i][0], colorPtr[i][1], colorPtr[i][2]);
				fwrite(colorPtr[k], 1 , 3 , filePtr);
			}
		}
	}

	free(lenColorfilePtr);
	for (i = 0; i < lenColorfile; i++) {
		free(colorPtr[i]);
	}
	free(colorPtr);

	fclose(filePtr);
	return 0;
}


//The one piece of c code you don't have to read or understand. Still, might as well read it, if you have time.
int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		usage(argv);
		return 1;
	}
	int width = atoi(argv[3]);
	int height = atoi(argv[4]);
	char* P3end = "/colorpaletteP3.ppm";
	char* P6end = "/colorpaletteP6.ppm";
	char buffer[strlen(argv[2]) + strlen(P3end)+1];
	sprintf(buffer, "%s%s", argv[2], P3end);
	int failed = P3colorpalette(argv[1], width, height, buffer);
	if (failed)
	{
		printf("Error in making P3colorpalette");
		return 1;
	}
	sprintf(buffer, "%s%s", argv[2], P6end);
	failed = P6colorpalette(argv[1], width, height, buffer);
	if (failed)
	{
		printf("Error in making P6colorpalette");
		return 1;
	}
	printf("P3colorpalette and P6colorpalette ran without error, output should be stored in %s%s, %s%s", argv[2], P3end, argv[2], P6end);
	return 0;
}
