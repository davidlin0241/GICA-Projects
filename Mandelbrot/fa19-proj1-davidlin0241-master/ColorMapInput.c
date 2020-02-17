/*********************
**  Color Map generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"


/**************
**This function reads in a file name colorfile.
**It then uses the information in colorfile to create a color array, with each color represented by an int[3].
***************/
uint8_t** FileToColorMap(char* colorfile, int* colorcount)
{
	//YOUR CODE HERE

  //First line contains the specified number of colors
  //All other lines are rgdb values
  //Return null pointer if file is malformed or doesn't exist (and free data)
  //if specified length < actual, do only first n colors
  //Need to keep track of n.

  //Use 2D array - array of pointers

  unsigned * numColors = (unsigned*) malloc(sizeof(unsigned));
  if (numColors == NULL) {
    printf("Unable to allocate for numColors in FileToColorMap");
    return NULL;
  }

  int i=0, j=0, numRead;
  int m1, m2, m3;
  uint8_t n1, n2, n3;
  uint8_t** colorPtr;

  FILE* filePtr = fopen(colorfile, "r");

  if (filePtr == NULL) {
    free(numColors);
    printf("No such file");
    return NULL;
  }

  numRead = fscanf(filePtr,  "%u", numColors); //first line
  if (numRead != 1 || *numColors < 1) {
    free(numColors);
    fclose(filePtr);
    printf("File is malformatted"); //also covers case if num colors is greater than actual
    return NULL;
  }

  colorPtr = (uint8_t **) malloc(sizeof(uint8_t*) * *numColors);

  if (colorPtr == NULL) {
    free(numColors);
    fclose(filePtr);
    printf("Unable to allocate for colorPtr in FileToColorMap");
    return NULL;
  }

  for (i = 0; i < *numColors; i++) { //covers case: numColors < actual
      numRead = fscanf(filePtr, "%i %i %i", &m1, &m2, &m3);

      if (numRead != 3) {
        free(numColors);
        fclose(filePtr);
        printf("File is malformatted"); //also covers case if num colors is greater than actual
        for (j = 0; j < i; j++) {
          free(colorPtr[j]);
        }
        free(colorPtr);
        return NULL;
      }

      if (0 <= m1 && m1 <= 255 &&
          0 <= m2 && m2 <= 255 &&
          0 <= m3 && m3 <= 255) {
        n1 = (uint8_t) m1;
        n2 = (uint8_t) m2;
        n3 = (uint8_t) m3;
        colorPtr[i] = (uint8_t *) malloc(sizeof(uint8_t) * 3);

        if (colorPtr[i] == NULL) {
          free(numColors);
          fclose(filePtr);
          printf("Unable to allocate for colorPtr[i] in FileToColorMap");
          for (j = 0; j < i; j++) {
            free(colorPtr[j]);
          }
          free(colorPtr);
          return NULL;
        }

        colorPtr[i][0] = n1;
        colorPtr[i][1] = n2;
        colorPtr[i][2] = n3;
      } else {
        printf("Numbers not between 0-255"); //also covers case if num colors is greater than actual
        free(numColors);
        fclose(filePtr);
        for (j = 0; j < i; j++) {
          free(colorPtr[j]);
        }
        free(colorPtr);
        return NULL;
      }
  }

  *colorcount = i;

  free(numColors);
  fclose(filePtr);
  return colorPtr;
}

/*
while (fscanf(filePtr, "%hhu %hhu %hhu", &n1, &n2, &n3) != EOF && *numColors > i) { //covers: numColors < actual

  colorPtr[i] = (uint8_t *) malloc(sizeof(uint8_t) * 3);

  if (colorPtr[i] == NULL) {
    printf("Unable to allocate for colorPtr[i] in FileToColorMap");
    return NULL;
  }

  colorPtr[i][0] = n1;
  colorPtr[i][1] = n2;
  colorPtr[i][2] = n3;
  i++;
  //TODO: Check all 3 are 0-255 and non numbers and first line
}
*/
