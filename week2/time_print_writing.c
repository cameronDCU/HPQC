#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);
double to_second_float(struct timespec in_time);

int main(int argc, char **argv)
{
    // Creates and initializes the variables
    int i, input;
    i = input = 0;
    struct timespec start_time, end_time, time_diff;
    double runtime = 0.0;

    // Check if there are the right number of arguments
    if (argc == 2)
    {
        // Converts the first argument to an integer
        input = atoi(argv[1]);
    }
    else // If arguments are incorrect
    {
        // Raise an error and exit
        fprintf(stderr, "Incorrect arguments. Usage: ./program_name <number>\n");
        exit(-1);
    }

    // Open the file to write the output (you can specify your file name here)
    FILE *output_file = fopen("data/new_output_c.txt", "w");
    if (output_file == NULL)
    {
        // Check if the file was opened successfully
        fprintf(stderr, "Error opening file for writing.\n");
        exit(-1);
    }

    // Get the time before the loop
    timespec_get(&start_time, TIME_UTC);

    // Iterates over all numbers up to the input
    for (i = 0; i < input; i++)
    {
        // Write the index to the file instead of printing
        fprintf(output_file, "%d, ", i);
    }

    // Get the time after the loop
    timespec_get(&end_time, TIME_UTC);

    // Calculate the runtime
    time_diff = calculate_runtime(start_time, end_time);
    runtime = to_second_float(time_diff);
    // Print output times
    printf("Time taken to read the file: %lf seconds\n", runtime);
    fprintf(output_file, "\n\nRuntime for core loop: %lf seconds.\n", runtime);

    // Close the file after writing
    fclose(output_file);

    return 0;
}

double to_second_float(struct timespec in_time)
{
    // Creates and initializes the variables
    float out_time = 0.0;
    long int seconds, nanoseconds;
    seconds = nanoseconds = 0;

    // Extract the elements from in_time
    seconds = in_time.tv_sec;
    nanoseconds = in_time.tv_nsec;

    // Calculate the time in seconds by adding the seconds and nanoseconds
    out_time = seconds + nanoseconds / 1e9;

    // Return the time as a double
    return out_time;
}

struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time)
{
    // Creates and initializes the variables
    struct timespec time_diff;
    long int seconds, nanoseconds;

    // Calculate the time difference in seconds and nanoseconds
    seconds = end_time.tv_sec - start_time.tv_sec;
    nanoseconds = end_time.tv_nsec - start_time.tv_nsec;

    // If the nanoseconds part is negative, adjust the seconds
    if (nanoseconds < 0)
    {
        // "Carry the one!"
        seconds = seconds - 1;
        nanoseconds = ((long int) 1e9) + nanoseconds;
    }

    // Store the runtime difference in time_diff
    time_diff.tv_sec = seconds;
    time_diff.tv_nsec = nanoseconds;

    return time_diff;
}
