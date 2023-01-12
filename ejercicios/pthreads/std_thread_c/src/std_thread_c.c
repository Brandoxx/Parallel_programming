// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <assert.h>  // assert
#include <inttypes.h>  // for PRIu64
#include <threads.h>  // c11 threads
#include <stdint.h>  // for uint64_t
#include <stdio.h>  // printf
#include <stdlib.h>  // malloc
#include <time.h>  // clock_gettime
#include <unistd.h>  // sleep

// thread_shared_data_t
typedef struct shared_data {  // shared data, threads share this data structure,
// and each thread can read and write
  uint64_t thread_count;
} shared_data_t;

// thread_private_data_t
typedef struct private_data {  // private data, each thread has its own
// private data structure
  uint64_t thread_number;  // rank
  shared_data_t* shared_data;
} private_data_t;

/**
 * @brief ...
 */
int greet(void* data);
int create_threads(shared_data_t* shared_data);

// procedure main(argc, argv[])
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  // create thread_count as result of converting argv[1] to integer
  // thread_count := integer(argv[1])
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc == 2) {
    if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {
    } else {
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }
  }
  // create shared_data, and initialize it. shared_data is a pointer to
  // a shared_data_t
  shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
  if (shared_data) {  // if shared_data is not null
    shared_data->thread_count = thread_count;  // shared_data.thread_count :=
    // thread_count

    struct timespec start_time, finish_time;  // create start_time and finish_
    // time as timespec
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    error = create_threads(shared_data);  // create a thread, passing shared_
    // data as argument

    clock_gettime(CLOCK_MONOTONIC, &finish_time);  // get duration, and store
    // it in finish_time
    double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
      (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;  // elapsed_time :=
      // finish_time - start_time

    printf("Execution time: %.9lfs\n", elapsed_time);  // print elapsed_time,
    // the execution time

    free(shared_data);  // free shared_data
  } else {
    fprintf(stderr, "Error: could not allocate shared data\n");
    return 12;
  }
  return error;
}  // end procedure



int create_threads(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  // for thread_number := 0 to thread_count do
  thrd_t* threads = (thrd_t*)
    malloc(shared_data->thread_count * sizeof(thrd_t));  // create threads
    // as array of pthread_t
  private_data_t* private_data = (private_data_t*)
    calloc(shared_data->thread_count, sizeof(private_data_t));  // create
    // private
    // data as array of private_data_t
  if (threads && private_data) {  // if threads and private_data are not null
    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {  // for thread_number := 0 to thread_count do
      private_data[thread_number].thread_number = thread_number;  // change
      // private_data[thread_number].thread_number
      private_data[thread_number].shared_data = shared_data;  // change private
      // data[thread_number].shared_data
      // create_thread(greet, thread_number)

      error = thrd_create(&threads[thread_number], greet
        , /*arg*/ &private_data[thread_number]);  // create a thread, passing
        // greet as function, and thread_number as argument
      if (error == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "Error: could not create secondary thread\n");
        error = 21;
        break;
      }
    }

    // print "Hello from main thread"
    printf("Hello from main thread\n");  // print "Hello from main thread"

    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      thrd_join(threads[thread_number], /*value_ptr*/ NULL);  // wait for
      // thread_number to finish
    }

    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n"
      , shared_data->thread_count);
    error = 22;
  }

  return error;
}

// procedure greet:
int greet(void* data) {
  // assert(data);
  private_data_t* private_data = (private_data_t*) data;  // private_data :=
  // data
  shared_data_t* shared_data = private_data->shared_data;  // shared_data :=
  // private_data.shared_data

  // print "Hello from secondary thread"
  printf("Hello from secondary thread %" PRIu64 " of %" PRIu64 "\n"
    , private_data->thread_number, shared_data->thread_count);  // print "Hello
    // from secondary thread" using shared_data.thread_count
  return EXIT_SUCCESS;
}  // end procedure