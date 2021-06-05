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


double calculate_rij(double* x, double* y, double*z, int i, int j)
{
    double epsilon = 0.00000000000001;
    
	return sqrt(pow((x[i] - x[j]),2.0) + pow((y[i] - y[j]),2.0) + pow((z[i] - z[j]),2.0) + epsilon);
}


double calculate_force(double* m, double* l, double** r, int i, int j)
{
	return m[j] * ((l[i] - l[j])/pow(r[i][j],3.0));
}


double update_velocity(double* v, double* F, double* m, int i)
{
	return v[i] + F[i]/m[i];
}

double update_position(double* position, double* v, double* F, double* m, int i)
{
	return position[i] + v[i] + 0.5 * (F[i] / m[i]);
}