// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

// thread_private_data_t
/* @brief Structure to store thread private data.
* @details This structure is used to store thread private data.
*/
typedef struct private {
  uint64_t thread_number; //A certain thread number, varies from thread to thread.
  uint64_t thread_count; // The total number of threads.
} private_data_t;


/* @brief Function to greet from a thread.
* @details This function greets from a thread.
* @param[in] private_data Private data of the thread.
*/

void* greet(void* data);

/* @brief Function that creates a certain number of threads.
* @details This function creates a certain number of threads.
* @param[in] thread_count Number of threads to create.
* @return 0 if successful, error number otherwise.
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
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN); // Number of processors online. 
  // assert(argc == 2);
  if (argc == 2) {
    if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {
    } else {
      fprintf(stderr, "error: invalid thread count\n"); // Print error message to stderr.
      return EXIT_FAILURE;
    }
  }

  int error = create_threads(thread_count); // Create threads. Return 0 if successful, error number otherwise.
  return error;
}  // end procedure


int create_threads(uint64_t thread_count) {
  int error = EXIT_SUCCESS;
  // create_thread(greet)
  pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t)); // Allocate memory for threads. Use malloc to allocate memory.
  private_data_t* private_data = (private_data_t*)
    calloc(thread_count, sizeof(private_data_t)); // Allocate memory for private data. Use calloc to allocate memory and initialize to 0.
  if (threads && private_data) {
    // for thread_number := 0 to thread_count do
    for (uint64_t thread_number = 0; thread_number < thread_count
        ; ++thread_number) {
      private_data[thread_number].thread_number = thread_number; // Set thread number in private data.
      private_data[thread_number].thread_count = thread_count; // Set thread count in private data.
      // create_thread(greet, thread_number)
      error = pthread_create(&threads[thread_number], /*attr*/ NULL, greet
        , /*arg*/ &private_data[thread_number]); // Create thread. Pass private data as argument.
      if (error == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "Error: could no create secondary thread\n");
        error = 22;
        break;
      }
    }

    // print("Hello from main thread")
    printf("Hello from main thread\n");

    for (uint64_t thread_number = 0; thread_number < thread_count
        ; ++thread_number) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL); // Wait for thread to finish. Pass NULL as value_ptr.
    }

    free(private_data); // Free memory for private data.
    free(threads); // Free memory for threads. 
  } else {
    fprintf(stderr, "error: could not allocate %ld threads\n", thread_count);
    error = 21;
  }
  return error;
}

// procedure greet
void* greet(void* data) {
  private_data_t* private_data = (private_data_t*) data; // Cast data to private_data_t*. This is because data is a void*.
  // print("Hello from secondary thread")
  printf("Hello from secondary thread %" PRIu64 " of %" PRIu64 "\n"
    , (*private_data).thread_number, private_data->thread_count); // Print message to stderr.
  return NULL;
}  // end procedure
