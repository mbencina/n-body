# n-body

## OpenMP
Compile as:
```
g++ -o projekt projekt.cpp -fopenmp -lm
```

Run as:
```
sbatch run.sh
```

## MPI
First run these two commands:
```
export OMPI_MCA_btl_openib_allow_ib=1
module load mpi
```

Run as:
```
sbatch run_mpi.sh
```
