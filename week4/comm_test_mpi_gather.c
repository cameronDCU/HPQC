#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
        // declare and initialise error handling variable
        int ierror = 0;

        // declare and initialise rank and size variables
        int my_rank, uni_size;
        my_rank = uni_size = 0;

        // initialise MPI
        ierror = MPI_Init(&argc, &argv);

        // gets the rank and world size
        ierror = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
        ierror = MPI_Comm_size(MPI_COMM_WORLD, &uni_size);

        // create and initialise transmission variables
        int send_message, count;
        send_message = my_rank * 10; // each rank generates a unique value
        count = 1;
        
        int *recv_messages = NULL; // pointer for root to receive data

        if (0 == my_rank)
        {
                // allocate memory for gathered messages at root
                recv_messages = malloc(uni_size * sizeof(int));
        }

        // gather messages from all ranks at root
        MPI_Gather(&send_message, count, MPI_INT, recv_messages, count, MPI_INT, 0, MPI_COMM_WORLD);

        if (0 == my_rank)
        {
                // sums up the received messages
                int total_sum = 0;
                for (int i = 0; i < uni_size; i++)
                {
                        total_sum += recv_messages[i];
                }

                // prints the total sum computed at root
                printf("Total sum using MPI_Gather: %d\n", total_sum);

                // free allocated memory
                free(recv_messages);
        }

        // finalise MPI
        ierror = MPI_Finalize();
        return 0;
}
