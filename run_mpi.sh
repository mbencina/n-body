#!/bin/bash
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH --output=out/mpi_bench.log
#SBATCH --reservation=fri

# TODO try more tasks and 2 nodes
mpicc -o mpi_impl mpi_impl.c
srun --mpi=pmix mpi_impl
