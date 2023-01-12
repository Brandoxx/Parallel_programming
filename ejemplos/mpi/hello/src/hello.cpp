#include <mpi.h>
#include <iostream>

/**
 * @brief Hello world program using MPI
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char* argv[]) {
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {  // Initialize MPI
    int process_number = -1;  // Process number
    MPI_Comm_rank(MPI_COMM_WORLD, &process_number);  // Get process number

    int process_count = -1;  // Process count
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);  // Get process count

    char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };  // Process hostname
    int hostname_length = -1;  // Hostname length
    MPI_Get_processor_name(process_hostname, &hostname_length);  // Get process hostname

    std::cout << "Hello from main thread of process " << process_number
      << " of " << process_count << " on " << process_hostname << std::endl;  // Print hello message
      // from every process

    MPI_Finalize();  // Finalize MPI
  } else {  // Error initializing MPI
    std::cerr << "error: could not init MPI" << std::endl;  // Print error message
  }
  return 0;
}
