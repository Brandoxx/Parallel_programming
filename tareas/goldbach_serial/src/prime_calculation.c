// Copyright 2022 Brandon Mora Umana
#include "prime_calculation.h"

// procedure prime_numbers_calculation (var max number: 64-bit integer):
// Let prime numbers be a 64 bit unsigned integer array
// Let bool be a Boolean array initially set to all true
// for i = 2 to sqrt(max number):
//    if bool[i] is true
//        for j = (i power 2) to max number by i:
//            if j is divisible by i
//                bool[j] = false
// end for
// for i=2 to max number
//    if bool[i] is true
//        add i to prime numbers
// end for
// end prime_numbers_calculation

int prime_numbers_calculation(int64_t maxNumber,
array_prime_numbers_t* prime_numbers) {  // Adapted from the sieve of
// Eratosthenes algorithm. https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
    assert(prime_numbers);
    int64_t maxNumberSqrt = (int64_t) sqrt(maxNumber) + 1;
    bool* is_prime = calloc(maxNumber + 1, sizeof(bool));
    for (int64_t index = 2; index < maxNumberSqrt; index++) {
        if (!(is_prime[index])) {  // If the number is prime.
            for (int64_t j = pow(index, 2); j < maxNumber; j += index) {
                if (j % index == 0) {  // If the number is a multiple of the
                // prime number.
                    is_prime[j] = true;
                }
            }
        }
    }
    for (int64_t index = 2; index < maxNumber; index++) {
        if (!(is_prime[index])) {  // If the number is prime.
            prime_number_t prime_number = {index, 0};
            array_prime_numbers_append(prime_numbers, prime_number);
        }
    }
    free(is_prime);
    return EXIT_SUCCESS;
}
