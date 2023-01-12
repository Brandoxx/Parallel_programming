#include <mpi.h>
#include <omp.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>

/**
 * @brief Calculate the start of the range for the given rank.
 * 
 * @param rank the rank of the process
 * @param end  the end of the range
 * @param workers  the number of workers
 * @param begin  the begin of the range
 * @return int 
 */
int calculate_start(int rank, int end, int workers, int begin) {
  const int range = end - begin;
  return begin + rank * (range / workers) + std::min(rank, range % workers); 
}

/**
 * @brief Calculate the end of the range for the given rank.
 * 
 * @param rank  the rank of the process
 * @param end  the end of the range
 * @param workers  the number of workers
 * @param begin  the begin of the range
 * @return int 
 */

int calculate_finish(int rank, int end, int workers, int begin) {
  return calculate_start(rank + 1, end, workers, begin);
}

int main(int argc, char* argv[]) {
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    int process_number = -1;  // rank
    MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

    int process_count = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);

    char process_hostname[MPI_MAX_PROCESSOR_NAME];
    int hostname_length = -1;
    MPI_Get_processor_name(process_hostname, &hostname_length);
    int overall_start = -1;
    int overall_finish = -1;
    if (argc == 3) {
      overall_start = atoi(argv[1]);
      overall_finish = atoi(argv[2]);
    } else { 
      if (process_number == 0) {
        std::cout << "Please enter the start value: " << std::endl;
        std::cin >> overall_start;
        std::cout << "Please enter the finish value: " << std::endl;
        std::cin >> overall_finish;
        for (int process = 1; process < process_count; ++process) {
          if (MPI_Send(&overall_start, 1, MPI_INT, process, 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
            std::cerr << "Error sending start value to process " << process << std::endl;
          }
          if (MPI_Send(&overall_finish, 1, MPI_INT, process, 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
            std::cerr << "Error sending finish value to process " << process << std::endl;
          }
        }
      } else {
        if (MPI_Recv(&overall_start, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
          std::cerr << "Error receiving start value from process 0" << std::endl;
        }
        if (MPI_Recv(&overall_finish, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
          std::cerr << "Error receiving finish value from process 0" << std::endl;
        }
      }
    }
    const int process_start = calculate_start(process_number, overall_finish // calculate_start
      , process_count, overall_start); // rank
    const int process_finish = calculate_finish(process_number, overall_finish // calculate_finish
      , process_count, overall_start);
    const int process_size = process_finish - process_start; // size

    std::cout << process_hostname << ':' << process_number << ": range [" 
      << process_start << ", " << process_finish << "[ size " << process_size
      << std::endl; // prints the range and size

    #pragma omp parallel default(none) \
      shared(std::cout, process_number, process_count, process_hostname, process_start, process_finish, process_size) // shared
    { // shared variables
      int thread_start = -1; // thread_start
      int thread_finish = -1;   // thread_finish

      // distribute the range of the process among the threads
      #pragma omp for
      for (int index = process_start; index < process_finish; ++index) {
        if (thread_start == -1) { // if we are in the first iteration
          thread_start = index; // thread_start = index
        }
        thread_finish = index; // thread_finish = index
      }

      ++thread_finish; // increment thread_finish
      const int thread_size = thread_finish - thread_start; // thread_size
      // print the range and size of the thread in a critical section
      #pragma omp critical(stdout)
      std::cout << '\t' << process_hostname << ':' << process_number << '.'
        << omp_get_thread_num() << ": range [" << thread_start << ", " <<
        thread_finish << "[ size " << thread_size << std::endl;
    } 
    MPI_Finalize();
  }

  return 0;
}
