#include <stdio.h>
#include <mpi.h>

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
        int send_message, total_sum, count;
        send_message = my_rank * 10; // each rank generates a unique value
        total_sum = 0;
        count = 1;

        // perform reduction to sum values from all ranks
        MPI_Reduce(&send_message, &total_sum, count, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        if (0 == my_rank)
        {
                // prints the total sum computed at root
                printf("Total sum using MPI_Reduce: %d\n", total_sum);
        }

        // finalise MPI
        ierror = MPI_Finalize();
        return 0;
}
