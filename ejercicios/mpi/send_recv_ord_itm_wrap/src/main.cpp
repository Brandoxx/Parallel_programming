// Copyright  2022 Brandon Mora Umana GNU GENERAL PUBLIC LICENSE
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include "Mpi.hpp"
#include "MpiError.hpp"

void process_values(Mpi& mpi, int process_number, int process_count
  , const char* process_hostname);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
try {
  Mpi mpi(argc, argv);
  int process_number = mpi.getProcessNumber();
  int process_count = mpi.getProcessCount();
  const char* process_hostname = mpi.getHostname().c_str();
    try {
      const double start_time = MPI_Wtime();
      process_values(mpi, process_number, process_count, process_hostname);
      const double elapsed = MPI_Wtime() - start_time;
      std::cout << process_hostname << ":" << process_number
        << ".m: elapsed time " << elapsed << "s" << std::endl;
    } catch (const MpiError& mpierror) {
      std::cerr << "error: " << mpierror.what() << std::endl;
      error = EXIT_FAILURE;
    }
  } catch (const MpiError& mpierror) {
    std::cerr << "error: " << mpierror.what() << std::endl;
    error = EXIT_FAILURE;
  }
  return error;
}
/**
 * @brief process the values. it receives the values from the process 0 and
 * sends them to the other processes
 * 
 * @param process_number the process number
 * @param process_count the amount of processes
 * @param process_hostname the name of the host
 *
 */
void process_values(Mpi& mpi, int process_number, int process_count
  , const char* process_hostname) {
  std::vector<double> values;
  size_t value_count = 0;
  // we read the values from the standard input
  if (process_number == 0) {  //  if we are the main process we do the reading
    double value = 0.0;
    // we put it in values
    while (std::cin >> value) {
      // we push the value to the values vector
      values.push_back(value);
    }
    // we get the size of the values vector
    value_count = values.size();

    for (int target = 1; target < process_count; ++target) {
      // compilation assert, we check if the computer architecture is 64 bits
      // if it is not we throw an error and stops the compilation
      static_assert(sizeof(value_count) == sizeof(uint64_t)
        , "update MPI_Send data type to match your architecture");\
        // we send the value count to the target process
        // REMEMBER: we are MPI sends and receives messages
        // in the order they are send them. So we can count that the 1st message
        // will be the value count
      try {
        mpi.send(value_count, target, 0);
      } catch (const MpiError& mpierror) {
        throw MpiError(mpierror.what(), mpi);
      }
      try {
        mpi.send(values, target, 0);
      } catch (const MpiError& mpierror) {
        throw MpiError(mpierror.what(), mpi);
      }
    }
  } else {  //  if we are not the main process we do the receiving and printing
  //  we receive the value count from the main process
    try {
      mpi.receive(value_count, 0, 0);
    } catch (const MpiError& mpierror) {
      throw MpiError(mpierror.what(), mpi);
    }
    //  we resize the values vector to match the value count
    values.resize(value_count);
    //  we receive the array of values from the main process
    try {
      mpi.receive(values, value_count, 0, 0);
    } catch (const MpiError& mpierror) {
      throw MpiError(mpierror.what(), mpi);
    }
  }
  // we print the values
  for (size_t index = 0; index < values.size(); ++index) {
    std::cout << process_hostname << ":" << process_number << ".m: values["
      << index << "] == " << values[index] << std::endl;
  }
}
