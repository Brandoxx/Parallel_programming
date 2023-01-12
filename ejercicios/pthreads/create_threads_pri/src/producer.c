// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#include <stdio.h>

#include "common.h"
#include "producer.h"

void* produce(void* data) {
  // const private_data_t* private_data = (private_data_t*)data;
  thread_data_t* private_data = (thread_data_t*)data;
  simulation_t* simulation = private_data->simulation;
  private_data->self = pthread_self();
  // asuming that the queue have somethig to consume
  while (true) {
    // declare my_unit := 0
    size_t my_unit = 0;
    // lock(can_access_next_unit)
    pthread_mutex_lock(&simulation->can_access_next_unit);
    // if there is a unit to produce
    if (simulation->next_unit < simulation->unit_count) {
      my_unit = ++simulation->next_unit;
    } else {
      // unlock(can_access_next_unit)
      pthread_mutex_unlock(&simulation->can_access_next_unit);
      // break while
      break;
    }
    // unlock(can_access_next_unit)
    pthread_mutex_unlock(&simulation->can_access_next_unit);

    usleep(1000 * random_between(simulation->producer_min_delay
      , simulation->producer_max_delay));
    // queue is thread safe, so we don't need to lock it
    queue_enqueue(&simulation->queue, my_unit);
    printf("Thread %zu Produced %zu\n", private_data->threadNumber, my_unit);

    // signal(can_consume)
    sem_post(&simulation->can_consume);
  }

  return NULL;
}