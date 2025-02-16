#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void initialise_vector(int vector[], int size);
int sum_vector(int vector[], int start, int end);

int main(int argc, char **argv) {
    int rank, size, num_arg;
    int *vector = NULL, local_sum, total_sum;

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

    // Broadcast size to all processes
    MPI_Bcast(&num_arg, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Allocate memory for the array on all processes
    vector = (int *)malloc(num_arg * sizeof(int));

    // Root initializes and broadcasts the full array
    if (rank == 0) {
        initialise_vector(vector, num_arg);
    }
    MPI_Bcast(vector, num_arg, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process computes partial sum
    int start = rank * (num_arg / size);
    int end = (rank == size - 1) ? num_arg : start + (num_arg / size);
    local_sum = sum_vector(vector, start, end);

    // Gather the sum at the root
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total Sum: %d\n", total_sum);
    }

    free(vector);
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
