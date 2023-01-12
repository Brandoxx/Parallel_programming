// Copyright 2022 Brandon Mora Umana

#ifndef SRC_ARRAY_PRIME_NUMBERS_H_
#define SRC_ARRAY_PRIME_NUMBERS_H_
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>
#include <stddef.h>

typedef struct prime_number {
/// @brief The prime number
uint64_t number;
/// @brief The index of the prime number
uint64_t timesRepeated;  /// The number of times the prime number is repeated
} prime_number_t;

typedef struct {
    /// @brief Capacity of the array
    int64_t capacity;
    /// @brief Number of elements in the array
    int64_t count;
    /// @brief Array of elements
    prime_number_t* elements;
    /// @brief The crive boolean array
    bool* crive;
} array_prime_numbers_t;

/**
 * @brief Initialize the array
 * 
 * @param array The array to initialize
 * @return int EXIT_SUCCESS if the array was initialized successfully
 */
int array_prime_numbers_init(array_prime_numbers_t* array);

/**
 * @brief Destroy the array
 * 
 * @param array The array to destroy
 */
void array_prime_numbers_destroy(array_prime_numbers_t* array);

/**
 * @brief Append an element to the array
 * 
 * @param array The array to append the element to
 * @param element The element to append
 * @return int EXIT_SUCCESS if the element was appended successfully
 */
int array_prime_numbers_append(array_prime_numbers_t* array,
 prime_number_t element);

/**
 * @brief Reset the counter of the elements in the array
 * 
 * @param array The array to reset the counter of the elements
 * 
 * @return int EXIT_SUCCESS if the counter was reset successfully
 */
int array_prime_numbers_reset_counter(array_prime_numbers_t* array);



#endif  // SRC_ARRAY_PRIME_NUMBERS_H_
