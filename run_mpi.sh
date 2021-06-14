#!/bin/bash
#SBATCH --ntasks=4
#SBATCH --nodes=1
#SBATCH --output=out/mpi_slurm.log
#SBATCH --reservation=fri

mpicc -lm -o mpi_impl mpi_impl.c
# arguments: N and n_iterations
srun --mpi=pmix mpi_impl 25000 10 > out/mpi_bench_p4_n1_25000.log
