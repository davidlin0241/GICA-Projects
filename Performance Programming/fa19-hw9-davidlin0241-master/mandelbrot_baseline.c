// Include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#include <x86intrin.h>
#endif

// Include OpenMP
#include <omp.h>

#include "mandelbrot.h"
#include "parameters.h"

/*****************************************
 * DO NOT MAKE ANY CHANGES TO THIS FILE! *
 *****************************************/

uint32_t iterations(struct parameters params, double complex point) {
    double complex z = 0;
    for (int i = 1; i <= params.maxiters; i++) {
        z = z * z + point;
        if (creal(z) * creal(z) + cimag(z) * cimag(z) >= params.threshold * params.threshold) {
            return i;
        }
    }
    return 0;
}

void mandelbrot(struct parameters params, double scale, int32_t *num_pixels_in_set) {
    uint32_t num_zero_pixels = 0;
    for (int i = params.resolution; i >= -params.resolution; i--) {
        for (int j = -params.resolution; j <= params.resolution; j++) {
            double complex point = (params.center +
                    j * scale / params.resolution +
                    i * scale / params.resolution * I);
            if (iterations(params, point) == 0) {
                num_zero_pixels++;
            }
        }
    }
    *num_pixels_in_set = num_zero_pixels;
}
