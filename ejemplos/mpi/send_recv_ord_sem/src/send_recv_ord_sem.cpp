#include <mpi.h>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

// DistributedExeption(process_number, exception_code, msg)
#define fail(msg) throw std::runtime_error(msg)
/**
 * @brief Prints in order of process number using MPI_Send and MPI_Recv
 * 
 * @param process_number Process number
 * @param process_count Process count
 * @param process_hostname Hostname
 */
void greet(int process_number, int process_count, const char* process_hostname);

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

    try {  // Try to greet
      greet(process_number, process_count, process_hostname);  // Greet
    } catch (const std::runtime_error& exception) {  // Error
      std::cerr << "error: " << exception.what() << std::endl;  // Print error
      error = EXIT_FAILURE;  // Set error code to failure
    }

    MPI_Finalize();
  } else {
    std::cerr << "error: could not init MPI" << std::endl;
    error = EXIT_FAILURE;
  }
  return error;
}

void greet(int process_number, int process_count, const char* process_hostname)
{
  const int previous_process = (process_count + process_number - 1) 
    % process_count;  // Previous process number (circular)
  const int next_process = (process_number + 1) % process_count;
  bool can_print = true;  // Can print flag (true if main thread)

  if (process_number > 0) {  // If not main thread
    // receive(&can_print, 1, previous_process)  
    if (MPI_Recv(&can_print, /*count*/ 1, MPI_C_BOOL, previous_process
      , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {  // Receive
      // can_print
      fail("could not receive message");
    }
  }

  std::cout << "Hello from main thread of process " << process_number
    << " of " << process_count << " on " << process_hostname << std::endl;  // Print
    // message

  // send(can_print, 1, next_process);
  if (MPI_Send(&can_print, /*count*/ 1, MPI_C_BOOL, next_process, /*tag*/ 0
    , MPI_COMM_WORLD) != MPI_SUCCESS) {  // Send can_print to next process
    fail("could not send message");  // Fail if could not send message
  }
}
