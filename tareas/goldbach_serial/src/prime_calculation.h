// Copyright 2022 Brandon Mora Umana
#ifndef SRC_PRIME_CALCULATION_H_
#define SRC_PRIME_CALCULATION_H_

#include <assert.h>
#include <stdlib.h>
#include <math.h>

#include "array_prime_numbers.h"

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

#endif  // SRC_PRIME_CALCULATION_H_
