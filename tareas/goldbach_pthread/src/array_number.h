// Copyright 2022 Brandon Mora Umana

#ifndef SRC_ARRAY_NUMBER_H_
#define SRC_ARRAY_NUMBER_H_
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>
#include <stddef.h>
#include "array_sums.h"


typedef struct number {
uint64_t number;
bool is_negative;
bool is_out_of_range;
bool is_even;
uint64_t sumNumber;
array_sums_t* sums;
} number_t;


typedef struct {
    int64_t capacity;  /// Capacity of the array
    int64_t count;  /// Number of elements in the array
    uint64_t maxNumber;  /// Maximum number to be evaluated
    number_t* elements;  /// Array of elements
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
