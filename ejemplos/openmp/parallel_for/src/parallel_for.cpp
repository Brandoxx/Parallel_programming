#include <omp.h>

#include <iostream>
/// @brief Example of how to use OpenMP to parallelize a for loop
/// @param argc Number of arguments
/// @param argv  Arguments, number of threads
/// @return  0 if everything is ok
int main(int argc, char* argv[]) {
  int thread_count = omp_get_max_threads();
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }

  int iteration_count = thread_count; // Number of iterations
  if (argc >= 3) { // If the user provides a number of iterations
    iteration_count = atoi(argv[2]); // Use that number of iterations
  }

  #pragma omp parallel for num_threads(thread_count) default(none) \
    shared(iteration_count, std::cout) // Creates a parallel region for the for loop with the number of threads specified
    // default(none) means that all variables are private by default unless specified
    // shared(iteration_count, std::cout) means that iteration_count and std::cout are shared
  for (int iteration = 0; iteration < iteration_count; ++iteration) {
    #pragma omp critical(stdout) // Create a critical region to print to stdout
    std::cout << omp_get_thread_num() << '/' << omp_get_num_threads() <<
      ": iteration " << iteration << '/' << iteration_count << std::endl;
  }
}
