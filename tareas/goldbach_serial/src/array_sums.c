// Copyright 2022 Brandon Mora Umana
#include <assert.h>
#include <stdlib.h>

#include "array_sums.h"

/**
 * @brief Increase the capacity of the array
 * 
 * @param array The array to increase the capacity of
 * @return int EXIT_SUCCESS if the capacity was increased successfully
 */
int array_sums_increase_capacity(array_sums_t* array);

int array_sums_init(array_sums_t* array) {
    assert(array);
    array->capacity = 0;
    array->count = 0;
    array->elements = NULL;
    return EXIT_SUCCESS;
}

void array_sums_destroy(array_sums_t* array) {
    assert(array);
    for (int64_t i = 0; i < array->count; i++) {
        free(array->elements[i]);
    }
    array->capacity = 0;
    array->count = 0;
    free(array->elements);
}

int array_sums_append(array_sums_t* array, uint64_t* element) {
    assert(array);
    if (array->count == array->capacity) {
        if (array_sums_increase_capacity(array) != EXIT_SUCCESS) {
            return EXIT_FAILURE;
        }
    }
    array->elements[array->count++] = element;


    return EXIT_SUCCESS;
}

int array_sums_increase_capacity(array_sums_t* array) {
    assert(array);
    size_t new_capacity = 10* (array->capacity ? array->capacity : 1);
    uint64_t** new_elements = (uint64_t**)
        realloc(array->elements, new_capacity * sizeof(uint64_t*));
    if (new_elements) {
        array->elements = new_elements;
        array->capacity = new_capacity;
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}
