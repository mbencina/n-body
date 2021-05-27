#!/bin/sh
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=32
#SBATCH --constraint=AMD
#SBATCH --job-name=niz-poslov
#SBATCH --time=00:02:00
#SBATCH --output=benchmark.log
#SBATCH --reservation=fri

export OMP_PLACES=cores
export OMP_PROC_BIND=TRUE
export OMP_NUM_THREADS=1

srun ./test 100 10
