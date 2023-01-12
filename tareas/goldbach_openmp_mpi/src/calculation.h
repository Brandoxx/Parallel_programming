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
 * @brief Calculates the Goldbach conjecture for a given number.
 * @details Calculates the Goldbach conjecture for a given number, and stores
 * the result in a variable inside the struct.
 * Calls two private functions, one to calculate even numbers, and one to
 * calculate odd numbers.
 * @param number An number to calculate the Goldbach conjecture for.
 * @param prime_numbers The array of prime numbers.
 * @return The error code.
 */
int goldbach_calculation(number_t* number,
array_prime_numbers_t* prime_numbers);

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

/**
 * @brief This function calculates the Goldbach conjecture every number in the
 * array.
 * 
 * @param numbers The array of numbers.
 * @param prime_numbers The array of prime numbers.
 * @param threads The number of threads to use.
 */
void calculateGoldbach(array_number_t* numbers, array_prime_numbers_t*
    prime_numbers, uint64_t threads);
