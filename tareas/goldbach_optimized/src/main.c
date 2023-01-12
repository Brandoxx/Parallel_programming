// Copyright 2022 Brandon Mora Umana
#define _POSIX_C_SOURCE 199309L

#include <assert.h>
#include <time.h>
#include <errno.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "array_number.h"
#include "array_sums.h"
#include "array_prime_numbers.h"
#include "calculation.h"
#include "user_interaction.h"

/**
 * @brief Joins all the threads.
 * @details Joins all the threads and frees the memory allocated for the threads.
 * @param threads The array of threads.
 * @param thread_data The array of thread data.
 * @param numberOfThreads The number of threads.
 * @return Error code.
 */
int join_threads(pthread_t* threads, thread_data_t* thread_data,
uint64_t numberOfThreads);
/**
 * @brief Creates all the threads.
 * @details Creates all the threads and enqueues the numbers to be calculated.
 * @param numbers The array of numbers.
 * @param prime_numbers The array of prime numbers.
 * @param numberOfThreads The number of threads.
 * @return Error code.
 */
int create_threads(array_number_t* numbers, uint64_t numberOfThreads,
array_prime_numbers_t* prime_numbers);

// call prime_numbers_calculation(max_number)
// call goldbach_calculation(Numbers, prime numbers)
// call print_numbers (Numbers)
// end main
// procedure main:
int main(int argc, char** argv) {
int error = EXIT_SUCCESS;
uint64_t threadNumber = __sysconf(_SC_NPROCESSORS_ONLN);
if (argc == 2) {
    threadNumber = strtoull(argv[1], NULL, 10);
    if (errno == ERANGE) {
        fprintf(stderr, "Error: The number of threads is too big. ");
    } else if (errno == EINVAL) {
        fprintf(stderr, "Error: The number of threads is not a number. ");
    }
}
// Let Numbers be a dynamic 64-bit unsigned integer array
array_number_t* numbers = (array_number_t*) malloc(sizeof(array_number_t));
if (numbers) {
    error = array_number_init(numbers);
    if (error == EXIT_SUCCESS) {
        // time measurement in seconds
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        // call read_numbers(Numbers)
        error = read_numbers(numbers);
        if (error == EXIT_SUCCESS) {
            array_prime_numbers_t* prime_numbers = (array_prime_numbers_t*)
             malloc(sizeof(array_prime_numbers_t));
            array_prime_numbers_init(prime_numbers);
            error = prime_numbers_calculation(numbers->maxNumber,
            prime_numbers);
            if (error == EXIT_SUCCESS) {
                error = create_threads(numbers, threadNumber, prime_numbers);
                if (error == EXIT_SUCCESS) {
                    // print_result(numbers);
                }
            }
            array_prime_numbers_destroy(prime_numbers);
            free(prime_numbers);
        }
    }
    array_number_destroy(numbers);
}
free(numbers);
return error;
}

// procedure create_threads(Numbers, numberOfThreads):
int create_threads(array_number_t* numbers, uint64_t numberOfThreads,
array_prime_numbers_t* prime_numbers) {
    assert(numbers);
    int error = EXIT_SUCCESS;
    shared_data_t* shared_data = (shared_data_t*) malloc(sizeof(shared_data_t));
    // shared_data->numbers = Numbers
    shared_data->prime_numbers = prime_numbers;
    shared_data->numbers = numbers;
    shared_data->consumed_numbers = 0;
    shared_data->mutex = (pthread_mutex_t*)
    malloc(sizeof(pthread_mutex_t));  // Declare mutex
    pthread_mutex_init(shared_data->mutex, NULL);  // Initialize mutex
    if (shared_data) {
        // Let threads be a dynamic pthread_t array
        pthread_t* threads = (pthread_t*)
        malloc(numberOfThreads * sizeof(pthread_t));
        thread_data_t* thread_data = (thread_data_t*)
        malloc(numberOfThreads * sizeof(thread_data_t));
        if (threads && thread_data) {
            // for i from 0 to numberOfThreads - 1:
            for (uint64_t i = 0; i < numberOfThreads; i++) {
                thread_data[i].thread_number = i;
                thread_data[i].shared_data = shared_data;
                //     create a thread
                //     call threadCalculation(Numbers)
                error = pthread_create(&threads[i], NULL,
                threadCalculation, &thread_data[i]);
                if (error != EXIT_SUCCESS) {
                    fprintf(stderr, "Error: The thread could not be created. ");
                    break;
                }
            }
            join_threads(threads, thread_data, numberOfThreads);
        } else {
            error = EXIT_FAILURE;
        }
    }
    // end create_threads
    return error;
}

// procedure join_threads(threads, threadData, numberOfThreads):
int join_threads(pthread_t* threads, thread_data_t* thread_data,
uint64_t numberOfThreads) {
    int error = EXIT_SUCCESS;
    // for i from 0 to threads length - 1:
    for (uint64_t i = 0; i < numberOfThreads; i++) {
        // join threads[i]
        error = pthread_join(threads[i], NULL);
    }
    // end for
    // free threads
    free(threads);
    // Destroy mutex
    pthread_mutex_destroy(thread_data->shared_data->mutex);
    // free mutex
    free(thread_data->shared_data->mutex);
    free(thread_data->shared_data);
    // free threadData
    free(thread_data);
    // end join_thread
    return error;
}
