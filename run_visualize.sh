#!/bin/sh
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=32
#SBATCH --constraint=AMD
#SBATCH --job-name=niz-poslov
#SBATCH --time=00:05:00
#SBATCH --output=out/data.csv
#SBATCH --reservation=fri

export OMP_PLACES=cores
export OMP_PROC_BIND=TRUE
export OMP_NUM_THREADS=32

# N iters
srun ./visualization 8 1000
