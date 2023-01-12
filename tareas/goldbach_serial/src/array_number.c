// Copyright 2022 Brandon Mora Umana
#include <assert.h>
#include <stdlib.h>

#include "array_number.h"

/**
 * @brief Increase the capacity of the array
 * 
 * @param array The array to increase the capacity of
 * @return int EXIT_SUCCESS if the capacity was increased successfully
 */
int array_number_increase_capacity(array_number_t* array);

int array_number_init(array_number_t* array) {
    assert(array);
    array->capacity = 0;
    array->count = 0;
    array->maxNumber = 0;
    array->elements = NULL;
    return EXIT_SUCCESS;
}


void array_number_destroy(array_number_t* array) {
    assert(array);
    for (int64_t i = 0; i < array->count; i++) {
        array_sums_destroy(array->elements[i].sums);
        free(array->elements[i].sums);
    }
    array->capacity = 0;
    array->count = 0;
    array->maxNumber = 0;
    free(array->elements);
}

int array_number_append(array_number_t* array, number_t element) {
    assert(array);
    if (element.number > array->maxNumber) {
        array->maxNumber = element.number;
    }
    if (array->count == array->capacity) {
        if (array_number_increase_capacity(array) != EXIT_SUCCESS) {
            return EXIT_FAILURE;
        }
    }
    array->elements[array->count++] = element;
    return EXIT_SUCCESS;
}

int array_number_increase_capacity(array_number_t* array) {
    assert(array);
    size_t new_capacity = 10* (array->capacity ? array->capacity : 1);
    number_t* new_elements = (number_t*)
        realloc(array->elements, new_capacity * sizeof(number_t));
    if (new_elements) {
        array->elements = new_elements;
        array->capacity = new_capacity;
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}
