// Copyright 2022 Brandon Mora Umana
#ifndef SRC_ARRAY_SUMS_H_
#define SRC_ARRAY_SUMS_H_
#include <stdbool.h>
#include <inttypes.h>
#include <stddef.h>

/// @brief The array of sums.
typedef struct {
    /// @brief Capacity of the array
    int64_t capacity;
    /// @brief Number of elements in the array
    int64_t count;
    /// @brief Array of elements (array of pointers)
    uint64_t** elements;
} array_sums_t;

/**
 * @brief Initialize the array
 * 
 * @param array The array to initialize
 * @return int EXIT_SUCCESS if the array was initialized successfully
 */
int array_sums_init(array_sums_t* array);

/**
 * @brief Destroy the array
 * 
 * @param array The array to destroy
 */
void array_sums_destroy(array_sums_t* array);

/**
 * @brief Append an element to the array
 * 
 * @param array The array to append the element to
 * @param element The element to append
 * @return int EXIT_SUCCESS if the element was appended successfully
 */
int array_sums_append(array_sums_t* array, uint64_t* element);




#endif  // SRC_ARRAY_SUMS_H_
