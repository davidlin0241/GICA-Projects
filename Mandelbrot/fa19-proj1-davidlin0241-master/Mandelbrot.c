/*********************
**  Mandelbrot fractal
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include <sys/types.h>
#include <math.h>

/*
This function returns the number of iterations before the initial point >= the threshold.
If the threshold is not exceeded after maxiters, the function returns 0.
*/
u_int64_t MandelbrotIterations(u_int64_t maxiters, ComplexNumber * point, double threshold)
{
    //YOUR CODE HERE
   u_int64_t i, numIters = 0;
   double output;
   ComplexNumber *z = newComplexNumber(0, 0);
   ComplexNumber *zSquared;// = ComplexProduct(z, z);
   for (i = 0; i < maxiters; i++) {
     zSquared = ComplexProduct(z, z);
     freeComplexNumber(z);
     z = ComplexSum(zSquared, point);
     freeComplexNumber(zSquared);
     output = ComplexAbs(z);
     numIters += 1;
     if (output >= threshold) {
       freeComplexNumber(z);
       return numIters;
     }
   }
   freeComplexNumber(z);
   return 0;
}

void test_mandelbrot_iterations() {
  ComplexNumber* temp;
  u_int64_t numIters;

  temp = newComplexNumber(-1, 1);
  numIters = MandelbrotIterations(1536, temp, 2);
  printf("%lu ", numIters);
  free(temp);

  temp = newComplexNumber(1, 0);
  numIters = MandelbrotIterations(1536, temp, 2);
  printf("%lu ", numIters);
  free(temp);

  temp = newComplexNumber(0, 0);
  numIters = MandelbrotIterations(1536, temp, 2);
  printf("%lu ", numIters);
  free(temp);

  temp = newComplexNumber(-0.5, 0);
  numIters = MandelbrotIterations(1536, temp, 2);
  printf("%lu ", numIters);
  free(temp);

  temp = newComplexNumber(-1, 0);
  numIters = MandelbrotIterations(1536, temp, 2);
  printf("%lu ", numIters);
  free(temp);
}

//Z^2 + C
/*
This function calculates the Mandelbrot plot and stores the result in output.
The number of pixels in the image is resolution * 2 + 1 in one row/column. It's a square image.
Scale is the the distance between center and the top pixel in one dimension.
*/
void Mandelbrot(double threshold, u_int64_t max_iterations, ComplexNumber* center, double scale, u_int64_t resolution, u_int64_t * output){
    //YOUR CODE HERE
    double currReal, currImaginary;
    ComplexNumber* tempCN;

    if (resolution == 0) {
      currReal = Re(center), currImaginary = Im(center);
      tempCN = newComplexNumber(currReal, currImaginary);

      if (tempCN == NULL) {
        printf("New complex number was not allocated");
        return;
      }

      output[0] = MandelbrotIterations(max_iterations, tempCN, threshold);
      freeComplexNumber(tempCN);
    } else {
      int size = 2 * resolution + 1, sizesquared = size * size;
      int i, reScalar, imScalar;

      for (i = 0; i < sizesquared; i++) {
        reScalar = i % size - resolution;
        imScalar = resolution - i/size;

        currReal = Re(center) + (scale / resolution) * reScalar;
        currImaginary = Im(center) + (scale / resolution) * imScalar;
        tempCN = newComplexNumber(currReal, currImaginary);

        if (tempCN == NULL) {
      		printf("New complex number was not allocated");
      		return;
      	}

        output[i] = MandelbrotIterations(max_iterations, tempCN, threshold);
        freeComplexNumber(tempCN);
      }
    }
}

/*
double stepSize = scale / resolution;
double currReal = Re(center) - scale, currImaginary = Im(center) + scale; //Start at top left.
ComplexNumber* tempCN = newComplexNumber(currReal, currImaginary);
size_t rows, cols, outputIndex = 0, i = 0, numPixels = resolution * 2 + 1;

for (rows = 0; rows < numPixels; rows++) {
  for (cols = 0; cols < numPixels; cols++) {
    output[outputIndex++] = MandelbrotIterations(max_iterations, tempCN, threshold);

    //Output checking
    //printf("%u #%u r %f i %f \n", output[outputIndex-1], outputIndex-1, Re(tempCN), Im(tempCN));

    //Generating coordinates for the NEXT iteration
    if (cols != numPixels - 1) {
      currReal += stepSize;
    } else {
      currReal = Re(center) - scale; //reset x
      currImaginary -= stepSize; //move y down
    }


    freeComplexNumber(tempCN);
    tempCN = newComplexNumber(currReal, currImaginary);
  }
}
freeComplexNumber(tempCN);
*/
