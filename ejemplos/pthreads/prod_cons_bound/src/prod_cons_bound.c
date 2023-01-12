// Simulates a producer and a consumer that share a bounded buffer

// @see `man feature_test_macros`
#define _DEFAULT_SOURCE

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include <unistd.h>

//we define an enum for error codes
enum {
  ERR_NOMEM_SHARED = EXIT_FAILURE + 1,
  ERR_NOMEM_BUFFER,
  ERR_NO_ARGS,
  ERR_BUFFER_CAPACITY,
  ERR_ROUND_COUNT,
  ERR_MIN_PROD_DELAY,
  ERR_MAX_PROD_DELAY,
  ERR_MIN_CONS_DELAY,
  ERR_MAX_CONS_DELAY,
  ERR_CREATE_THREAD,
};
/**
 * @brief  A bounded buffer
 *
 * @details  The buffer is implemented as a circular array. The buffer is
 *          bounded by its capacity. The buffer is shared between a producer
 *         and a consumer. The producer adds items to the buffer and the
 *        consumer removes items from the buffer.
 */

typedef struct {
  size_t thread_count;
  size_t buffer_capacity;
  double* buffer;
  size_t rounds;
  useconds_t producer_min_delay;
  useconds_t producer_max_delay;
  useconds_t consumer_min_delay;
  useconds_t consumer_max_delay;

  sem_t can_produce;
  sem_t can_consume;
} shared_data_t;

typedef struct  {
  size_t thread_number;
  shared_data_t* shared_data;
} private_data_t;
/**
 * @brief  Analyzes the command line arguments
 * 
 * @param argc 
 * @param argv 
 * @param shared_data 
 * @return error code
 */
 
int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data);
/**
 * @brief  Creates threads and waits for them to finish
 * 
 * @param shared_data of a team of threads
 * @return error code 
 *
 */
int create_threads(shared_data_t* shared_data);
/**
 * @brief The producer thread, produces items and adds them to the buffer
 * 
 * @param data 
 * @return nothing
 */
void* produce(void* data);
/**
 * @brief The consumer thread, removes items from the buffer and consumes them
 * 
 * @param data private data of the thread
 * @return void* 
 */
void* consume(void* data);
useconds_t random_between(useconds_t min, useconds_t max);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;

  shared_data_t* shared_data = (shared_data_t*)
    calloc(1, sizeof(shared_data_t));

  if (shared_data) {
    error = analyze_arguments(argc, argv, shared_data);
    if (error == EXIT_SUCCESS) {
      shared_data->buffer = (double*) //we allocate the buffer 
        calloc(shared_data->buffer_capacity, sizeof(double));
      if (shared_data->buffer) {
        sem_init(&shared_data->can_produce, /*pshared*/ 0
          , shared_data->buffer_capacity); //we initialize the semaphore for producer
        sem_init(&shared_data->can_consume, /*pshared*/ 0, /*value*/ 0); //we initialize the semaphore for consumer

        unsigned int seed = 0u; //we initialize the seed for random number generator
        getrandom(&seed, sizeof(seed), GRND_NONBLOCK);// use entropy as seed for random number generator
        srandom(seed);

        struct timespec start_time;
        clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &start_time);

        error = create_threads(shared_data);

        struct timespec finish_time;
        clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &finish_time);

        double elapsed = (finish_time.tv_sec - start_time.tv_sec) +
          (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
        printf("execution time: %.9lfs\n", elapsed);

        sem_destroy(&shared_data->can_consume); //we destroy the semaphore for consumer
        sem_destroy(&shared_data->can_produce);//we destroy the semaphore for producer
        free(shared_data->buffer);
      } else {
        fprintf(stderr, "error: could not create buffer\n");
        error = ERR_NOMEM_BUFFER;
      }
    }

    free(shared_data);
  } else {
    fprintf(stderr, "Error: could not allocate shared data\n");
    error = ERR_NOMEM_SHARED;
  }

  return error;
}
// here we analyze the arguments passed to the program and we make error checks
int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  if (argc == 7) {
    //we use sscanf to convert the arguments to the correct type
    if (sscanf(argv[1], "%zu", &shared_data->buffer_capacity) != 1
      || shared_data->buffer_capacity == 0) {
        fprintf(stderr, "error: invalid buffer capacity\n");
        error = ERR_BUFFER_CAPACITY;
    } else if (sscanf(argv[2], "%zu", &shared_data->rounds) != 1
      || shared_data->rounds == 0) {
        fprintf(stderr, "error: invalid round count\n");
        error = ERR_ROUND_COUNT;
    } else if (sscanf(argv[3], "%u", &shared_data->producer_min_delay) != 1) {
        fprintf(stderr, "error: invalid min producer delay\n");
        error = ERR_MIN_PROD_DELAY;
    } else if (sscanf(argv[4], "%u", &shared_data->producer_max_delay) != 1) {
        fprintf(stderr, "error: invalid max producer delay\n");
        error = ERR_MAX_PROD_DELAY;
    } else if (sscanf(argv[5], "%u", &shared_data->consumer_min_delay) != 1) {
        fprintf(stderr, "error: invalid min consumer delay\n");
        error = ERR_MIN_CONS_DELAY;
    } else if (sscanf(argv[6], "%u", &shared_data->consumer_max_delay) != 1) {
        fprintf(stderr, "error: invalid max consumer delay\n");
        error = ERR_MAX_CONS_DELAY;
    }
  } else {
    fprintf(stderr, "usage: prod_cons_bound buffer_capacity rounds"
      " producer_min_delay producer_max_delay"
      " consumer_min_delay consumer_max_delay\n");
      error = ERR_NO_ARGS;
  }
  return error;
}

int create_threads(shared_data_t* shared_data) {
  assert(shared_data);
  int error = EXIT_SUCCESS;

  pthread_t producer, consumer;
  error = pthread_create(&producer, /*attr*/ NULL, produce, shared_data);
  if (error == EXIT_SUCCESS) {
    error = pthread_create(&consumer, /*attr*/ NULL, consume, shared_data);
    if (error != EXIT_SUCCESS) {
      fprintf(stderr, "error: could not create consumer\n");
      error = ERR_CREATE_THREAD;
    }
  } else {
    fprintf(stderr, "error: could not create producer\n");
    error = ERR_CREATE_THREAD;
  }

  if (error == EXIT_SUCCESS) {
    pthread_join(producer, /*value_ptr*/ NULL);
    pthread_join(consumer, /*value_ptr*/ NULL);
  }

  return error;
}

void* produce(void* data) {
  // const private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = (shared_data_t*)data;
  size_t count = 0;
  for (size_t round = 0; round < shared_data->rounds; ++round) { //we produce the number of rounds
    for (size_t index = 0; index < shared_data->buffer_capacity; ++index) { //we produce the number of elements in the buffer
      // wait(can_produce)
      sem_wait(&shared_data->can_produce); //we wait until we can produce

      usleep(1000 * random_between(shared_data->producer_min_delay // we wait micro seconds 
        , shared_data->producer_max_delay)); //we wait a random time between the min and max delay
      shared_data->buffer[index] = ++count;//we produce the element
      printf("Produced %lg\n", shared_data->buffer[index]); //we print the element produced

      // signal(can_consume)
      sem_post(&shared_data->can_consume); //we signal to the consumer that they  can consume
    }
  }

  return NULL;
}

void* consume(void* data) {
  // const private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = (shared_data_t*)data; // get the shared data
  for (size_t round = 0; round < shared_data->rounds; ++round) { //we consume the number of rounds
    for (size_t index = 0; index < shared_data->buffer_capacity; ++index) { //we consume the number of elements in the buffer
      // wait(can_consume)
      sem_wait(&shared_data->can_consume);  //we wait until we can consume

      double value = shared_data->buffer[index];  //we consume the element
      usleep(1000 * random_between(shared_data->consumer_min_delay
        , shared_data->consumer_max_delay));  //we wait a random time between the min and max delay in microseconds
      printf("\tConsumed %lg\n", value);  // prints the element consumed
      // signal(can_produce)
      sem_post(&shared_data->can_produce);  // signal to the producer that they can produce
    }
  }

  return NULL;
}
/**
 * @brief Returns a random number between min and max, inclusive.
 * @param min The minimum value.
 * @param max The maximum value.
 * @return A random number between min and max, inclusive.
 */

useconds_t random_between(useconds_t min, useconds_t max) {
  return min + (max > min ? (random() % (max - min)) : 0); //we return a random number between the min and max
}
