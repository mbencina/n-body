#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "/usr/include/openmpi-x86_64/mpi.h"

#define DEFAULT_N 8
#define DEFAULT_ITERS 10
#define GRAV_CONST 0.00000000006673
#define EPS 0.00000000000001

typedef struct {
   double px, py, pz;
} Position;

typedef struct {
   double vx, vy, vz;
} Velocity;

typedef struct {
   double fx, fy, fz;
} Force;

double* mass;         // mase za vsa telesa
Position* position;   // pozicija vseh teles
Velocity* n_velocity; // hitrost vseh teles
Velocity* velocity;   // hitrost teles za 1 proces
Force* force;         // sile za 1 proces

// podporne funkcije
double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

double calculate_distance(Position a, Position b)
{
    return sqrt(pow((a.px - b.px), 2.0) + pow((a.py - b.py), 2.0) + pow((a.pz - b.pz), 2.0) + EPS);
}

void print_iteration(Position* pos, Velocity* v, int N, int iter)
{
    int i;
    printf("ITERATION %d\n", iter);

    printf("x:  %.2f", pos[0].px);
    for (i = 1; i < N; i++)
        printf(", %.2f", pos[i].px);
    
    printf("\ny:  %.2f", pos[0].py);
    for (i = 1; i < N; i++)
        printf(", %.2f", pos[i].py);
    
    printf("\nz:  %.2f", pos[0].pz);
    for (i = 1; i < N; i++)
        printf(", %.2f", pos[i].pz);

    printf("\nvx: %.2f", v[0].vx);
    for (i = 1; i < N; i++)
        printf(", %.2f", v[i].vx);
    
    printf("\nvy: %.2f", v[0].vy);
    for (i = 1; i < N; i++)
        printf(", %.2f", v[i].vy);
    
    printf("\nvz: %.2f", v[0].vz);
    for (i = 1; i < N; i++)
        printf(", %.2f", v[i].vz);   
    
    printf("\n");
}

int main(int argc, char* argv[])
{
	int part_ix;				// zacetni ix v global arrayu za nek proces
	int N;          			// stevilo delcev
	int n_iters;   	 			// stevilo iteracij
	double max_x;				// maksimalne začetne koordinate x
	double max_y;				// maksimalne začetne koordinate y
	double max_z;				// maksimalne začetne koordinate z
	double min_mass;			// minimalna masa
	double max_mass;			// maksimalna masa
	double max_velocity;		// makismalna hitrost
	double innittime, totaltime;// merjenje časa
	MPI_Datatype MPI_POSITION;	// custom datatype za pozicijo
	MPI_Datatype MPI_VELOCITY;	// custom datatype za hitrost

	int procs, myid;
	MPI_Init(&argc, &argv);					// initiailzation
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);	// process ID
	MPI_Comm_size(MPI_COMM_WORLD, &procs);	// number of processes
	
	// random seed
	srand(1573949136);

    // argumenti: st. teles in st. iter
    N = DEFAULT_N;
    n_iters = DEFAULT_ITERS;
    if (argc >= 3) {
        N = atoi(argv[1]);
        n_iters = atoi(argv[2]);
    }

	// poskrbimo da: N mod p = 0
	while (N % procs)
		N++;

	// dolocimo part size in part index
	int part_size = N / procs;
   	part_ix = myid * part_size;

    // definicija posebnih MPI tipov
	MPI_Type_contiguous(3, MPI_DOUBLE, &MPI_POSITION);
	MPI_Type_commit(&MPI_POSITION);
	MPI_Type_contiguous(3, MPI_DOUBLE, &MPI_VELOCITY);
	MPI_Type_commit(&MPI_VELOCITY);

	// definicija zacetnih mej
    max_x = 10.0;
	max_y = 10.0;
	max_z = 10.0;
	max_velocity = 10.0;
	min_mass = 10e11;
	max_mass = 10e12;

    // alokacija spomina za strukture
	// globalne
	mass = (double *) malloc(N * sizeof(double));
	position = (Position *) malloc(N * sizeof(Position));
	n_velocity = (Velocity *) malloc(N * sizeof(Velocity));
	// lokalne
	velocity = (Velocity *) malloc(part_size * sizeof(Velocity));
	force = (Force *) malloc(part_size * sizeof(Force));

	// data init
	if (myid == 0) {
		for (int i = 0; i < N; i++) {
			mass[i] = fRand(min_mass, max_mass);
			position[i].px = fRand(-max_x, max_x);
			position[i].py = fRand(-max_y, max_y);
			position[i].pz = fRand(-max_z, max_z);
			n_velocity[i].vx = fRand(-max_velocity, max_velocity);
			n_velocity[i].vy = fRand(-max_velocity, max_velocity);
			n_velocity[i].vz = fRand(-max_velocity, max_velocity);
		}
	}

	// zečetek štetja časa
	innittime = MPI_Wtime();

    // broadcast position & mass to all; scatter initial velocity to local arrays
	MPI_Bcast(mass, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(position, N, MPI_POSITION, 0, MPI_COMM_WORLD);
	MPI_Scatter(n_velocity, part_size, MPI_VELOCITY, velocity, part_size, MPI_VELOCITY, 0, MPI_COMM_WORLD);

    // iteriranje cez cas
	for (int iter = 0; iter < n_iters; iter++) {
		// poracunamo sile
		for (int i = 0; i < part_size; i++) {
			force[i].fx = 0.0;
			force[i].fy = 0.0;
			force[i].fz = 0.0;

			for (int j = 0; j < N; j++) {
				// we skip computing for the same bodies
				if (j == i + part_ix) continue;

				double d = calculate_distance(position[i + part_ix], position[j]);

				force[i].fx += mass[j] * ((position[i + part_ix].px - position[j].px) / pow(d, 3.0));
				force[i].fy += mass[j] * ((position[i + part_ix].py - position[j].py) / pow(d, 3.0));
				force[i].fz += mass[j] * ((position[i + part_ix].pz - position[j].pz) / pow(d, 3.0));
			}
			force[i].fx *= -1 * GRAV_CONST * mass[i + part_ix];
            force[i].fy *= -1 * GRAV_CONST * mass[i + part_ix];
			force[i].fz *= -1 * GRAV_CONST * mass[i + part_ix];
		}

		// poracunamo nove hitrosti
		for (int i = 0; i < part_size; i++) {
			velocity[i].vx += (force[i].fx / mass[i + part_ix]);
			velocity[i].vy += (force[i].fy / mass[i + part_ix]);
			velocity[i].vz += (force[i].fz / mass[i + part_ix]);
		}
		
		// poracunamo nove pozicije
		for (int i = 0; i < part_size; i++) {
			position[i + part_ix].px += velocity[i].vx + 0.5 * (force[i].fx / mass[i + part_ix]);
			position[i + part_ix].py += velocity[i].vy + 0.5 * (force[i].fy / mass[i + part_ix]);
			position[i + part_ix].pz += velocity[i].vz + 0.5 * (force[i].fz / mass[i + part_ix]);
		}

		// posljemo nove positione vsem
		MPI_Allgather(position + part_ix, part_size, MPI_POSITION, 
						position, part_size, MPI_POSITION, MPI_COMM_WORLD);
		
		/* // posljemo hitrosti za izpis
		MPI_Gather(velocity, part_size, MPI_VELOCITY, 
						n_velocity, part_size, MPI_VELOCITY, 0, MPI_COMM_WORLD);

		// izpis
		if (myid == 0)
			print_iteration(position, n_velocity, N, iter); */
   }

	// izpis časa
	totaltime = MPI_Wtime() - innittime;
	if (myid == 0) {
		printf("Time MPI: %f s\n", totaltime);
	}

	// free mem
	free(mass);
	free(position);
	free(n_velocity);
	free(velocity);

	MPI_Finalize();

	return 0;
}
