#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Struct to hold simulation parameters
typedef struct {
    int cycles;
    int samples;
    char output_file[256];
} SimulationParams;

// Function declarations
int check_args(int argc, char **argv, SimulationParams *params);
void initialise_vector(double vector[], int size, double initial);
void print_vector(double vector[], int size);
void update_positions(double* positions, double* velocities, double* accelerations, int points, double time, double dt, double k, double m);
int generate_timestamps(double* time_stamps, int time_steps, double step_size);
double driver(double time);
void print_header(FILE* p_out_file, int points);

int main(int argc, char **argv)
{
    // Struct to hold input parameters
    SimulationParams params;

    // Check arguments and populate struct
    if (check_args(argc, argv, &params) != 0) {
        return 1; // Exit if argument parsing fails
    }   
    // Number of points (set to 5 for example)
    int points = 5;

    // Spring constant and mass
    double k = 1.0, m = 1.0; 

    // Time step size
    double dt = 0.01;

    // Compute total time steps and step size
    int time_steps = params.cycles * params.samples + 1;
    double step_size = 1.0 / params.samples;

    // Create vector for time stamps
    double* time_stamps = (double*) malloc(time_steps * sizeof(double));
    initialise_vector(time_stamps, time_steps, 0.0);
    generate_timestamps(time_stamps, time_steps, step_size);

    // Create vectors for positions, velocities, and accelerations
    double* positions = (double*) malloc(points * sizeof(double));
    double* velocities = (double*) malloc(points * sizeof(double));
    double* accelerations = (double*) malloc(points * sizeof(double));

    initialise_vector(positions, points, 0.0);
    initialise_vector(velocities, points, 0.0);
    initialise_vector(accelerations, points, 0.0);

    // Open output file
    FILE* out_file = fopen(params.output_file, "w");
    if (!out_file) {
        perror("Error opening file");
        free(time_stamps);
        free(positions);
        free(velocities);
        free(accelerations);
        return 1;
    }
    print_header(out_file, points);

    // Iterate through each time step
    for (int i = 0; i < time_steps; i++) {
        // Update positions and velocities
        update_positions(positions, velocities, accelerations, points, time_stamps[i], dt, k, m);

        // Print index and time stamp
        fprintf(out_file, "%d, %lf", i, time_stamps[i]);

        // Print positions
        for (int j = 0; j < points; j++) {
            fprintf(out_file, ", %lf", positions[j]);
        }

        fprintf(out_file, "\n");
    }

    // Free memory and close file
    free(time_stamps);
    free(positions);
    free(velocities);
    free(accelerations);
    fclose(out_file);

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

// Updates positions and velocities using leapfrog integration
void update_positions(double* positions, double* velocities, double* accelerations, int points, double time, double dt, double k, double m)
{
    // Update accelerations using Hooke's Law
    for (int i = 1; i < points - 1; i++) {
        double left_spring = -k * (positions[i] - positions[i-1]);
        double right_spring = -k * (positions[i] - positions[i+1]);
        accelerations[i] = (left_spring + right_spring) / m;
    }

    // Update velocity using leapfrog method
    for (int i = 1; i < points - 1; i++) {
        velocities[i] += accelerations[i] * dt;
    }

    // Update positions using velocity
    for (int i = 1; i < points - 1; i++) {
        positions[i] += velocities[i] * dt;
    }

    // Boundary conditions (fixed ends)
    positions[0] = driver(time);
    positions[points - 1] = 0.0;
}

// Generates timestamps
int generate_timestamps(double* timestamps, int time_steps, double step_size)
{
    for (int i = 0; i < time_steps; i++) {
        timestamps[i] = i * step_size;
    }                                                              
    return time_steps;
}

// Initializes vector with an initial value
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
