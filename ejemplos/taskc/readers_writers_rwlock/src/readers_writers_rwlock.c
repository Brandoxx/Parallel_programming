// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#define _DEFAULT_SOURCE

#include <assert.h>
#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Structure for the readers-writers lock
 * 
 */
typedef struct shared_data {  // Structure for the readers-writers lock
  pthread_rwlock_t can_access_counter;  // Readers-writers lock
  size_t counter;  // Counter
} shared_data_t;  // End of the structure for the readers-writers lock 

void* reader(void* data);  // Thread function for the reader
void* writer(void* data);  // Thread function for the writer
int create_threads(shared_data_t* shared_data);  // Create the threads

int main() {  // Main function
  int error = EXIT_SUCCESS;  // Initialize the error code
  shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));  // Allocate memory for the shared data
  if (shared_data) {  // If the memory was allocated
    error = pthread_rwlock_init(&shared_data->can_access_counter, /*attr*/NULL);  // Initialize the readers-writers lock
    if (error == EXIT_SUCCESS) {  // If the readers-writers lock was initialized
      error = create_threads(shared_data);  // Create the threads
      pthread_rwlock_destroy(&shared_data->can_access_counter);  // Destroy the readers-writers lock
    } else {  // If the readers-writers lock was not initialized
      fprintf(stderr, "error: could not create rwlock\n");  // Print an error message
      error = 11;  // Set the error code
    }

    free(shared_data);  // Free the memory
  } else {  // If the memory was not allocated
    fprintf(stderr, "error: could not allocated shared memory\n");  // Print an error message
    error = 12;  // Set the error code
  }  // End if

  return error;
}

int resize(void** array, size_t* capacity, const size_t count, size_t elem_sz) {  // Resize the array
  if (count < *capacity) {  // If the array is not full
    return EXIT_SUCCESS;  // Return success
  }
  const size_t new_capacity = 10 * (*capacity ? *capacity : 1);  // Calculate the new capacity
  void* new_arr = realloc(*array, new_capacity * elem_sz);  // Allocate memory for the new array
  if (new_arr == NULL) {  // If the memory was not allocated
    return EXIT_FAILURE;  // Return failure
  }
  *capacity = new_capacity;   // Set the new capacity
  *array = new_arr;  // Set the new array
  return EXIT_SUCCESS;  // Return success
}  // End of the function

/**
 * @brief Create the threads for the readers-writers lock
 * 
 * @param shared_data 
 * @return int 
 */
int create_threads(shared_data_t* shared_data) {  // Create the threads
  int error = EXIT_SUCCESS;  // Initialize the error code
  size_t capacity = 0;  // Capacity of the array
  size_t count = 0;  // Number of elements in the array
  pthread_t* threads = NULL;  // Array of threads
  char ch = '\0';  // Character read from the standard input
  while (scanf("%c", &ch) == 1) {  // While there is a character to read
    error = resize((void**)&threads, &capacity, count, sizeof(pthread_t));  // Resize the array
    if (error) {  // If the array was not resized
      fprintf(stderr, "error: could not allocate memory for threads\n");  // Print an error message
      error = 22;  // Set the error code
      break;  // Break the loop
    }

    if (toupper(ch) == 'R') {  // If the character is 'R'
      error = pthread_create(&threads[count++], NULL, reader, shared_data); // Create the reader thread
    } else if (toupper(ch) == 'W') {  // If the character is 'W'
      error = pthread_create(&threads[count++], NULL, writer, shared_data);  // Create the writer thread
    }  // Silently ignoring other chars

    if (error) {  // If the thread was not created
      fprintf(stderr, "error: could not create thread %zu\n", count);  // Print an error message
      error = 21;  // Set the error code
      break;  // Break the loop
    }
  }

  for (size_t index = 0; index < count; ++index) {  // For each thread
    pthread_join(threads[index], NULL);  // Wait for the thread to finish
  }

  free(threads);
  return error;
}
/**
 * @brief Thread function for the reader 
 * 
 * @param data 
 * @return void* 
 */
void* reader(void* data) {
  shared_data_t *shared_data = (shared_data_t*)data;

  pthread_rwlock_rdlock(&shared_data->can_access_counter);  // Lock the readers-writers lock
  size_t value = shared_data->counter;  // Read the counter
  pthread_rwlock_unlock(&shared_data->can_access_counter);  // End of the critical section

  printf("Reader got %zu\n", value);  // Print the value
  return NULL;
}

/**
 * @brief Thread function for the writer
 * 
 * @param data  shared data
 * @return void* 
 */
void* writer(void* data) {  // Thread function for the writer
  shared_data_t *shared_data = (shared_data_t*)data;  // Cast the data to the shared data type
  pthread_rwlock_wrlock(&shared_data->can_access_counter);  // Lock the readers-writers lock
  size_t value = ++shared_data->counter;  // Increment the counter
  pthread_rwlock_unlock(&shared_data->can_access_counter);  // End of the critical section
  printf("Writer increased to %zu\n", value);  // Print the value
  return NULL;
}
