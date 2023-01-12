// Copyright 2022 Brandon Mora Umana
#define _POSIX_C_SOURCE 199309L

#include <assert.h>
#include <errno.h>
#include <time.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "array_number.h"
#include "array_sums.h"
#include "array_prime_numbers.h"
#include "goldbach_calculation.h"
#include "prime_calculation.h"
#include "user_interaction.h"

// procedure main:
// Let Numbers be a dynamic 64-bit unsigned integer array
// call read_numbers(Numbers)
// Let prime_numbers be a dynamic 64-bit unsigned integer array
// call prime_numbers_calculation(max_number)
// call goldbach_calculation(Numbers, prime numbers)
// call print_numbers (Numbers)
// end main

int main(void) {
int error = EXIT_SUCCESS;
array_number_t* numbers = malloc(sizeof(array_number_t));
if (numbers) {
    error = array_number_init(numbers);
    if (error == EXIT_SUCCESS) {
        error = read_numbers(numbers);
        if (error == EXIT_SUCCESS) {
            // Get time before prime numbers calculation
            struct timespec start_time;
            error = clock_gettime(CLOCK_MONOTONIC, &start_time);
            array_prime_numbers_t* prime_numbers =
             malloc(sizeof(array_prime_numbers_t));
            array_prime_numbers_init(prime_numbers);
            error = prime_numbers_calculation(numbers->maxNumber,
            prime_numbers);
            if (error == EXIT_SUCCESS) {
                error = goldbach_calculation(numbers, prime_numbers);
                if (error == EXIT_SUCCESS) {
                    array_prime_numbers_destroy(prime_numbers);
                    free(prime_numbers);
                }
            }
            // Get time after prime numbers calculation
            struct timespec end_time;
            error = clock_gettime(CLOCK_MONOTONIC, &end_time);
            //time in nanoseconds
            double elapsed_time = (end_time.tv_sec - start_time.tv_sec);
            printf("Elapsed time: %f seconds\n", elapsed_time/1000000);
            print_result(numbers);
        }
    }
    array_number_destroy(numbers);
}
free(numbers);
return error;
}
