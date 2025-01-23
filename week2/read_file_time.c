#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(int argc, char **argv) {
    // Check for correct usage
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return 1;
    }

    char *file_path = argv[1];
    FILE *file;
    char buffer[BUFFER_SIZE]; // Buffer to store file contents
    int *numbers = NULL;      // Array to store numbers read from file
    int count = 0, i = 0;
    clock_t start_time, end_time;

    // Open the file for reading
    file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s.\n", file_path);
        return 1;
    }

    // Start timing the file reading process
    start_time = clock();

    // Read the entire file into the buffer
    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Count the number of commas to estimate number of integers
        for (i = 0; buffer[i] != '\0'; i++) {
            if (buffer[i] == ',') count++;
        }

        // Allocate memory for the integers
        numbers = (int *)malloc((count + 1) * sizeof(int));
        if (numbers == NULL) {
            fprintf(stderr, "Error: Memory allocation failed.\n");
            fclose(file);
            return 1;
        }

        // Tokenize the buffer to extract individual numbers
        i = 0;
        char *token = strtok(buffer, ", ");
        while (token != NULL) {
            numbers[i++] = atoi(token); // Convert to integer and store in array
            token = strtok(NULL, ", ");
        }
    }

    // Stop timing
    end_time = clock();

    // Close the file
    fclose(file);

    // Calculate runtime
    double runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    // Print the results
    printf("Read %d numbers from file %s.\n", i, file_path);
    printf("Time taken to read file: %lf seconds.\n", runtime);

    // Print the numbers (optional, for verification)
    printf("Numbers:\n");
    for (int j = 0; j < i; j++) {
        printf("%d ", numbers[j]);
    }
    printf("\n");

    // Free allocated memory
    free(numbers);

    return 0;
}
