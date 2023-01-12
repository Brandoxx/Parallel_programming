#include <omp.h>

#include <iostream>
#include <vector>

/// @brief Prints mappping of threads
/// @param label Label to print
/// @param mapping  Mapping to print
void print_mapping(const char* label, const std::vector<int>& mapping) {
  std::cout << label;
  for (size_t index = 0; index < mapping.size(); ++index) {
    std::cout << mapping[index] << (index == mapping.size() - 1 ? '\n' : ' ');
  }
}
/// @brief An example of how to use OpenMP mapping in for loops
/// @param argc  Number of arguments  
/// @param argv  Arguments, number of threads
/// @return  0 if everything is ok
int main(int argc, char* argv[]) {
  int thread_count = omp_get_max_threads();
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }

  int iteration_count = thread_count;
  if (argc >= 3) {
    iteration_count = atoi(argv[2]);
  }

  int block_size = 0; // Block size for the mapping
  if (argc >= 4) { // If the user provides a block size
    block_size = atoi(argv[3]); // Use that block size
  } 

  std::vector<int> mapping(iteration_count); // Mapping of threads to iterations
 
  #pragma omp parallel num_threads(thread_count) \ // Create a parallel region with the number of threads specified
    default(none) shared(iteration_count, mapping, block_size) // default(none) means that all variables are private by default unless specified
    // shared(iteration_count, mapping, block_size) means that iteration_count, mapping and block_size are shared
  {
    if (block_size == 0) { // If the block size is 0
      #pragma omp for schedule(static) // Create a for loop with static scheduling
      for (int iteration = 0; iteration < iteration_count; ++iteration) { // For each iteration
        mapping[iteration] = omp_get_thread_num();
      } 

      #pragma omp single // Create a single region
      print_mapping("static    ", mapping); // Print the mapping 

      #pragma omp for schedule(dynamic) // Create a for loop with dynamic scheduling
      for (int iteration = 0; iteration < iteration_count; ++iteration) { // For each iteration
        mapping[iteration] = omp_get_thread_num(); // Set the mapping
      }

      #pragma omp single // Create a single region
      print_mapping("dynamic   ", mapping); // Print the mapping
 
      #pragma omp for schedule(guided) // Create a for loop with guided scheduling
      for (int iteration = 0; iteration < iteration_count; ++iteration) { // For each iteration
        mapping[iteration] = omp_get_thread_num();
      }

      #pragma omp single // Create a single region
      print_mapping("guided    ", mapping); // Print the mapping
    } else {
      #pragma omp for schedule(static,block_size) // Create a for loop with static scheduling and block size
      for (int iteration = 0; iteration < iteration_count; ++iteration) { // For each iteration
        mapping[iteration] = omp_get_thread_num();
      }
 
      #pragma omp single // Create a single region
      print_mapping("static,N  ", mapping); // Print the mapping

      #pragma omp for schedule(dynamic,block_size) // Create a for loop with dynamic scheduling and block size
      for (int iteration = 0; iteration < iteration_count; ++iteration) {
        mapping[iteration] = omp_get_thread_num();
      }
 
      #pragma omp single // Create a single region
      print_mapping("dynamic,N ", mapping);

      #pragma omp for schedule(guided,block_size) // Create a for loop with guided scheduling and block size
      for (int iteration = 0; iteration < iteration_count; ++iteration) { // For each iteration
        mapping[iteration] = omp_get_thread_num();
      }

      #pragma omp single // Create a single region
      print_mapping("guided,N  ", mapping); 
    }
  }
}
