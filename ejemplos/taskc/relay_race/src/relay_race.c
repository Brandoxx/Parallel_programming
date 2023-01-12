// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#define _DEFAULT_SOURCE

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Shared data for every thread
typedef struct shared_data {
  size_t team_count; // Number of teams
  useconds_t stage1_duration; // Duration of stage 1
  useconds_t stage2_duration; // Duration of stage 2
  size_t position; // Position of the team
  pthread_barrier_t start_barrier; // The start barrier, like a start signal
  sem_t* batons; // The batons that the teams will use to pass
  pthread_mutex_t finish_mutex; // The mutex to protect the finish variable
} shared_data_t;

typedef struct private_data { // Private data for every thread
  size_t thread_number;  // rank of the thread
  shared_data_t* shared_data; // Shared data
} private_data_t;

int create_threads(shared_data_t* shared_data);
int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data);
/**
 * @brief The function that the threads will execute for stage 1. It will
 * simulate the carrying of the baton. At the end, they'll pass the baton to
 * the next teammate.
 * @param data 
 * @return void* 
 */
void* start_race(void* data);
/**
 * @brief The function that the threads will execute for stage 2. It will
 * simulate the carrying of the baton. At the end, they'll cross the finish
 * line and print the position of the team.
 * 
 * @param data 
 * @return void* 
 */
void* finish_race(void* data);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;

  shared_data_t* shared_data = (shared_data_t*)
    calloc(1, sizeof(shared_data_t));

  if (shared_data) {
    error = analyze_arguments(argc, argv, shared_data);
    if (error == EXIT_SUCCESS) {
      shared_data->position = 0; // Initialize the position to 0
      error = pthread_barrier_init(&shared_data->start_barrier,
        /*attr*/ NULL, shared_data->team_count); // Initialize the start barrier
      shared_data->batons = (sem_t*) calloc(shared_data->team_count
        , sizeof(sem_t)); // Initialize the batons
      pthread_mutex_init(&shared_data->finish_mutex, /*attr*/ NULL);

      if (error == EXIT_SUCCESS && shared_data->batons) {
        // For every baton, initialize it to 0
        for (size_t index = 0; index < shared_data->team_count; ++index) {
          sem_init(&shared_data->batons[index], /*pshared*/ 0, /*value*/ 0);
        }

        struct timespec start_time, finish_time; // To measure the time
        clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &start_time); // Get the start time

        error = create_threads(shared_data); // Create the threads

        clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &finish_time); // Get the finish time
        double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
          (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9; // Calculate the elapsed time
        printf("execution time: %.9lfs\n", elapsed_time); // Print the elapsed time
        // For every baton, destroy it
        for (size_t index = 0; index < shared_data->team_count; ++index) {
          sem_destroy(&shared_data->batons[index]);
        }
        pthread_mutex_destroy(&shared_data->finish_mutex); // Destroy the mutex
        pthread_barrier_destroy(&shared_data->start_barrier); // Destroy the start barrier
      } else {
        fprintf(stderr, "error: could not init mutex\n");
        error = 11;
      }
    }

    free(shared_data);
  } else {
    fprintf(stderr, "error: could not allocated shared memory\n");
    error = 12;
  }

  return error;
}

int analyze_arguments(int argc, char* argv[]
    , shared_data_t* shared_data) {
  if (argc == 4) { // Check if the number of arguments is correct
    if ( sscanf(argv[1], "%zu", &shared_data->team_count) != 1
      || shared_data->team_count == 0 ) { // Check if the team count is valid
      fprintf(stderr, "invalid team count: %s\n", argv[1]);
      return 11;
    }

    if ( sscanf(argv[2], "%u", &shared_data->stage1_duration) != 1 ) { // Check if the stage 1 duration is valid
      fprintf(stderr, "invalid stage 1 duration: %s\n", argv[2]);
      return 12;
    }

    if ( sscanf(argv[3], "%u", &shared_data->stage2_duration) != 1 ) { // Check if the stage 2 duration is valid
      fprintf(stderr, "invalid stage 2 duration: %s\n", argv[3]);
      return 13;
    }
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "usage: relay_race teams stage1duration stage2duration\n");
    return 10;
  }
}

int create_threads(shared_data_t* shared_data) {
  int error = 0;

  const size_t thread_count = 2 * shared_data->team_count;
  pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t));

  private_data_t* private_data = (private_data_t*)
    calloc(thread_count, sizeof(private_data_t));

  if (threads && private_data) {
    for (size_t index = 0; index < shared_data->team_count; ++index) {
      private_data[index].thread_number = index;
      private_data[index].shared_data = shared_data;

      error = pthread_create(&threads[index], NULL, start_race
        , &private_data[index]);

      if (error) {
        fprintf(stderr, "error: could not create thread %zu\n", index);
        error = 21;
      }
    }

    for (size_t index = shared_data->team_count; index < thread_count;
        ++index) {
      private_data[index].thread_number = index;
      private_data[index].shared_data = shared_data;

      error = pthread_create(&threads[index], NULL, finish_race
        , &private_data[index]);

      if (error) {
        fprintf(stderr, "error: could not create thread %zu\n", index);
        error = 21;
      }
    }

    for (size_t index = 0; index < thread_count; ++index) {
      pthread_join(threads[index], NULL);
    }

    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "error: could not allocate memory for %zu threads\n"
      , shared_data->team_count);
    error = 22;
  }

  return error;
}

void* start_race(void* data) {
  private_data_t* private_data = (private_data_t*)data; // Get the private data
  shared_data_t* shared_data = private_data->shared_data; // Get the shared data

  const size_t rank = private_data->thread_number; // Get the rank of the thread
  const size_t team_number = rank; // Get the team number

  pthread_barrier_wait(&shared_data->start_barrier); // Wait for the start barrier
  usleep(1000 * shared_data->stage1_duration); // Sleep for the stage 1 duration
  sem_post(&shared_data->batons[team_number]); // Post the baton
 
  return NULL;
}

void* finish_race(void* data) {
  private_data_t* private_data = (private_data_t*)data; // Get the private data
  shared_data_t* shared_data = private_data->shared_data; // Get the shared data

  const size_t rank = private_data->thread_number; // Get the rank of the thread
  const size_t team_number = rank - shared_data->team_count; // Get the team number
  assert(team_number < shared_data->team_count); // Check if the team number is valid

  // wait(batons[team_number])
  sem_wait(&shared_data->batons[team_number]); // Wait for the baton to be posted
  usleep(1000 * shared_data->stage2_duration); // Sleep for the stage 2 duration

  pthread_mutex_lock(&shared_data->finish_mutex); // Lock the mutex
  const size_t our_position = ++shared_data->position; // Get our position and increment the position
  // if (our_position <= 3) {
    printf("Place %zu: team %zu\n", our_position, team_number); // Print our position
  // }
  pthread_mutex_unlock(&shared_data->finish_mutex); // Unlock the mutex

  return NULL;
}
