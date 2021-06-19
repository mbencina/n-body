# The n-body problem using OpenMP and MPI 

By [van123helsing](https://github.com/van123helsing) and me.

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
You can change the number of bodies and iterations in the file `run_mpi.sh`.

## Visualization of data
### Visualize
You can visualize the provided generated file by running the [visual.pde](/visual/visual.pde) file in the [Processing](https://processing.org/) editor.  
To use your own generated file (see [next section](#obtaining-data-for-visualization)) change the `filename` variable to your file name (without the `.csv` suffix):
```
filename = "your_file_name";
```
A demo visualization can be found [here](/visual/demo/demo_visualization.mp4).  

### Obtaining data for visualization  
You can set parameters (random seed, initial speed span, initial coordinate span (within 0-1000), minimal mass etc.) in the main function of `visualization.cpp`.  
You can also set the number of iterations and bodies in `run_visualize.sh`.
Compile as:
```
g++ -o visualization visualization.cpp -fopenmp -lm
```

Run as:
```
sbatch run_visualize.sh
```
Place the generated `.csv` file you wish to visualize in the `/data` folder.


## Results OpenMP
### 10 iterations
| N           | 1           | 4           | 8           | 16          | 32          |
| ----------- | ----------- | ----------- | ----------- | ----------- | ----------- |
| 5000	      | 93,98 	    | 26,5	      | 14,29	      | 8,09	      | 7,76        |
| 10000	      | 207,63	    | 105,66	    | 56	        | 30,5	      | 21,16       |
| 15000	      | 467,24	    | 237,72	    | 125,61	    | 66,34  	    | 41,87       |
| 20000	      | 783,65	    | 422,38	    | 222,89	    | 116,11	    | 67,77       |
| 25000	      | 971,26	    | 761,08	    | 347,4	      | 178,81	    | 102,22      |
| 30000	      | 1349,7	    | 949,7	      | 499,56	    | 258,66	    | 145,51      |
| 35000	      | 1740,36	    | 1292,89	    | 679,97	    | 350,49	    | 194,94      |
| 40000	      | 1986,85	    | 1540,21	    | 887,87	    | 455,89	    | 250,33      |
| 45000	      | 2264,48	    | 1881,09	    | 1123,57	    | 575,96	    | 317,26      |
| 50000	      | 2583,74	    | 2109,33	    | 1386,63	    | 708,3	      | 387,99      |

## Results MPI 
### 10 iterations
#### 1 node
| N           | 4           | 8           | 16          | 32          |
| ----------- | ----------- | ----------- | ----------- | ----------- |
| 5000	      | 27.95	      | 18.11	      | 9.52	      | 4.91        |
| 10000	      | 111.26	    | 72.38	        | 38.16	      | 19.19       |
| 15000	      | 251.10	    | 154.57	    | 78.47  	    | 39.80       |
| 20000	      | 479.11	    | 278.24	    | 152.67	    | 76.47       |
| 25000	      | 896.95	    | 435.55	      | 238.68	    | 108.93      |
| 30000	      | 1199.77	      | 626.43	    | 311.35	    | 155.71      |

#### 2 nodes
| N           | 4           | 8           | 16          | 32          |
| ----------- | ----------- | ----------- | ----------- | ----------- |
| 5000	      | 33.87	      | 18.95	      | 9.60	      | 4.40        |
| 10000	      | 191.75	    | 75.57	        | 37.89	      | 17.39       |
| 15000	      | 309.12	    | 154.56	    | 79.87  	    | 38.43       |
| 20000	      | 546.29	    | 275.54	    | 139.33    | 76.10       |
| 25000	      | 827.74	    | 429.42	      | 216.42	    | 118.95      |
| 30000	      | 1204.73	      | 590.74	    | 311.12	    | 155.70      |
