#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>
#include "base_func.h"



void openmp(double* m, double* x, double* y, double* z, double** r, double* Fx, double* Fy, double* Fz, int N, int iteracije)
{


}



int main(int argc, char *argv[]) 
{

	int N = atoi(argv[1]); // stevilo delcev
    int iteracije = atoi(argv[2]);

    double* m =  _init(N, 1.0, 10.0); //mase
    double* x =  _init(N, 0.0, 10.0); //x-koordinate
    double* y =  _init(N, 0.0, 10.0); //y-koordinate
    double* z =  _init(N, 0.0, 10.0); //z-koordinate
    double* vx = _init(N, 0.0, 10.0); //hitrost v x-koordinati
    double* vy = _init(N, 0.0, 10.0); //hitrost v y-koordinati
    double* vz = _init(N, 0.0, 10.0); //hitrost v z-koordinati

    double** r = r_init(N);           //inicializiramo prazen array
    
    double* Fx = (double*)malloc(sizeof(double) * N); // inicializiramo sile
    double* Fy = (double*)malloc(sizeof(double) * N);
    double* Fz = (double*)malloc(sizeof(double) * N);

    r  = calculate_rij(N, x, y, z, r);    //poracunamo vrednosti glede na hitrosti
    Fx = calculate_force(N, m, x, r, Fx); //poracunamo sile v x-koordinati
    Fy = calculate_force(N, m, y, r, Fy); //poracunamo sile v y-koordinati
    Fz = calculate_force(N, m, z, r, Fz); //poracunamo sile v z-koordinati

	//OpenMP solution
	//double start_recursive = omp_get_wtime();
	openmp(m, x, y, z, r, Fx, Fy, Fz, N, iteracije);
	//double end_recursive = omp_get_wtime();
    
    //print results
	//printf("Time openmpi: %f s\n", end_recursive-start_recursive);
	return 0;
}
