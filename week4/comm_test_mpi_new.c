#include <stdio.h>
#include <mpi.h>
    
    // This function initialises MPI
    void init_mpi(int argc, char **argv, int *my_rank, int*uni_size){
	// declare and initialise error handling variable
	MPI_Init(&argc, &argv);

	// gets the rank and world size
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&uni_size);
}
    
    // Function to perform root task
    void root_task(int uni_size) {
    int recv_message, count = 1, source, tag = 0;
    MPI_Status status;

    for (int their_rank = 1; their_rank < uni_size; their_rank++) {
        source = their_rank;
        MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
        printf("Hello, I am %d of %d. Received %d from %d\n", 0, uni_size, recv_message, source);
    }
}

    void non_root_task(int my_rank, int uni_size) {
    int send_message = my_rank * 10, count = 1, dest = 0, tag = 0;

    MPI_Send(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
    printf("Hello, I am %d of %d. Sent %d to %d\n", my_rank, uni_size, send_message, dest);
}
    // Main function runs the rest of the code
    int main(int argc, char **argv) {
        int my_rank, uni_size, ierror;
    
        // Initialize MPI
        ierror = init_mpi(argc, argv, &my_rank, &uni_size);

	// creates and initialies transmission variables
	int send_message, recv_message, count, dest, source, tag;
	send_message = recv_message = dest = source = tag = 0;
	count = 1;
	MPI_Status status;
	
	if (uni_size > 1)
	{
		if (0 == my_rank)
		{root_task(uni_size);
		} // end for (int their_rank = 1; their_rank < uni_size; their_rank++)
		   // end if (0 == my_rank)
		else // i.e. (0 != my_rank)
		{
                 non_root_task(my_rank, uni_size)
		} // end else // i.e. (0 != my_rank)
	} // end if (uni_size > 1)
	else // i.e. uni_size <=1
	{
		// prints a warning
		printf("Unable to communicate with less than 2 processes. MPI communicator size = %d\n", uni_size);
	}

	// finalise MPI
	ierror = MPI_Finalize();
	return 0;
}
