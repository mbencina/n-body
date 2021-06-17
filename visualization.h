#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <math.h>  


void print_positions(double* m, double* x, double* y, double* z, int N, int iter)
{
    int i;

    for (i = 0; i < N; i++)
        printf("%.2f,%.2f,%.2f,%.2f,%d\n", m[i], x[i], y[i], z[i], iter);
    
}
