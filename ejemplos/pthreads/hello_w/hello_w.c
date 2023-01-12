#include <assert.h> // assert. Good for debugging. Validates assumptions.
#include <inttypes.h>  // For PRIu64, etc. For printf format specifiers.
#include <pthread.h> // pthread_create and pthread_join
#include <stdio.h> // printf and fprintf are defined here
#include <stdint.h> // uint64_t is defined here, good for 64-bit integers
#include <stdlib.h> 
#include <unistd.h> // For sysconf, it is the system call that returns the number of processors
 /*
  * @brief: greeting function
  * @param: data - Contains the thread number
  * @return: void
  */
void* greet(void* data);

    /*
     * @brief: Creates threads and makes them greet
     * @param: athread_count - Number of threads to create
     * @return: 0 - Success
     */

int create_threads(uint64_t thread_count); 

// procedure main
int main(int argc, char* argv[]) {
#if 0
  for (int index = 0; index < argc; ++index) {
    printf("argv[%d] = '%s'\n", index, argv[index]);
  }
#endif
  // thread_count := integer(argv[1])
  // Create thread_count as the result of converting argv[1] to integer
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN); // Number of processors on the system, defined in unistd.h
  // assert(argc == 2);
  if (argc == 2) { // If there is an argument
    if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) { // If there's a number in the argument
    } else {
      fprintf(stderr, "error: invalid thread count\n"); // Print error message
      return EXIT_FAILURE;
    }
  }

  int error = create_threads(thread_count); // Create threads 
  return error;
}  // end procedure


int create_threads(uint64_t thread_count) {
  int error = EXIT_SUCCESS;
  // create_thread(greet)
  pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t)); // Allocate memory for an array of thread_count threads
  if (threads) { // If the memory was allocated successfully
    // for thread_number := 0 to thread_count do
    for (uint64_t thread_number = 0; thread_number < thread_count  
        ; ++thread_number) { // For each thread
      // create_thread(greet, thread_number)
      error = pthread_create(&threads[thread_number], /*attr*/ NULL, greet 
        , /*arg*/ (void*)thread_number); // Create a thread
      if (error == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "Error: could no create secondary thread\n"); // Print error message
        error = 22; 
        break; 
      }
    }

    // print("Hello from main thread")
    printf("Hello from main thread\n"); 

    for (uint64_t thread_number = 0; thread_number < thread_count
        ; ++thread_number) { // For each thread
      pthread_join(threads[thread_number], /*value_ptr*/ NULL); //Wait for the thread to finish and join it with the main thread 
    }

    free(threads); // Free the memory allocated for the array of threads
  } else {
    fprintf(stderr, "error: could not allocate %ld threads\n", thread_count); // Prints an error message
    error = 21;
  }
  return error;
}

// procedure greet
void* greet(void* data) {
  const uint64_t thread_number = (uint64_t) data; // Convert data to an integer
  // print("Hello from secondary thread")
  printf("Hello from secondary thread %" PRIu64 "\n", thread_number); //Prints a greeting with the thread number
  return NULL;
}  // end procedure
