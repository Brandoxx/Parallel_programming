#include <iostream>

// Openmp creates parallel regions using pragmas. This regions affect only the
// following line, unless you use curly braces to create a block.

// It's more simple than pthreads, but it's not as flexible as pthreads.
int main() {
  #pragma omp parallel // Create a parallel region
  {
    #pragma omp critical // Create a critical region
    std::cout << "Hello from secondary thread" << std::endl;
  }

  #pragma omp parallel // Creates a region to print
  std::cout << "jiji\n";
}
