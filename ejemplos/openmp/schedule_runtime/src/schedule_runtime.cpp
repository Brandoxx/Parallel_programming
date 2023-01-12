#include <omp.h>

#include <iostream>
#include <vector>

void print_mapping(const char* label, const std::vector<int>& mapping) {
  std::cout << label;
  for (size_t index = 0; index < mapping.size(); ++index) {
    std::cout << mapping[index] << (index == mapping.size() - 1 ? '\n' : ' ');
  }
}
/// @brief An example of how to use OpenMP mapping in for loops at runtime
/// @param argc 
/// @param argv 
/// @return 
int main(int argc, char* argv[]) {
  int thread_count = omp_get_max_threads();
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }

  int iteration_count = thread_count;
  if (argc >= 3) {
    iteration_count = atoi(argv[2]);
  }

  std::vector<int> mapping(iteration_count);

  #pragma omp parallel for num_threads(thread_count) schedule(runtime) \ // Create a parallel region with the number of threads specified and runtime scheduling
    default(none) shared(iteration_count, mapping) // default(none) means that all variables are private by default unless specified
  for (int iteration = 0; iteration < iteration_count; ++iteration) { // For each iteration
    mapping[iteration] = omp_get_thread_num();
  }

  print_mapping("runtime    ", mapping); // Print the mapping
}
