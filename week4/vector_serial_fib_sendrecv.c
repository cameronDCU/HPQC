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

        // Send num_arg to all processes
        for (int i = 1; i < size; i++) {
            MPI_Send(&num_arg, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        // Receive num_arg from rank 0
        MPI_Recv(&num_arg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Allocate memory for the array on all processes
    vector = (int *)malloc(num_arg * sizeof(int));

    if (rank == 0) {
        initialise_vector(vector, num_arg);

        // Send the vector to all processes
        for (int i = 1; i < size; i++) {
            MPI_Send(vector, num_arg, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
    } else {
        // Receive the vector from rank 0
        MPI_Recv(vector, num_arg, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

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
