/*********************
**  Mandelbrot fractal movie generator
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include "ColorMapInput.h"
#include <sys/types.h>

void printUsage(char* argv[])
{
  printf("Usage: %s <threshold> <maxiterations> <center_real> <center_imaginary> <initialscale> <finalscale> <framecount> <resolution> <output_folder> <colorfile>\n", argv[0]);
  printf("    This program simulates the Mandelbrot Fractal, and creates an iteration map of the given center, scale, and resolution, then saves it in output_file\n");
}

//2 1536 -0.561397233777 -0.643059076016 2 1e-7 5 100 student_output/partB defaultcolormap.txt

/*
This function calculates the threshold values of every spot on a sequence of frames. The center stays the same throughout the zoom. First frame is at initialscale, and last frame is at finalscale scale.
The remaining frames form a geometric sequence of scales, so
if initialscale=1024, finalscale=1, framecount=11, then your frames will have scales of 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1.
                                                            1024, 1024 * (1/1024)^(1/10)
As another example, if initialscale=10, finalscale=0.01, framecount=5, then your frames will have scale 10, 10 * (0.01/10)^(1/4), 10 * (0.01/10)^(2/4), 10 * (0.01/10)^(3/4), 0.01 .
*/
void MandelMovie(double threshold, u_int64_t max_iterations, ComplexNumber* center, double initialscale, double finalscale, int framecount, u_int64_t resolution, u_int64_t ** output){
    //YOUR CODE HERE
    int i, frac = framecount - 1;
    double scale = initialscale;
    for (i = 0; i < framecount; i++) {
      Mandelbrot(threshold, max_iterations, center, scale, resolution, output[i]);
      scale = initialscale * pow( finalscale/initialscale, ((double) (i+1))/((double) frac ));
    }

}

/**************
**This main function converts command line inputs into the format needed to run MandelMovie.
**It then uses the color array from FileToColorMap to create PPM images for each frame, and stores it in output_folder
***************/
int main(int argc, char* argv[])
{
	//Tips on how to get started on main function:
	//MandelFrame also follows a similar sequence of steps; it may be useful to reference that.
	//Mayke you complete the steps below in order.

	//STEP 1: Convert command line inputs to local variables, and ensure that inputs are valid.
	/*
	Check the spec for examples of invalid inputs.
	Remember to use your solution to B.1.1 to process colorfile.
	*/

  //<threshold> <maxiterations> <center_real> <center_imaginary>
  //<initialscale> <finalscale> <framecount> <resolution> <output_folder>
  //<colorfile>

	//10 args
  if (argc != 11) {
		printf("%s: Wrong number of arguments, expecting 10\n", argv[0]);
		printUsage(argv);
		return 1;
	}

  double threshold, scale, finalscale; //initial scale
	ComplexNumber* center;
  int framecount, i, j;
  int* colorcount = (int*) malloc(sizeof(int));

  if (colorcount == NULL) {
		printf("Unable to allocate for colorcount");
		return 1;
	}

	u_int64_t max_iterations, resolution;
  char* colorfile, *outputfolder;

  //need finanscale framecount outputfolder colorfile

	threshold = atof(argv[1]);
	max_iterations = (u_int64_t)atoi(argv[2]);
	center = newComplexNumber(atof(argv[3]), atof(argv[4]));
	scale = atof(argv[5]);
  finalscale = atof(argv[6]);
  framecount = atoi(argv[7]);
	resolution = (u_int64_t) atoi(argv[8]);
  outputfolder = argv[9];
  colorfile = argv[10];
  uint8_t** colorMap = FileToColorMap(colorfile, colorcount);
  int numColors = *colorcount;

  //colorfile is not properly formatted, as specified in the above description of colorfile.
  //Insufficient memory for output. (Because of this, you should allocate enough memory for output in your main function).
	if (threshold <= 0 || scale <= 0 || max_iterations <= 0
        || framecount > 10000 || framecount <= 0 || resolution < 0
        || (framecount == 1 && scale != finalscale)) {
    free(center);
		printf("Input is invalid");
		printUsage(argv);
		return 1;
	}
	u_int64_t size = 2 * resolution + 1;
  u_int64_t sizesquared = size * size;


	//STEP 2: Run MandelMovie on the correct arguments.
	/*
	MandelMovie requires an output array, so make sure you allocate the proper amount of space.
	If allocation fails, free all the space you have already allocated (including colormap), then return with exit code 1.
	*/

  u_int64_t** output; //i assume this is the colormap
	output = (u_int64_t **) malloc(sizeof(uint64_t*) * framecount);
	if (output == NULL) {
		printf("Unable to allocate %lu bytes\n", sizeof(uint64_t*) * framecount);
		return 1;
	}

  for (i = 0; i < framecount; i++) {
    output[i] = (u_int64_t *) malloc(sizeof(u_int64_t) * sizesquared);

    if (output[i] == NULL) {
      printf("Unable to allocate %lu bytes on iteration %i\n", sizeof(u_int64_t) * sizesquared,i+1);
      for (j = 0; j < i; j++) {
        free(output[j]);
      }
      free(output);

      for (j = 0; j < numColors; j++) {
        free(colorMap[j]);
      }
      free(colorMap);

      return 1;
    }
  }

  MandelMovie(threshold, max_iterations, center, scale, finalscale, framecount, resolution, output);



	//STEP 3: Output the results of MandelMovie to .ppm files.
	/*
	Convert from iteration count to colors, and output the results into output files.
	Use what we showed you in Part B.1.2, create a seqeunce of ppm files in the output folder.
	Feel free to create your own helper function to complete this step.
	As a reminder, we are using P6 format, not P3.
	*/

	//YOUR CODE HERE

  //go through every ele (frame) of  2d array
  //for each frame output the colors for each iteration count
  //colormap
  uint8_t pathSize = 120; //some value from pizza
  char path[pathSize]; //need to hold both output folder and path
  int iters;

  uint8_t** blackColor = (uint8_t **) malloc(sizeof(uint8_t*));

  if (blackColor == NULL) {
    printf("Unable to allocate for blackColor");
    return 1;
  }

  blackColor[0] = (uint8_t *) malloc(sizeof(uint8_t) * 3);

  if (blackColor[0] == NULL) {
    printf("Unable to allocate for blackColor[0]");

    freeComplexNumber(center);
    free(colorcount);

    for (j = 0; j < numColors; j++) {
      free(colorMap[j]);
    }
    free(colorMap);

    for (j = 0; j < framecount; j++) {
      free(output[j]);
    }
    free(output);

    return 1;
  }

  blackColor[0][0] = (uint8_t) 0;
  blackColor[0][1] = (uint8_t) 0;
  blackColor[0][2] = (uint8_t) 0;

  FILE *filePtr;
  for (i = 0; i < framecount; i++) { //each frame
    for (j = 0; j < sizesquared; j++) {

      //"output_folder/" + "frame00000.ppm"
      //sprintf(a, "%s%s%05d", "su", "per", 1);
      //sprintf(a, "%s/frame%05i.ppm", "hi", 5);

      if (j == 0) {
        sprintf(path, "%s/frame%05i.ppm", outputfolder, i);
        filePtr = fopen(path, "w+");

        if (filePtr == NULL) {
            printf("Could not open file");

            freeComplexNumber(center);
            free(colorcount);

            for (j = 0; j < numColors; j++) {
              free(colorMap[j]);
            }
            free(colorMap);

            for (j = 0; j < framecount; j++) {
              free(output[j]);
            }
            free(output);

            free(blackColor[0]);
            free(blackColor);

            return 1;
        }

        fprintf(filePtr, "P6 %lu %lu %u\n", size, size, 255);
      }

      iters = output[i][j];

      if (iters != 0) {
        fwrite(colorMap[(iters - 1) % numColors], sizeof(uint8_t), 3, filePtr);
      } else {
        fwrite(blackColor[0], sizeof(uint8_t), 3, filePtr);
      }
      //iters % len(colormap)
    }
  }

  //"creating mandel frame"


	//STEP 4: Free all allocated memory
	/*
	Make sure there's no memory leak.
	*/
	//YOUR CODE HERE
  fclose(filePtr);
  freeComplexNumber(center);
  free(colorcount);
  for (j = 0; j < numColors; j++) {
    free(colorMap[j]);
  }
  free(colorMap);
  for (j = 0; j < framecount; j++) {
    free(output[j]);
  }
  free(output);
  free(blackColor[0]);
  free(blackColor);
	return 0;
}
