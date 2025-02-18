# Part 1: Hello World
helo_mpi.c is code where multiple processes print their rank and the total number of processes in the communicator.

It can be ran using mpirun -np <num_processes> bin/hello_mpi. The code was benchmarked for a number of different processes up to 10.

![MPI Program Benchmark](https://github.com/cameronDCU/HPQC/main/week3/mpi.png)
For the MPI program as the number of processes increases, the real time decreases, indicating that the workload is being distributed across multiple processes, leading to faster execution.

For the serial program the execution time remains relatively constant regardless of the input as there is no parallel processing.
![Serial Program Benchmark](https://github.com/cameronDCU/HPQC/main/week3/serial.png)
