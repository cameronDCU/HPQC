#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// function declaration
int root_task(int uni_size);

int main(int argc, char **argv) 
{
	// declare and initialise error handling variable
	int ierror = 0;
	
	// declare and initialise rank and size varibles
	int my_rank, uni_size;
	my_rank = uni_size = 0;

	// declare and initialise the numerical argument variable
	int num_arg = 0;

	// check the number of arguments
	if (argc == 2) // program name and numerical argument
	{
		// declare and initialise the numerical argument
		num_arg = atoi(argv[1]);
	}
	else // the number of arguments is incorrect
	{
		// raise an error
		fprintf(stderr, "ERROR: You did not provide a numerical argument!\n");
		fprintf(stderr, "Correct use: mpicc -n 4 proof [NUMBER]\n");
		return (-1);
	}

	// intitalise MPI
	ierror = MPI_Init(&argc, &argv);

	// gets the rank and world size
	ierror = MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	ierror = MPI_Comm_size(MPI_COMM_WORLD,&uni_size);

	// creates and initialies transmission variables
	int send_message, recv_message, count, dest, source, tag;
	send_message = recv_message = dest = source = tag = 0;
	count = 1;
	MPI_Status status;
	
	if (uni_size > 1)
	{
		if (0 == my_rank)
		{
			root_task(uni_size);
		} // end if (0 == my_rank)
		else // i.e. (0 != my_rank)
		{
			// sets the destination for the message
			dest = 0; // destination is root

			// creates the message
			send_message = my_rank * num_arg;

			// sends the message
			MPI_Send(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
		} // end else // i.e. (0 != my_rank)
	} // end if (uni_size > 1)
	else // i.e. uni_size <=1
	{
		// prints a warning
		fprintf(stderr, "Unable to communicate with less than 2 processes. MPI communicator size = %d\n", uni_size);
	}

	// finalise MPI
	ierror = MPI_Finalize();
	return 0;
}

int root_task(int uni_size)
{

	// creates and initialies transmission variables
	int recv_message, count, source, tag;
	recv_message = source = tag = 0;
	count = 1;
	MPI_Status status;

	// creates and intiialises the variable for the final output
	int output_sum = 0;
	
	// iterates through all the other ranks
	for (int their_rank = 1; their_rank < uni_size; their_rank++)
	{
		// sets the source argument to the rank of the sender
		source = their_rank;
		// receives the messages
		MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
		// adds the values together
		output_sum += recv_message;
	} // end for (int their_rank = 1; their_rank < uni_size; their_rank++)
	// outputs the result
	printf("The combined result is %d\n", output_sum);

	return 0;
}
