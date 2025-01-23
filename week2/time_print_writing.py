import sys
import time
import os  # For handling file paths and creating directories

def main():
    # Ensure the data directory exists
    if not os.path.exists('data'):
        os.makedirs('data')  # Create the data directory if it doesn't exist

    # Check if the correct number of arguments is provided
    if len(sys.argv) != 2:
        raise Exception("Incorrect number of arguments.\nUsage: python time_print.py <number>")

    try:
        # Convert the first argument to an integer
        in_arg = int(sys.argv[1])
    except ValueError:  # Handle the case where the argument cannot be converted to an integer
        raise Exception("Invalid argument. Please provide an integer.\nUsage: python time_print.py <number>")

    # Get the start time for the loop
    start_time = time.time()

    # Open the file in the 'data' directory for writing
    with open('data/output.txt', 'w') as file:
        # Iterate over all numbers up to the input value
        for i in range(in_arg):
            # Write the index to the file, formatted as requested
            file.write("{}, ".format(i))

    # Get the end time for the loop
    end_time = time.time()

    # Calculate the total run time
    run_time = end_time - start_time

    # Print the time it took to write to the file
    print("\nTime for writing to file: {:.6f} seconds".format(run_time))

if __name__ == "__main__":
    main()
