// Copyright 2022 Brandon Mora Umana
#include <assert.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>

#include "array_prime_numbers.h"
#include "array_number.h"

typedef struct {
    /**
     * @brief The array of prime numbers.
     * @details The array of prime numbers.
     */
    array_prime_numbers_t* prime_numbers;
    /**
     * @brief The array of numbers.
     * @details The array of numbers.
     */
    array_number_t* numbers;
    uint64_t consumed_numbers;
    /**
     * @brief The mutex.
     * @details The mutex.
     */
    pthread_mutex_t* mutex;
} shared_data_t;

typedef struct {
    /**
     * @brief The thread number.
     * @details The thread number.
     */
    uint64_t thread_number;
    /**
     * @brief Shared data between the threads.
     * @details Shared data between the threads.
     */
    shared_data_t* shared_data;
} thread_data_t;

/**
 * @brief Assigns a number to a thread.
 * @details Assigns a number to a thread, and then calls the goldbach
 * calculation function. This function is used in the thread creation.
 * @param data Contains a thread number and a pointer to a shared data
 * @return Nothing.
 */
void* threadCalculation(void* data);

/**
 * @brief This function finds primes in the range [2, n] and stores them in an 
 * array.
 * @details This uses the sieve od Eratosthenes algorithm. We use a boolean array
 * to mark the numbers that are not prime. We start with the number 2 and mark
 * all its multiples as not prime. We then move to the next number that is not
 * marked as not prime and repeat the process.
 * @param prime_numbers The array with the prime numbers.
 * @param maxNumber The max number inputed by the user.
 * @return Error code.
 */
int prime_numbers_calculation(int64_t maxNumber,
array_prime_numbers_t* prime_numbers);
