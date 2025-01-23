import time

def read_file(filename):
    """Reads the content of a file and times the operation."""
    start_time = time.time()  # Record the start time
    
    with open(filename, 'r') as file:  # Open the file in read mode
        content = file.read()  # Read the entire content of the file into memory
    
    end_time = time.time()  # Record the end time
    
    # Calculate the time taken to read the file
    elapsed_time = end_time - start_time
    
    # Print the runtime and the first few characters of the file for confirmation
    print(f"Time taken to read the file: {elapsed_time:.6f} seconds")
    return elapsed_time

if __name__ == "__main__":
    # File to be read (update the path if needed)
    filename = "data/output.txt"  # Replace with your actual file path

    # Read the file and measure the time
    try:
        read_file(filename)
    except FileNotFoundError:
        print(f"Error: The file '{filename}' was not found.")
