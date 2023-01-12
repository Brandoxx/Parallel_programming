#include <mpi.h>
#include <omp.h>
#include <iostream>

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
      << " of " << process_count << " on " << process_hostname << std::endl;
      // Print hello message from every process

    int thread_count = omp_get_max_threads(); // Get thread count
    if (argc == 2) {  // Check if thread count was specified
      thread_count = atoi(argv[1]);  // Set thread count
    }
    #pragma omp parallel num_threads(thread_count) default(none) \
      shared(std::cout, process_number, process_count, process_hostname)
    {
      #pragma omp critical(stdout)
      std::cout << "\tHello from thread " << omp_get_thread_num() << " of "
        << omp_get_num_threads() << " of process " << process_number
        << " of " << process_count << " on " << process_hostname << std::endl;
        // Print hello message from every thread on every process on every thread
    }

    MPI_Finalize();  // Finalize MPI
  } else {
    std::cerr << "error: could not init MPI" << std::endl;
  }
  return 0;
}
