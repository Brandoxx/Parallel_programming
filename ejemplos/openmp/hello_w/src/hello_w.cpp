#include <omp.h>

#include <iostream>
/// @brief  This is a simple example of how to use OpenMP. It receives a number 
///         of threads as a parameter and prints a message from each thread.
/// @param argc Number of arguments
/// @param argv Arguments, number of threads
/// @return 0 if everything is ok
int main(int argc, char* argv[]) {
  int thread_count = omp_get_max_threads(); // Get the number of threads available
  if (argc == 2) {
    thread_count = atoi(argv[1]); // If the user provides a number of threads
  }

  #pragma omp parallel num_threads(thread_count) // Create a parallel region, with the number of threads specified
  {
    #pragma omp critical(stdout) // Create a critical region to print to stdout
    std::cout << "Hello from secondary thread " << omp_get_thread_num()
      << " of " << omp_get_num_threads() << std::endl; // Print the thread number and the total number of threads
  }
}

#if 0
  #pragma omp parallel num_threads(2)
  {
    if (omp_get_thread_num() == 0) {
      produce();
    } else {
      consume();
    }
  }
#endif
