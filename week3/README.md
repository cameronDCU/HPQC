# Part 1: Hello World
helo_mpi.c is code where multiple processes print their rank and the total number of processes in the communicator.

It can be ran using mpirun -np <num_processes> bin/hello_mpi. The code was benchmarked for a number of different processes up to 10.

![MPI Program Benchmark](https://github.com/cameronDCU/HPQC/blob/main/week3/mpi.png)
For the MPI program as the number of processes increases, the real time decreases, indicating that the workload is being distributed across multiple processes, leading to faster execution.

For the serial program the execution time remains relatively constant regardless of the input as there is no parallel processing.
![Serial Program Benchmark](https://github.com/cameronDCU/HPQC/blob/main/week3/serial.png)

# Part 2: Proof.c

### main()

The `main()` function is responsible for initializing the necessary variables and handling the program's execution flow. The key steps are as follows:

1. **Initializes Variables:**
   - `rank`: Represents the unique identifier of the current process.
   - `size`: Represents the total number of processes involved in the execution.

2. **Input Validation:**
   - The program checks for the correct input, which should be a single numerical value. This input will be used in the subsequent calculations.

3. **MPI Initialization:**
   - The program initializes the MPI environment and retrieves the rank of the current process and the total number of processes.

4. **Loop Through Processes:**
   - Once the input and MPI are properly initialized, the program will loop through the processes, performing calculations and data exchanges.

### check_sum()

The `check_sum()` function is responsible for calculating the total sum across all processes. It performs the following tasks:

1. **Initializes the `output_sum`:**
   - A variable `output_sum` is initialized to store the total sum of the calculations.

2. **Loops Through All Processes:**
   - The function loops through all processes to accumulate values. Each process sends data to process 0 for aggregation.

### Example: Running with 4 Ranks and Input Value 8

Let’s consider an example where the program is run with 4 ranks and an input value of 8:

1. **Rank 1 sends `1 x 8` to Rank 0.**
2. **Rank 2 sends `2 x 8` to Rank 0.**
3. **Rank 3 sends `3 x 8` to Rank 0.**
4. **Rank 4 sends `4 x 8` to Rank 0.**

Rank 0 receives these values, and the total sum is calculated as:
output_sum = (1 * 8) + (2 * 8) + (3 * 8) + (4 * 8) = 8 + 16 + 24 + 32 = 80

This calculation can be simplified to:
output = ((rank-1)*rank)/2 * input_value

# Part 3: MPI Vector Addition

I began by reading and understanding the serial program, `vector_serial.c`, which performs basic vector addition. The key steps in the serial code include:
- **Input Validation**: The program checks for correct input (a numerical argument for the vector size).
- **Vector Initialization**: A vector is created and initialized with Fibonacci numbers.
- **Summing the Vector**: The program sums all elements of the vector.
- **Output**: The sum of the vector is printed.

I executed the serial code and benchmarked its performance using the following commands:
gcc PHY1090/week3/vector_serial.c -o bin/vector_serial
time ./bin/vector_serial 10

I made a copy of the serial code and modified it to perform a non-trivial mathematical operation. Instead of initializing the vector with zeros, I filled the vector with Fibonacci numbers, which added complexity to the program. The program takes a single numerical argument from the command line that specifies the size of the vector. The program initializes the vector using Fibonacci numbers, with the first two values set as 0 and 1, and the rest calculated based on the Fibonacci relation.

gcc PHY1090/week3/vector_serial_fib.c -o bin/vector_serial_fib
time ./bin/vector_serial_fib 10

Using the pseudocode from the lecture slides, I created a parallel version of the program named vector_serial_fib_mpi.c. I benchmarked both the serial and parallel implementations and compared their performance.

![Serial vs Parallel Time]()

The parallelized code performed worse than the serial implementation for this code. This may be due to the overhead for MPI communication and possible unequal distribution of the workload.
