// Copyright 2022 Brandon Mora Umana

#ifndef SRC_ARRAY_NUMBER_H_
#define SRC_ARRAY_NUMBER_H_
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>
#include <stddef.h>
#include "array_sums.h"


typedef struct number {
/// @brief The number.
uint64_t number;
/// @brief A boolean that indicates if the number is negative.
bool is_negative;
/// @brief A boolean that indicates if the number is out of range.
bool is_out_of_range;
/// @brief A boolean that indicates if the number is even
bool is_even;
/// @brief Number of sums.
uint64_t sumNumber;
/// @brief The array of sums.
array_sums_t* sums;
} number_t;


/// @brief The array of numbers.
typedef struct {
    /// @brief Capacity of the array.
    int64_t capacity;
    /// @brief Number of elements in the array.
    int64_t count;
    /// @brief Maximum number to be evaluated.
    uint64_t maxNumber;
    /// @brief The array of elements.
    number_t* elements;
} array_number_t;

/**
 * @brief Initialize the array
 * 
 * @param array The array to initialize
 * @return int EXIT_SUCCESS if the array was initialized successfully
 */
int array_number_init(array_number_t* array);
 /**
  * @brief Destroy the array
  * 
  * @param array The array to destroy
  */
void array_number_destroy(array_number_t* array);

/**
 * @brief Append an element to the array
 * 
 * @param array The array to append the element to
 * @param element The element to append
 * @return int EXIT_SUCCESS if the element was appended successfully
 */
int array_number_append(array_number_t* array, number_t element);




#endif  // SRC_ARRAY_NUMBER_H_
