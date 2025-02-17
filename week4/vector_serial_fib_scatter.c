#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void initialise_vector(int vector[], int size);
int sum_vector(int vector[], int start, int end);

int main(int argc, char **argv) {
    int rank, size, num_arg;
    int *vector = NULL, *local_vector = NULL, local_sum, total_sum;
    int chunk_size, remainder;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        if (argc != 2) {
            fprintf(stderr, "Usage: %s [NUMBER]\n", argv[0]);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        num_arg = atoi(argv[1]);
    }

    // Broadcast num_arg to all processes
    MPI_Bcast(&num_arg, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate chunk size
    chunk_size = num_arg / size;
    remainder = num_arg % size;

    // Allocate memory for local chunks
    local_vector = (int *)malloc(chunk_size * sizeof(int));

    if (rank == 0) {
        // Allocate and initialize vector only on root
        vector = (int *)malloc(num_arg * sizeof(int));
        initialise_vector(vector, num_arg);
    }

    // Scatter chunks of the vector to all processes
    MPI_Scatter(vector, chunk_size, MPI_INT, local_vector, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Compute local sum
    local_sum = sum_vector(local_vector, 0, chunk_size);

    // Handle remainder (only root processes compute extra elements)
    if (rank == 0 && remainder > 0) {
        for (int i = num_arg - remainder; i < num_arg; i++) {
            local_sum += vector[i];
        }
    }

    // Reduce all partial sums to get the total sum
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total Sum: %d\n", total_sum);
        free(vector);
    }

    free(local_vector);
    MPI_Finalize();
    return 0;
}

void initialise_vector(int vector[], int size) {
    if (size > 0) vector[0] = 0;
    if (size > 1) vector[1] = 1;
    for (int i = 2; i < size; i++) {
        vector[i] = vector[i - 1] + vector[i - 2];
    }
}

int sum_vector(int vector[], int start, int end) {
    int sum = 0;
    for (int i = start; i < end; i++) {
        sum += vector[i];
    }
    return sum;
}

