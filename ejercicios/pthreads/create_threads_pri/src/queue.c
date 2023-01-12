// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#include <assert.h>
#include <stdlib.h>

#include "queue.h"

void queue_remove_first_unsafe(queue_t* queue);
bool queue_is_empty_unsafe(queue_t* queue);

// Not thread safe, because the mutex is not initialized
int queue_init(queue_t* queue) {
  assert(queue);
  // initialize the mutex
  int error = pthread_mutex_init(&queue->can_access_queue, /*attr*/ NULL);
  queue->head = NULL;
  queue->tail = NULL;
  return error;
}
// Not thread safe, because the mutex is destroyed
int queue_destroy(queue_t* queue) {
  queue_clear(queue); // remove all nodes and free memory
  return pthread_mutex_destroy(&queue->can_access_queue); //destroy the mutex
}

bool queue_is_empty(queue_t* queue) {
  assert(queue);
  //we use mutex to check if we have exclusive access to the queue
  pthread_mutex_lock(&queue->can_access_queue); // lock the mutex
  bool result = queue->head == NULL;
  pthread_mutex_unlock(&queue->can_access_queue);// unlock the mutex
  return result;
}

bool queue_is_empty_unsafe(queue_t* queue) {
  assert(queue);
  return queue->head == NULL;
}

int queue_enqueue(queue_t* queue, const size_t data) {
  assert(queue);
  int error = EXIT_SUCCESS;
  // we give memory to the new node
  queue_node_t* new_node = (queue_node_t*) calloc(1, sizeof(queue_node_t));
  if (new_node) {
    new_node->data = data; // we put the data in the new node
    // we use mutex to check if we have exclusive access to the queue
    pthread_mutex_lock(&queue->can_access_queue); // lock the queue
    if (queue->tail) {
      queue->tail = queue->tail->next = new_node;
    } else {
      queue->head = queue->tail = new_node;
    }
    pthread_mutex_unlock(&queue->can_access_queue); // unlock the mutex
  } else {
    error = EXIT_FAILURE;
  }

  return error;
}

int queue_dequeue(queue_t* queue, size_t* data) {
  assert(queue);
  int error = 0;

  pthread_mutex_lock(&queue->can_access_queue); // lock the queue
  if (!queue_is_empty_unsafe(queue)) {
    if (data) {
      *data = queue->head->data;
    }
    queue_remove_first_unsafe(queue); // remove the first node
  } else {
    error = EXIT_FAILURE;
  }
  pthread_mutex_unlock(&queue->can_access_queue);

  return error;
}
/**
 * @brief Remove the first node from the queue, not thread safe
 * @param queue The queue
 */
 
void queue_remove_first_unsafe(queue_t* queue) {
  assert(queue);
  assert(!queue_is_empty_unsafe(queue)); // the queue must not be empty
  // but it uses the unsafe 
  queue_node_t* node = queue->head;
  queue->head = queue->head->next;
  free(node);
  if (queue->head == NULL) {
    queue->tail = NULL;
  }
}

void queue_clear(queue_t* queue) {
  assert(queue);
  pthread_mutex_lock(&queue->can_access_queue); // lock the mutex
  while (!queue_is_empty_unsafe(queue)) {
    queue_remove_first_unsafe(queue);
  }
  pthread_mutex_unlock(&queue->can_access_queue); // unlock the mutex
}

