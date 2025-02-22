#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

// Struct to hold simulation parameters
typedef struct {
    int cycles;
    int samples;
    char output_file[256];
} SimulationParams;

// Function declarations
int check_args(int argc, char **argv, SimulationParams *params);
void initialise_vector(double vector[], int size, double initial);
void update_positions(double* positions, int points, double time);
int generate_timestamps(double* time_stamps, int time_steps, double step_size);
double driver(double time);
void print_header(FILE* p_out_file, int points);

int main(int argc, char **argv)
{
    // Initialise MPI
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Check arguments and populate struct
    SimulationParams params;
    if (check_args(argc, argv, &params) != 0) {
        MPI_Finalize();
        return 1; // Exit if argument parsing fails
    }

    // Number of points (set to 5 for example)
    int points = 5;

    // Compute total time steps and step size
    int time_steps = params.cycles * params.samples + 1;
    double step_size = 1.0 / params.samples;

    // Create vector for time stamps
    double* time_stamps = (double*) malloc(time_steps * sizeof(double));
    initialise_vector(time_stamps, time_steps, 0.0);
    generate_timestamps(time_stamps, time_steps, step_size);

    // Create vector for positions and initialise every element to zero
    double* positions = (double*) malloc(points * sizeof(double));
    initialise_vector(positions, points, 0.0);

    // Create buffer for each process to send its data
    double* local_data = (double*) malloc((time_steps * (points + 1)) * sizeof(double));

    // Fill local_data with the necessary information (time step, positions)
    for (int i = 0; i < time_steps; i++) {
        local_data[i * (points + 1)] = time_stamps[i]; // Store the timestamp
        update_positions(positions, points, time_stamps[i]);
        for (int j = 0; j < points; j++) {
            local_data[i * (points + 1) + j + 1] = positions[j]; // Store positions
        }
    }

    // Gather all the data at root process (rank 0)
    double* gathered_data = NULL;
    if (rank == 0) {
        gathered_data = (double*) malloc(size * time_steps * (points + 1) * sizeof(double));
    }

    MPI_Gather(local_data, time_steps * (points + 1), MPI_DOUBLE, gathered_data, 
               time_steps * (points + 1), MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Write data to file on the root process
    if (rank == 0) {
        FILE* out_file = fopen(params.output_file, "w");
        if (!out_file) {
            perror("Error opening file");
            free(time_stamps);
            free(positions);
            free(gathered_data);
            MPI_Finalize();
            return 1;
        }
        
        print_header(out_file, points);

        // Write the gathered data
        for (int i = 0; i < size * time_steps; i++) {
            fprintf(out_file, "%d, %lf", i, gathered_data[i * (points + 1)]);
            for (int j = 0; j < points; j++) {
                fprintf(out_file, ", %lf", gathered_data[i * (points + 1) + j + 1]);
            }
            fprintf(out_file, "\n");
        }

        fclose(out_file);
        free(gathered_data);
    }

    // Free local memory and finalize MPI
    free(local_data);
    free(time_stamps);
    free(positions);

    MPI_Finalize();
    return 0;
}

// Prints header to output file
void print_header(FILE* p_out_file, int points)
{
    fprintf(p_out_file, "#, time");
    for (int j = 0; j < points; j++) {
        fprintf(p_out_file, ", y[%d]", j);
    }
    fprintf(p_out_file, "\n");
}

// Defines a simple harmonic oscillator as the driving force
double driver(double time)
{
    return sin(time * 2.0 * M_PI);
}

// Updates positions
void update_positions(double* positions, int points, double time)
{
    double* new_positions = (double*) malloc(points * sizeof(double));
    new_positions[0] = driver(time);
    for (int i = 1; i < points; i++) {
        new_positions[i] = positions[i-1];
    }
    for (int i = 0; i < points; i++) {
        positions[i] = new_positions[i];
    }
    free(new_positions);
}

// Generates timestamps
int generate_timestamps(double* timestamps, int time_steps, double step_size)
{
    for (int i = 0; i < time_steps; i++) {
        timestamps[i] = i * step_size;
    }
    return time_steps;
}

// Initialises vector with an initial value
void initialise_vector(double vector[], int size, double initial)
{
    for (int i = 0; i < size; i++) {
        vector[i] = initial;
    }
}

// Checks and parses command line arguments
int check_args(int argc, char **argv, SimulationParams *params)
{
    if (argc != 4) {
        fprintf(stderr, "ERROR: Incorrect number of arguments.\n");
        fprintf(stderr, "Usage: %s <cycles> <samples> <output_file>\n", argv[0]);
        return -1;
    }
    // Parse cycles and samples
    params->cycles = atoi(argv[1]);
    params->samples = atoi(argv[2]);
    // Parse output file
    snprintf(params->output_file, sizeof(params->output_file), "%s", argv[3]);
    return 0;
}

