#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// Custom sum function for MPI_Op_create
void custom_sum_function(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype) {
    int i;
    int *in = (int *)invec;
    int *inout = (int *)inoutvec;

    for (i = 0; i < *len; i++) {
        inout[i] += in[i];
    }
}

int main(int argc, char **argv)
{
    int my_rank, uni_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &uni_size);

    int send_message = my_rank * 10;  // Each process will send a unique value

    // Define custom MPI operation
    MPI_Op custom_sum;
    MPI_Op_create(custom_sum_function, 1, &custom_sum);

    int recv_message_custom = 0;

    // Perform sum using the custom operation
    MPI_Reduce(&send_message, &recv_message_custom, 1, MPI_INT, custom_sum, 0, MPI_COMM_WORLD);

    // Print the result only on rank 0 (root)
    if (my_rank == 0) {
        printf("Total sum using custom MPI_Op: %d\n", recv_message_custom);
    }

    // Free custom operation
    MPI_Op_free(&custom_sum);

    MPI_Finalize();
    return 0;
}
