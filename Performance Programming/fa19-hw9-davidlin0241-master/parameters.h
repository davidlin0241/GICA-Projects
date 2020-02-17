#ifndef PARAMETERS_H
#define PARAMETERS_H

/*****************************************
 * DO NOT MAKE ANY CHANGES TO THIS FILE! *
 *****************************************/

#include <complex.h>
#include <stdint.h>

struct parameters {
    double threshold;
    int maxiters;
    int numframes;
    int resolution;
    double complex center;
    double initialscale;
    double finalscale;
};

#endif