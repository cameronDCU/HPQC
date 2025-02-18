#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// Function declarations
int check_args(int argc, char **argv);
void initialise_vector(int vector[], int size);
int sum_vector(int vector[], int size);

int main(int argc, char **argv)
{
    // MPI Initialization
    MPI_Init(&argc, &argv);

    // Get the rank (process id) and the number of processes
    int rank, num_proc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

    // Declare and initialize the numerical argument variable
    int num_arg = check_args(argc, argv);

    // Create and initialize the vector in the root process (rank 0)
    int* my_vector = NULL;
    if (rank == 0) {
        my_vector = malloc(num_arg * sizeof(int));
        initialise_vector(my_vector, num_arg);
    }

    // Calculate the chunk size for each process
    int chunk = num_arg / num_proc;
    int remainder = num_arg % num_proc;

    // Adjust start and stop indices for each process
    int start = rank * chunk + (rank < remainder ? rank : remainder); // Adjust for remainder
    int stop = start + chunk + (rank < remainder ? 1 : 0);

    // Allocate memory for the local vector
    int local_size = stop - start;
    int* local_vector = malloc(local_size * sizeof(int));

    // Scatter the vector to all processes
    MPI_Scatterv(my_vector, // Send buffer
                 (int[]){chunk + (rank < remainder ? 1 : 0)}, // Send counts for each process
                 (int[]){0}, // Displacements
                 MPI_INT, // Data type
                 local_vector, // Receive buffer
                 local_size, // Number of elements for each process
                 MPI_INT, // Data type
                 0, // Root process
                 MPI_COMM_WORLD);

    // Sum the portion of the vector that each process is responsible for
    int my_sum = sum_vector(local_vector, local_size);

    // Use MPI to gather the partial sums and reduce them into the total sum
    int total_sum = 0;
    MPI_Reduce(&my_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Only the root process (rank 0) will print the total sum
    if (rank == 0) {
        printf("Total Sum: %d\n", total_sum);
    }

    // Free memory
    free(local_vector);
    if (rank == 0) {
        free(my_vector);
    }

    // Finalize MPI
    MPI_Finalize();

    return 0;
}

// Function to sum a vector of ints into another int
int sum_vector(int vector[], int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += vector[i];
    }
    return sum;
}

// Function to initialize all values in a vector to Fibonacci numbers
void initialise_vector(int vector[], int size)
{
    if (size > 0) vector[0] = 0;
    if (size > 1) vector[1] = 1;
    for (int i = 2; i < size; i++) {
        vector[i] = vector[i-1] + vector[i-2];
    }
}

// Function that checks the program's arguments
int check_args(int argc, char **argv)
{
    int num_arg = 0;
    if (argc == 2) {
        num_arg = atoi(argv[1]);
    } else {
        fprintf(stderr, "ERROR: You did not provide a numerical argument!\n");
        fprintf(stderr, "Correct use: %s [NUMBER]\n", argv[0]);
        exit(-1);
    }
    return num_arg;
}
