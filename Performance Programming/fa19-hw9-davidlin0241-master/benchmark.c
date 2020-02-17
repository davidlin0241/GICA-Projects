#include <complex.h>
#include <getopt.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "mandelbrot.h"
#include "parameters.h"

/*****************************************
 * DO NOT MAKE ANY CHANGES TO THIS FILE! *
 *****************************************/

#define NUM_ARGS 8

// Prints the usage of this program and exits with code 1, signifying an error.
void usage(void) {
    printf("Usage:\n");
    printf("\tbenchmark -t <threshold> -m <maxiterations> -n <numframes> -r <resolution>\n");
    int padded_length = strlen(
        "benchmark -R <creal> -I <cimag> -i <initialscale> -f <finalscale>");
    printf("\t%*s",
            padded_length + 1,
            "-R <creal> -I <cimag> -i <initialscale> -f <finalscale>\n");
    printf("Options:\n");
    printf("\t-t, threshold\n");
    printf("\t\tThe threshold used to determine what's in the Mandelbrot set.\n");
    printf("\t-m, maxiters\n");
    printf("\t\tThe maximum number of iterations to determine if a point is in the Mandelbrot set.\n");
    printf("\t-n, numframes\n");
    printf("\t\tThe number of frames to generate.\n");
    printf("\t-r, resolution\n");
    printf("\t\tThe width and height (in pixel) of the image.\n");
    printf("\t-R, center_real\n");
    printf("\t\tThe real component of the center of the image.\n");
    printf("\t-I, center_imag\n");
    printf("\t\tThe imaginary component of the center of the image.\n");
    printf("\t-i, initialscale\n");
    printf("\t\tThe starting width and height (in the complex plane) of the image.\n");
    printf("\t-f, finalscale\n");
    printf("\t\tThe ending width and height (in the complex plane) of the image.\n");
    exit(1);
}

// Uses the getopt_long method to get command line arguments for this program
// and stores the results in the global parameters struct.
void get_params(int argc, char *argv[], struct parameters *params) {
    static struct option opts[] = {
        { "threshold",      required_argument, NULL, 't'    },
        { "maxiters",       required_argument, NULL, 'm'    },
        { "numframes",      required_argument, NULL, 'n'    },
        { "resolution",     required_argument, NULL, 'r'    },
        { "center_real",    required_argument, NULL, 'R'    },
        { "center_imag",    required_argument, NULL, 'I'    },
        { "initialscale",   required_argument, NULL, 'i'    },
        { "finalscale",     required_argument, NULL, 'f'    },
        { NULL,             0,                 NULL, 0      }
    };

    double center_real;
    double center_imag;

    int opt;
    int required_opt_count = 0;
    while ((opt = getopt_long(argc, argv, "t:m:n:r:R:I:i:f:", opts, NULL)) != -1) {
        switch (opt) {
        case 't':
        if (sscanf(optarg, "%lf", &params->threshold) != 1) {
            printf("Invalid value for threshold!\n");
            usage();
        }
        required_opt_count++;
        break;
        case 'm':
        if (sscanf(optarg, "%d", &params->maxiters) != 1) {
            printf("Invalid value for maxiters!\n");
            usage();
        }
        required_opt_count++;
        break;
        case 'n':
        if (sscanf(optarg, "%d", &params->numframes) != 1) {
            printf("Invalid value for numframes!\n");
            usage();
        }
        required_opt_count++;
        break;
        case 'r':
        if (sscanf(optarg, "%d", &params->resolution) != 1) {
            printf("Invalid value for resolution!\n");
            usage();
        }
        required_opt_count++;
        break;
        case 'R':
        if (sscanf(optarg, "%lf", &center_real) != 1) {
            printf("Invalid value for center_real!\n");
            usage();
        }
        required_opt_count++;
        break;
        case 'I':
        if (sscanf(optarg, "%lf", &center_imag) != 1) {
            printf("Invalid value for center_imag!\n");
            usage();
        }
        required_opt_count++;
        break;
        case 'i':
        if (sscanf(optarg, "%lf", &params->initialscale) != 1) {
            printf("Invalid value for initialscale!\n");
            usage();
        }
        required_opt_count++;
        break;
        case 'f':
        if (sscanf(optarg, "%lf", &params->finalscale) != 1) {
            printf("Invalid value for finalscale!\n");
            usage();
        }
        required_opt_count++;
        break;
        default:
        usage();
        }
    }

    if (required_opt_count != NUM_ARGS) {
        usage();
    }

    params->center = center_real + center_imag * I;
}

double getscale(struct parameters params, int framenum) {
    double logratio = log(params.finalscale) - log(params.initialscale);
    double scale = exp(log(params.initialscale) +
            (framenum * logratio) / (params.numframes - 1));
    return scale;
}

int main(int argc, char *argv[]) {
    struct parameters params;           // Parameters for constructing the mandelbrot set.
    get_params(argc, argv, &params);

    struct timeval time;
    gettimeofday(&time, NULL);
    uint64_t start = 1000000L * time.tv_sec + time.tv_usec;

    int32_t *num_pixels_in_set_for_frame = malloc(sizeof(int32_t) * params.numframes);
    for (int i = 0; i < params.numframes; i++) {
        mandelbrot(params, getscale(params, i), num_pixels_in_set_for_frame + i);
    }

    gettimeofday(&time, NULL);
    uint64_t end = 1000000L * time.tv_sec + time.tv_usec;
    printf("%" PRIu64 " microseconds\n", end - start);

    int framesize = (2 * params.resolution + 1) * (2 * params.resolution + 1);
    int max_pixel_num_width = snprintf(NULL, 0, "%d", framesize);
    int max_frame_num_width = snprintf(NULL, 0, "%d", params.numframes);
    for (int i = 0; i < params.numframes; i++) {
        printf("Frame %*d, Scale %f: %*d/%d pixels in set\n",
                max_frame_num_width,
                i,
                getscale(params, i),
                max_pixel_num_width,
                num_pixels_in_set_for_frame[i],
                framesize);
    }
    return 0;
}