# MPI Communication Test Report

## Part 1:
The following summarizes the results of compiling and running `comm_test_mpi.c` with different numbers of processes using `mpicc` and `mpirun`. The objective was to observe variations in execution order and performance.
The program was compiled using:
```sh
mpicc -o bin/comm_test_mpi comm_test_mpi.c

The program was executed using mpirun -np <num_procs> bin/comm_test_mpi.
There were slightly different variations in execution timing, but this was expected due to the nature of MPI communication and scheduling.

comm_test_mpi_new is a refactored version of `comm_test_mpi.c` by breaking the main function into separate functions for improved readability and modularity. The program demonstrates basic MPI communication between a root process and multiple non-root processes. ## Code Structure The refactored code consists of three main functions: - `init_mpi()` → Initializes MPI and retrieves rank and size. - `root_task()` → Handles message reception at rank 0. - `non_root_task()` → Sends messages from non-root processes.
It can be ran using mpirun -np <num_procs> bin/comm_test_mpi_new

Various versions of `comm_test_mpi_new.c` were created to explicitly use the four variants of `MPI_Send()`: - `MPI_Ssend()` (Synchronous Send) - `MPI_Bsend()` (Buffered Send) - `MPI_Rsend()` (Ready Send) - `MPI_Isend()` (Non-Blocking Send) Each version was tested to observe differences in behavior, potential errors, and reliability. The results provide insight into which method is best suited for reliable communication.

Using MPI_Ssend resulted in no unexpected errors. The output was in an orderly fashion.
Using a manually allocated buffer with MPI_Bsend() resulted in buffered messages still arriving in order.
MPI_Rsend requires the receivr to be ready before sending. It resulted in occasional errors stating ‘ready send must be matched by receive’. When the receiver was ready, the output appeared correctly.
MPI_Isend resulted in messages arriving out of order.

## Part 2:
The program pingpong.c measures the time taken for message passing between two MPI processes using a simple "ping-pong" communication pattern. One process (rank 0) sends a message to the other process (rank 1), which increments a counter and sends it back. This repeats for a specified number of iterations.
It can be ran using the following:
mpicc -o pingpong pingpong.c mpirun -np 2 ./pingpong 1000

The code was ran using only 2 processes. The latency was found to be 0.00056 seconds (560 microseconds) using a linear fit.

A new version of the program named pingpongvector.c was created. It takes an additional argument which is the length of the array transmitted between the client and the root.
It can be ran using the following:
mpirun -np 2 ./pingpongvector <num_pings> <message_size>

### Results:
The data was plotted, and a linear fit was added to the plot. The graph can be found under the filename pingpongvectorbandwidth.png.
![Ping-Pong Vector Bandwidth](week4/pingpongvectorbandwidth.png)

 The bandwidth is equal to the reciprocal of the slope. The latency was found to be the y-intercept. The bandwidth for pingpongvector.c was found to be 71.99MB/s and the latency was 0.00044.

## Part 3:
In the previous approach, each process independently created a copy of the array before working on different elements. This approach leads to redundancy in initialization and higher memory usage. Instead, we explore the following methods:
Prediction:
•	MPI_Broadcast() should be the fastest for cases where all processes need the entire array.
•	MPI_Scatter() should be more efficient when each process requires only a portion of the data.
•	Manual Send/Recv may introduce overhead due to multiple send/receive calls.
A plot of the results can be found under the filename mpi_comparison.png. It was found that MPI_scatter was the fastest, Bcast was the second fastest and using Send/Recv was the slowest. The performance differences arise due to how data is distributed among processes.
![MPI Comparison](week4/mpi_comparison.png)
MPI_Scatter was the fastest as it efficiently distributes data in parallel. MPI_Bcast sends a copy of the entire data to each process hence why it has increased overhead. MPI_Send and MPI_Recv are the slowest as each operation requires an explicit message passing, and processes wait to receive the message until it has been sent.
A custom reduce operation was created and can be found in comm_test_mpi_custom.c. It can be ran using  mpirun -np <num_processes>  ./comm_test_mpi_custom . The program calculates the sum of my_rank * 10 across all processes using a custom MPI reduction operation. If you run with np=4, the output should be 60. The code arrived at this correct result and agreed with the result of the comm_test_mpi which used the predefined MPI operation.

