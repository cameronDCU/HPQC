#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// declares the functions that will be called within main
// note how declaration lines are similar to the initial line
// of a function definition, but with a semicolon at the end;
int check_args(int argc, char **argv);
void initialise_vector(int vector[], int size);
void print_vector(int vector[], int size);
int sum_vector(int vector[], int size);

int main(int argc, char **argv)
{
	 // MPI Initialization
        MPI_Init(&argc, &argv);

         // Get the rank (process id) and the number of processes
        int rank, num_proc;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
        // declare and initialise the numerical argument variable
	int num_arg = check_args(argc, argv);

	// creates a vector variable
	// int my_vector[num_arg]; // suffers issues for large vectors
	int* my_vector = malloc (num_arg * sizeof(int));
	// and initialises every element to zero
	initialise_vector(my_vector, num_arg);

        // Calculate the chunk size for each process
        int chunk = num_arg / num_proc;
        int start = rank * chunk;
    	int stop = (rank + 1) * chunk;
	
       // Ensure the last process handles any remaining elements if needed
    	if (rank == num_proc - 1) {
        stop = num_arg;
    	}
       
        // Sum the portion of the vector that each process is responsible for
       int my_sum = sum_vector(my_vector + start, stop - start);

       // Use MPI to gather the results from all processes
       int total_sum = 0;
       MPI_Reduce(&my_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

       // Only the root process (rank 0) will print the total sum
       if (rank == 0) {
           printf("Total Sum: %d\n", total_sum);
       }

       // Free memory
       free(my_vector);

       // Finalize MPI
       MPI_Finalize();

       return 0;
}

// defines a function to sum a vector of ints into another int
int sum_vector(int vector[], int size)
{
	// creates a variable to hold the sum
	int sum = 0;

	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// sets the elements of the vector to the initial value
		sum += vector[i];
	}

	// returns the sum
	return sum;
}

// defines a function to initialise all values in a vector to a given inital value
void initialise_vector(int vector[], int size)
{
    // Handle the first two Fibonacci numbers
    if (size > 0) vector[0] = 0; // First Fibonacci number
    if (size > 1) vector[1] = 1; // Second Fibonacci number

    // Calculate Fibonacci numbers for the rest of the vector
    for (int i = 2; i < size; i++)
    {
        vector[i] = vector[i-1] + vector[i-2]; // Fibonacci relation
    }
}

// defines a function to print a vector of ints
void print_vector(int vector[], int size)
{
	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// prints the elements of the vector to the screen
		printf("%d\n", vector[i]);
	}
}

// defines a function that checks your arguments to make sure they'll do what you need
int check_args(int argc, char **argv)
{
	// declare and initialise the numerical argument
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
		fprintf(stderr, "Correct use: %s [NUMBER]\n", argv[0]);

		// and exit COMPLETELY
		exit (-1);
	}
	return num_arg;
}

