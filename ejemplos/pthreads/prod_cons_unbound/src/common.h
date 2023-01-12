
#ifndef COMMON_H
#define COMMON_H

#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#include "queue.h"

enum {
  ERR_NOMEM_SHARED = EXIT_FAILURE + 1,
  ERR_NOMEM_BUFFER,
  ERR_NO_ARGS,
  ERR_UNIT_COUNT,
  ERR_PRODUCER_COUNT,
  ERR_CONSUMER_COUNT,
  ERR_MIN_PROD_DELAY,
  ERR_MAX_PROD_DELAY,
  ERR_MIN_CONS_DELAY,
  ERR_MAX_CONS_DELAY,
  ERR_CREATE_THREAD,
};
/**
 * @brief Shared data for all threads, consumers and producers
 * @details This structure is shared between all threads, consumers and producers.
 * It contains the queue, the semaphores and the mutex.
 * As this implementation is unbouded, we should have count of the number of
 * elements in the queue.
 */
typedef struct simulation {
  size_t unit_count;
  size_t producer_count;
  size_t consumer_count;
  useconds_t producer_min_delay;
  useconds_t producer_max_delay;
  useconds_t consumer_min_delay;
  useconds_t consumer_max_delay;

  queue_t queue;
  pthread_mutex_t can_access_next_unit; //protects the next_unit variable
  size_t next_unit;
  sem_t can_consume;
  pthread_mutex_t can_access_consumed_count;
  size_t consumed_count;
} simulation_t;

useconds_t random_between(useconds_t min, useconds_t max);

#endif  // COMMON_H

