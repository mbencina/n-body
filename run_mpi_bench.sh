#!/bin/bash
#SBATCH --ntasks=4
#SBATCH --nodes=2
#SBATCH --output=out/mpi_slurm_4_2.log
#SBATCH --time=05:00:00
#SBATCH --reservation=fri

mpicc -lm -o mpi_impl mpi_impl.c

for (( i=5000; i<=30000; i+=5000 ));
do
  # arguments: N and n_iterations
  srun --mpi=pmix mpi_impl $i 10 > "out/mpi_bench_p4_n2_$i.log"
done
