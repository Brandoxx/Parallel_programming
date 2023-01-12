// Copyright 2022 Brandon Mora Umana
#include <assert.h>
#include <stdlib.h>

#include "array_prime_numbers.h"
#include "array_number.h"

/**
 * @brief Calculates the Goldbach conjecture for a given number.
 * @details Calculates the Goldbach conjecture for a given number, and stores
 * the result in a variable inside the struct.
 * Calls two private functions, one to calculate even numbers, and one to
 * calculate odd numbers.
 * @param numbers The array of structs of type number_t.
 * @param prime_numbers The array of prime numbers.
 * @return The error code.
 */
int goldbach_calculation(array_number_t* numbers,
array_prime_numbers_t* prime_numbers);
