#include <mpi.h>
#include <iostream>
#include <stdexcept>

#include "UniformRandom.hpp"

#define fail(msg) throw std::runtime_error(msg)

/**
 * @brief generates random statistics for a given process
 * 
 * @param process_number Process number
 * @param process_count Process count
 */
void generate_lucky_statistics(int process_number, int process_count);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    try {
      int process_number = -1;
      MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

      int process_count = -1;
      MPI_Comm_size(MPI_COMM_WORLD, &process_count);

      char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
      int hostname_length = -1;
      MPI_Get_processor_name(process_hostname, &hostname_length);

      generate_lucky_statistics(process_number, process_count);  // Generate statistics
      // for every process
    } catch (const std::runtime_error& exception) {
      std::cout << exception.what() << std::endl;
      error = EXIT_FAILURE;
    }
    MPI_Finalize();
  } else {
    std::cerr << "error: could not init MPI" << std::endl;
    error = EXIT_FAILURE;
  }
  return error;
}

void generate_lucky_statistics(int process_number, int process_count) {
  // Generate my lucky number
  UniformRandom<int> uniformRandom(process_number);  // Create uniform random number generator
  const int my_lucky_number = uniformRandom.between(0, 100);  // Generate random number between 0 and 100

  std::cout << "Process " << process_number << ": my lucky number is "
    << my_lucky_number << std::endl;  // Print my lucky number

  int all_min = -1;  // Minimum of all lucky numbers
  int all_max = -1;  // Maximum of all lucky numbers
  int all_sum = -1;  // Sum of all lucky numbers

  // Update distributed statistics from processes' lucky numbers
  if (MPI_Reduce(/*input*/ &my_lucky_number, /*output*/ &all_min, /*count*/ 1
    , MPI_INT, MPI_MIN, /*root*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce min");  // Reduce minimum
  }
  if (MPI_Reduce(/*input*/ &my_lucky_number, /*output*/ &all_max, /*count*/ 1
    , MPI_INT, MPI_MAX, /*root*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce max");  // Reduce maximum
  }
  if (MPI_Reduce(/*input*/ &my_lucky_number, /*output*/ &all_sum, /*count*/ 1
    , MPI_INT, MPI_SUM, /*root*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce sum");  // Reduce sum
  }
  // MPI_Reduce() is a collective operation, so all processes must call it
  if (process_number == 0) {
    const double all_average = double(all_sum) / process_count;  // Average of all lucky numbers
    std::cout << "Process " << process_number << ": all minimum = "
      << all_min << std::endl;  // Print minimum of all lucky numbers
    std::cout << "Process " << process_number << ": all average = "
      << all_average << std::endl;  // Print average
    std::cout << "Process " << process_number << ": all maximum = "
      << all_max << std::endl;  // Print statistics
  }
}
