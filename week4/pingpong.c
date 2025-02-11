#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, counter = 0, num_pings;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    num_pings = atoi(argv[1]); // Simple conversion without error handling

    if (rank == 0) { // Root process
        start_time = MPI_Wtime();
        while (counter < num_pings) {
            MPI_Send(&counter, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&counter, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        end_time = MPI_Wtime();
        printf("Final Counter: %d\nElapsed Time: %f seconds\nAverage Time: %f seconds\n",
               counter, end_time - start_time, (end_time - start_time) / num_pings);
    } else { // Client process
        while (counter < num_pings) {
            MPI_Recv(&counter, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            counter++;
            MPI_Send(&counter, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
