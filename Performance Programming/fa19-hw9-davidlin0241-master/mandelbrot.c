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

//The following functions are put into Mandelbrot directly for more speed.

/*
//For tail case use only.
uint32_t iterations(struct parameters params, double complex point) {
    double complex z = 0;
    double re, im;
    for (int j = 1; j <= params.maxiters; j++) {
      z = z * z + point;
      re = creal(z);
      im = cimag(z);
      if (re * re + im * im >= params.threshold * params.threshold) {
        return 0;
      }
    }
    return 1;
}

//SIMD iterations
uint32_t Siterations(struct parameters params, __m256d __REv, __m256d __IMv) {
    __m256d __zRE = _mm256_setzero_pd(), __zIM = _mm256_setzero_pd(),
     __double = _mm256_set1_pd(2), __orgZRE, __orgZIM,
     __maskThreshold = _mm256_set1_pd(params.threshold * params.threshold),
     __temp = _mm256_setzero_pd(), __temp2 = _mm256_setzero_pd();
     //uint32_t numZeros = 0;

    //z = z * z + p
    for (int j = 1; j <= params.maxiters; j++) {
      __orgZRE = __zRE;
      __orgZIM = __zIM;

      __zRE = _mm256_mul_pd(__zRE, __zRE); //x^2
      __zIM = _mm256_mul_pd(__zIM, __zIM); //y^2
      __zRE = _mm256_sub_pd(__zRE, __zIM); //x^2 - y^2 real part

      __zIM = _mm256_mul_pd(__orgZRE, __orgZIM); //xyi
      __zIM = _mm256_mul_pd(__double, __zIM); //2xyi imaginary part

      __zRE = _mm256_add_pd(__zRE, __REv); //x^2 - y^2 + REv
      __zIM = _mm256_add_pd(__zIM, __IMv); //(2xy + IMv)i

      __temp =_mm256_mul_pd(__zRE, __zRE); //square of real
      __temp2 = _mm256_mul_pd(__zIM, __zIM); //square of imaginary

      __temp = _mm256_add_pd(__temp, __temp2); //magnitude
      __temp = _mm256_cmp_pd (__temp, __maskThreshold, 1); //less than with threshold mask

      //__temp[0] == 0 && __temp[1] == 0 && __temp[2] == 0 && __temp[3] == 0
      if (__temp[0] + __temp[1] + __temp[2] + __temp[3] == 0) {
        return 0;
      }
    }

    //No tail case necessary here. Handled in Mandelbrot.

    /*
    for (int j = 0; j < 4; j++) {
      if (__temp[j] != 0) {
        numZeros++;
      }
    }
    return numZeros;
    //

    return (__temp[0] != 0) + (__temp[1] != 0) + (__temp[2] != 0) + (__temp[3] != 0);
}
*/

void mandelbrot(struct parameters params, double scale, int32_t *num_pixels_in_set) {
    int32_t num_zero_pixels = 0;
    int i, size = 2 * params.resolution + 1, sizesquared = size * size;
    double complex center = params.center;
    double scaleDivRes = scale / params.resolution;
    __m256d _REv, _IMv;

    int reScalar, imScalar;
    double complex point, z = 0;
    double re, im;

    omp_set_num_threads(8);
    #pragma omp parallel
    {
      #pragma omp for reduction(+: num_zero_pixels)
      for (i = 0; i < sizesquared / 4 * 4; i += 4) {
        _REv = _mm256_set_pd( creal(center) + scaleDivRes * (i % size - params.resolution),
    		  creal(center) + scaleDivRes * ( (i+1) % size - params.resolution ),
    		    creal(center) + scaleDivRes * ( (i+2) % size - params.resolution ),
    		      creal(center) + scaleDivRes * ( (i+3) % size - params.resolution));

    	  _IMv = _mm256_set_pd( cimag(center) + scaleDivRes * (params.resolution - i/size),
    		  cimag(center) + scaleDivRes * ( params.resolution - (i+1)/size ),
    		    cimag(center) + scaleDivRes * ( params.resolution - (i+2)/size ),
    		      cimag(center) + scaleDivRes * ( params.resolution - (i+3)/size ));

        //Siterations setup
        __m256d __zRE = _mm256_setzero_pd(), __zIM = _mm256_setzero_pd(),
         __double = _mm256_set1_pd(2), __orgZRE, __orgZIM,
         __maskThreshold = _mm256_set1_pd(params.threshold * params.threshold),
         __temp = _mm256_setzero_pd(), __temp2 = _mm256_setzero_pd();
         //uint32_t numZeros = 0;

        //z = z * z + p
        for (int j = 1; j <= params.maxiters; j++) {
          __orgZRE = __zRE;
          __orgZIM = __zIM;

          __zRE = _mm256_mul_pd(__zRE, __zRE); //x^2
          __zIM = _mm256_mul_pd(__zIM, __zIM); //y^2
          __zRE = _mm256_sub_pd(__zRE, __zIM); //x^2 - y^2 real part

          __zIM = _mm256_mul_pd(__orgZRE, __orgZIM); //xyi
          __zIM = _mm256_mul_pd(__double, __zIM); //2xyi imaginary part

          __zRE = _mm256_add_pd(__zRE, _REv); //x^2 - y^2 + REv
          __zIM = _mm256_add_pd(__zIM, _IMv); //(2xy + IMv)i

          __temp =_mm256_mul_pd(__zRE, __zRE); //square of real
          __temp2 = _mm256_mul_pd(__zIM, __zIM); //square of imaginary


          if (j % 8 == 0) {
            __temp = _mm256_add_pd(__temp, __temp2); //magnitude
            __temp = _mm256_cmp_pd (__temp, __maskThreshold, 1); //less than with threshold mask
            if (__temp[0] + __temp[1] + __temp[2] + __temp[3] == 0)
              break;
          }

          //At this point when j == params.maxiters and the threshold is not exceeded, add 1
          if (j == params.maxiters)
            num_zero_pixels += (__temp[0] != 0) + (__temp[1] != 0) + (__temp[2] != 0) + (__temp[3] != 0);
        }
      }

      //Tail case with iterations
      #pragma omp parallel for reduction(+: num_zero_pixels)
      for (i = sizesquared / 4 * 4; i < sizesquared; i++) {
    		reScalar = i % size - params.resolution;
    		imScalar = params.resolution - i / size;

        point = (params.center +
                  reScalar * scaleDivRes +
                  imScalar * scaleDivRes * I);

        for (int j = 1; j <= params.maxiters; j++) {
          z = z * z + point;
          re = creal(z);
          im = cimag(z);
          if (re * re + im * im >= params.threshold * params.threshold)
            break;

          //At this point when j == params.maxiters and the threshold is not exceeded, add 1
          if (j == params.maxiters) {
            num_zero_pixels++;
          }
        }
  	  }

      *num_pixels_in_set = num_zero_pixels;
    }
}
