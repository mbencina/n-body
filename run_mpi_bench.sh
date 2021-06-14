#!/bin/bash
#SBATCH --ntasks=32
#SBATCH --nodes=1
#SBATCH --output=out/mpi_slurm_32_1.log
#SBATCH --time=02:00:00
#SBATCH --reservation=fri

mpicc -lm -o mpi_impl mpi_impl.c

for (( i=15000; i<=30000; i+=5000 ));
do
  # arguments: N and n_iterations
  srun --mpi=pmix mpi_impl $i 10 > "out/mpi_bench_p32_n1_$i.log"
done
