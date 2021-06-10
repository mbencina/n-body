#include <stdlib.h>
#include <stdio.h>
#include "base_func.h"
#include "/usr/include/openmpi-x86_64/mpi.h"

#define DEFAULT_N 8
#define DEFAULT_ITERS 20

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);					// initiailzation
	
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);	// process ID
	MPI_Comm_size(MPI_COMM_WORLD, &procs);	// number of processes

    // TODO test
    int N = DEFAULT_N;
    int n_iters = DEFAULT_ITERS;
    if (argc >= 3) {
        N = atoi(argv[1]);          // stevilo delcev
        n_iters = atoi(argv[2]);    // stevilo iteracij
    }

    // TODO define special data types
    
    // TODO allocate memory for data structures (some need to be defiend outside - global)

	// initialize global board
	if (myid == 0)
	{
		srand(1573949136);
		// TODO init data
	}

	// divide work
	int part_size = N / procs;
    if (myid == 0)
        printf("%d %d %d\n", N, n_iters, part_size);

    // TODO MPI_Bcast, MPI_Bcast, MPI_Scatter

    // TODO loop

	// TODO free memory

	MPI_Finalize();

	return 0;
}
