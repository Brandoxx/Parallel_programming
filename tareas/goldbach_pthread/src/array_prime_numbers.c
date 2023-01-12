// Copyright 2022 Brandon Mora Umana
#include <assert.h>
#include <stdlib.h>

#include "array_prime_numbers.h"

/**
 * @brief Increase the capacity of the array
 * 
 * @param array The array to increase the capacity of
 * @return int EXIT_SUCCESS if the capacity was increased successfully
 */
int array_prime_numbers_increase_capacity(array_prime_numbers_t* array);

int array_prime_numbers_init(array_prime_numbers_t* array) {
    assert(array);
    array->capacity = 0;
    array->count = 0;
    array->elements = NULL;
    return EXIT_SUCCESS;
}


void array_prime_numbers_destroy(array_prime_numbers_t* array) {
    assert(array);
    array->capacity = 0;
    array->count = 0;
    free(array->elements);
}

int array_prime_numbers_append(array_prime_numbers_t* array,
prime_number_t element) {
    assert(array);
    if (array->count == array->capacity) {
        if (array_prime_numbers_increase_capacity(array) != EXIT_SUCCESS) {
            return EXIT_FAILURE;
        }
    }
    array->elements[array->count++] = element;
    return EXIT_SUCCESS;
}

int array_prime_numbers_increase_capacity(array_prime_numbers_t* array) {
    assert(array);
    size_t new_capacity = 10* (array->capacity ? array->capacity : 1);
    prime_number_t* new_elements = (prime_number_t*)
        realloc(array->elements, new_capacity * sizeof(prime_number_t));
    if (new_elements) {
        array->elements = new_elements;
        array->capacity = new_capacity;
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}

int array_prime_numbers_reset_counter(array_prime_numbers_t* array) {
    assert(array);
    for (int64_t i = 0; i < array->count; i++) {
        array->elements[i].timesRepeated = 0;
    }
    return EXIT_SUCCESS;
}
