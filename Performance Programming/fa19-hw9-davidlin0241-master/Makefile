CFLAGS=-Wall -Wno-unused-result -march=haswell -std=c99 -fopenmp -O3
DEBFLAGS=-g -Wall -Wno-unused-result -march=haswell -std=c99 -fopenmp -Og

all : benchmark benchmark_baseline

benchmark : benchmark.o mandelbrot.o
	gcc $(CFLAGS) -o benchmark benchmark.o mandelbrot.o -lm

benchmark_baseline : benchmark.o mandelbrot_baseline.o
	gcc $(CFLAGS) -o benchmark_baseline benchmark.o mandelbrot_baseline.o -lm

debug : clean benchmark.c mandelbrot.c parameters.h mandelbrot.h
	gcc $(DEBFLAGS) -c mandelbrot.c
	gcc $(DEBFLAGS) -c benchmark.c
	gcc $(DEBFLAGS) -o benchmark benchmark.o mandelbrot.o -lm

benchmark.o : benchmark.c parameters.h
	gcc $(CFLAGS) -c benchmark.c

mandelbrot.o : mandelbrot.c mandelbrot.h parameters.h
	gcc $(CFLAGS) -c mandelbrot.c

mandelbrot_baseline.o : mandelbrot_baseline.c mandelbrot.h parameters.h
	gcc $(CFLAGS) -c mandelbrot_baseline.c

clean:
	rm -f *.o
	rm -f benchmark
	rm -f benchmark_baseline
	rm -f out/*.ppm