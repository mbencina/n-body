#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>
#include "base_func.h"
#include "visualization.h"

// distance from center to edge
#define BOX_LIMIT 500
#define MIN_MASS 1000000000000.0

double if_out_of_bounds_bounce(double i, double* v, int indeks)
{
    if (i >= 2 * BOX_LIMIT) 
    {
        i = 2 * BOX_LIMIT;
        v[indeks] = -1 * v[indeks];
    } else if (i <= 0) 
    {
        i = 0;
        v[indeks] = -1 * v[indeks];
    }
    return i;
}

void print_iteration(double* x, double* y, double* z, double* vx, double* vy, double* vz, int N, int iter)
{
    int i;
    printf("ITERATION %d\n", iter);

    printf("x:  %.2f", x[0]);
    for (i = 1; i < N; i++)
        printf(", %.2f", x[i]);
    
    printf("\ny:  %.2f", y[0]);
    for (i = 1; i < N; i++)
        printf(", %.2f", y[i]);
    
    printf("\nz:  %.2f", z[0]);
    for (i = 1; i < N; i++)
        printf(", %.2f", z[i]);

    printf("\nvx: %.2f", vx[0]);
    for (i = 1; i < N; i++)
        printf(", %.2f", vx[i]);
    
    printf("\nvy: %.2f", vy[0]);
    for (i = 1; i < N; i++)
        printf(", %.2f", vy[i]);
    
    printf("\nvz: %.2f", vz[0]);
    for (i = 1; i < N; i++)
        printf(", %.2f", vz[i]);   
    
    printf("\n");
}

void openmp(double* m, double* x, double* y, double* z, double* vx, double* vy, double* vz, double** r, double* Fx, double* Fy, double* Fz, int N, int iteracije)
{
    for(int iter = 0; iter<iteracije; iter++)
    {
        //poracunamo R (sprememba pozicije)
        int i,j;
        #pragma omp parallel for collapse(2) shared(x,y,z) schedule(static)
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                r[i][j]  = calculate_rij(x, y, z, i, j);

        
        //poracunamo sile
        for (i = 0; i < N; i++)
        {
            double sum_x = 0.0;
            double sum_y = 0.0;
            double sum_z = 0.0;
            #pragma omp parallel for reduction(+:sum_x,sum_y,sum_z) shared(m,x,y,z,r) schedule(static)
            for (j = 0; j < N; j++) //mogoce preverimo ce je hitrej, da je vsaka v svoji for zanki: zarad memori dostopov ...
            {
                sum_x += calculate_force(m, x, r, i, j);
                sum_y += calculate_force(m, y, r, i, j);
                sum_z += calculate_force(m, z, r, i, j); 
            }
            Fx[i] = -1 * GRAV_CONST * m[i] * sum_x;
            Fy[i] = -1 * GRAV_CONST * m[i] * sum_y;
            Fz[i] = -1 * GRAV_CONST * m[i] * sum_z;		
        }

        //poracunamo nove hitrosti
        #pragma omp parallel for shared(m, vx, vy, vz, Fx, Fy, Fz) schedule(static)
        for (i = 0; i < N; i++)
        {
            vx[i] = update_velocity(vx, Fx, m, i);
            vy[i] = update_velocity(vy, Fy, m, i);
            vz[i] = update_velocity(vz, Fz, m, i);
        }

        //poracunamo nove pozicije
        #pragma omp parallel for shared(m, vx, vy, vz, Fx, Fy, Fz, x, y, z) schedule(static)
        for (i = 0; i < N; i++)
        {
            double xi = update_position(x, vx, Fx, m, i);
            double yi = update_position(y, vy, Fy, m, i);
            double zi = update_position(z, vz, Fz, m, i);
            x[i] = if_out_of_bounds_bounce(xi, vx, i);
            y[i] = if_out_of_bounds_bounce(yi, vy, i);
            z[i] = if_out_of_bounds_bounce(zi, vz, i);
        }
        

        //print_iteration(x, y, z, vx, vy, vz, N, iter);
        print_positions(m, x, y, z, N, iter);

	}
}



int main(int argc, char *argv[]) 
{
    //---------------------------------------------------------------------------
    // START OF INITIALIZATION
    //---------------------------------------------------------------------------
	int N = atoi(argv[1]); // stevilo delcev
    int iteracije = atoi(argv[2]);
    srand(17);

    double* m =  _init(N, MIN_MASS, MIN_MASS * 2); //mase
    double* x =  _init(N, 100.0, 900.0); //x-koordinate
    double* y =  _init(N, 100.0, 900.0); //y-koordinate
    double* z =  _init(N, 100.0, 900.0); //z-koordinate
    double* vx = _init(N, -1.0, 1.0); //hitrost v x-koordinati
    double* vy = _init(N, -1.0, 1.0); //hitrost v y-koordinati
    double* vz = _init(N, -1.0, 1.0); //hitrost v z-koordinati

    double** r = r_init(N);           //inicializiramo prazen array
    
    double* Fx = (double*)malloc(sizeof(double) * N); // inicializiramo sile
    double* Fy = (double*)malloc(sizeof(double) * N);
    double* Fz = (double*)malloc(sizeof(double) * N);

    //---------------------------------------------------------------------------
    // END OF INITIALIZATION
    //---------------------------------------------------------------------------

    printf("ix,m,x,y,z,iter\n");
	// OpenMP solution
	double start_openmp = omp_get_wtime();
	openmp(m, x, y, z, vx, vy, vz, r, Fx, Fy, Fz, N, iteracije);
	double end_openmp = omp_get_wtime();
    
    //print results
	//printf("Time openmpi: %f s\n", end_openmp-start_openmp);
	return 0;
}
