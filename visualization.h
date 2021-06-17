#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <math.h>  

#define MIN_MASS 100000000000.0

void print_positions(double* m, double* x, double* y, double* z, int N, int iter)
{
    int i;

    for (i = 0; i < N; i++)
        printf("%d,%.4f,%.2f,%.2f,%.2f,%d\n", i, m[i]/MIN_MASS*100, x[i], y[i], -z[i], iter);
    
}
