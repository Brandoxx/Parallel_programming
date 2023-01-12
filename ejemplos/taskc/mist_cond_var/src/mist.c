#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct {
  unsigned int counter;
  unsigned int max;
  pthread_mutex_t mutex;
  pthread_cond_t cond_var;
} mist_t;

/**
 * @brief Initialize the mist_t structure
 * 
 * @param mist  Pointer to the mist_t structure
 * @param max  Maximum value of the counter
 */
 
void mist_init(mist_t* mist, unsigned max) {
  mist->counter = 0;  // Initialize the counter
  mist->max = max;  // Initialize the maximum value
  pthread_mutex_init(&mist->mutex, NULL);  // Initialize the mutex
  pthread_cond_init(&mist->cond_var, NULL);  // Initialize the condition variable
}

void mist_destroy(mist_t* mist) {  // Destroy the mist_t structure
  mist->counter = 0;  // Reset the counter
  pthread_mutex_destroy(&mist->mutex);  // Destroy the mutex
  pthread_cond_destroy(&mist->cond_var);  // Destroy the condition variable
}
/**
 * @brief Increment the counter
 *      If the counter reaches the maximum value, the condition variable is signaled
 * @param mist  Pointer to the mist_t structure
 */
void mistery(mist_t* mist) {
  pthread_mutex_lock(&mist->mutex);  // Lock the mutex
  ++mist->counter;  // Increment the counter
  if (mist->counter < mist->max) {  // If the counter is less than the maximum value
    // Preferred: while ( pthread_cond_wait(...) != 0 ) /* empty */;
    pthread_cond_wait(&mist->cond_var, &mist->mutex);  // Wait for the condition variable
  } else {  // If the counter is greater than or equal to the maximum value
    mist->counter = 0;  // Reset the counter
    pthread_cond_broadcast(&mist->cond_var);  // Signal the condition variable
  }  // End if
  pthread_mutex_unlock(&mist->mutex);  // Unlock the mutex
} 

static mist_t mist;  // Declare the mist_t structure

/**
 * @brief Thread function for the mistery
 *    The mistery is solved when the counter reaches the maximum value
 * @param data  Pointer to the mist_t structure
 * @return void* 
 */
void* run(void* data) {  
  fprintf(stderr, "%zu: before mist()\n", (size_t)data);  // Print a message
  sleep((unsigned)(size_t)data);  // Sleep for a while
  mistery(&mist);  // Solve the mistery
  fprintf(stderr, "%zu: after mist()\n", (size_t)data);  // Print a message
  return NULL;
} 

/**
 * @brief Main function for the mist_cond_var program
 * 
 * @return int 
 */
int main() {
  mist_init(&mist, 3);  // Initialize the mist_t structure with a maximum value of 3

  pthread_t* workers = malloc(3 * sizeof(pthread_t));  // Allocate memory for the threads
  for (size_t index = 0; index < 3; ++index) {  // For each thread
    pthread_create(&workers[index], NULL, run, (void*)index);  // Create the thread
  }

  for (size_t index = 0; index < 3; ++index) {
    pthread_join(workers[index], NULL);  // Wait for the thread to finish
  }

  mist_destroy(&mist);
  return 0;
}
