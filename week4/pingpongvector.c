#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void initialise_vector(int *vector, int size, int initial){
    for (int i=0; i < size; i++) {
        vector[i] = initial;
    }
}

int main(int argc, char *argv[]) {
    int rank, num_pings, message_size;
    double start_time, end_time;
    int *message_buffer; // pointer for the array

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    num_pings = atoi(argv[1]); // Simple conversion without error handling
    message_size = atoi(argv[2]); // length of array

    // Allocate memory for the message buffer
    message_buffer = (int *)malloc(message_size * sizeof(int));
     // Initialize the message buffer
    initialise_vector(message_buffer, message_size, 1);
    message_buffer[0] = 0; // Use the first element as the counter

if (rank == 0) { // Root process
        start_time = MPI_Wtime();
        while (message_buffer[0] < num_pings) {
            MPI_Send(message_buffer, message_size, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(message_buffer, message_size, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        end_time = MPI_Wtime();
        printf("Final Counter: %d\nElapsed Time: %f seconds\nAverage Time per Ping: %f seconds\n",
               message_buffer[0], end_time - start_time, (end_time - start_time) / num_pings);
    } else { // Client process
        while (message_buffer[0] < num_pings) {
            MPI_Recv(message_buffer, message_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            message_buffer[0]++; // Increment the counter
            MPI_Send(message_buffer, message_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    
    // Free up memory
    free(message_buffer);

    MPI_Finalize();
    return 0;
}
