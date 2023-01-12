#include <mpi.h>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

// DistributedExeption(process_number, exception_code, msg)
#define fail(msg) throw std::runtime_error(msg)
/**
 * @brief Process values using send and recv. Each process sends its value to 
 * all other processes.
 * 
 * @param process_number Process number
 * @param process_count Process count
 * @param process_hostname Hostname
 */
void process_values(int process_number, int process_count
  , const char* process_hostname);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    int process_number = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

    int process_count = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);

    char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
    int hostname_length = -1;
    MPI_Get_processor_name(process_hostname, &hostname_length);

    try {
      const double start_time = MPI_Wtime();  // Start time
      process_values(process_number, process_count, process_hostname);  // Process values
      const double elapsed = MPI_Wtime() - start_time;  // Elapsed time
      std::cout << process_hostname << ":" << process_number  // Print time
        << ".m: elapsed time " << elapsed << "s" << std::endl;  // Print time
    } catch (const std::runtime_error& exception) {  // If error
      std::cerr << "error: " << exception.what() << std::endl;  // Print error
      error = EXIT_FAILURE;
    }

    MPI_Finalize();  // Finalize MPI
  } else {  
    std::cerr << "error: could not init MPI" << std::endl;
    error = EXIT_FAILURE;  // Error
  }
  return error;
}

void process_values(int process_number, int process_count
  , const char* process_hostname) {
  std::vector<double> values;
  size_t value_count = 0;

  if (process_number == 0) {
    double value = 0.0;
    while (std::cin >> value) {
      values.push_back(value);
    }

    value_count = values.size();

    for (int target = 1; target < process_count; ++target) {
      static_assert(sizeof(value_count) == sizeof(uint64_t));
      if (MPI_Send(&value_count, /*count*/ 1, MPI_UINT64_T, target
        , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {  // If error
        fail("could not send value count");
      }
      if (MPI_Send(&values[0], value_count, MPI_DOUBLE, target
        , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {  // If error
        fail("could not send values");  // If error
      }
    }
  } else {
    if (MPI_Recv(&value_count, /*capacity*/ 1, MPI_UINT64_T, /*source*/ 0
      , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {  // If error
      fail("could not receive value count");  // If error
    }

    values.resize(value_count);  // Resize values

    if (MPI_Recv(&values[0], /*capacity*/ value_count, MPI_DOUBLE, /*source*/ 0
      , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {  // If error
      fail("could not receive values");  // If error
    }
  }

  for (size_t index = 0; index < values.size(); ++index) {  // For each value
    std::cout << process_hostname << ":" << process_number << ".m: values["
      << index << "] == " << values[index] << std::endl;  // Print value
  }
}
