#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <math.h>  

#define GRAV_CONST 0.00000000006673

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}


double * _init(int N, double od_, double do_)
{
    int k;

	double* m = (double*)malloc(sizeof(double) * N);

	for (k = 0; k < N; k++)
        m[k] = fRand(od_,do_);

	return m;
}

double** r_init(int N)
{
	int k, l;

	double* bd = (double*)malloc(sizeof(double) * N * N);
	double** b = (double**)malloc(sizeof(double*) * N);
	for (k = 0; k < N; k++)
		b[k] = &bd[k * N];

	return b;
}


double ** calculate_rij(int N, double* x, double* y, double*z, double** r)
{
    double epsilon = 0.00000000000001;

    int i,j;

	for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            r[i][j] = sqrt(pow((x[i] - x[j]),2.0) + pow((y[i] - y[j]),2.0) + pow((z[i] - z[j]),2.0) + epsilon);

	return r;
}


double * calculate_force(int N, double* m, double* l, double** r, double* F)
{
    int i,j;

	for (i = 0; i < N; i++)
    {
        double sum = 0.0;

        for (j = 0; j < N; j++)
            sum += m[j] * ((l[i] - l[j])/pow(r[i][j],3.0));
            
        F[i] = -1 * GRAV_CONST * m[i] * sum;
    }
	return F;
}