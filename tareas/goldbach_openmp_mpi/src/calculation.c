// Copyright 2022 Brandon Mora Umana
#include "calculation.h"

/**
 * @brief This function calculates the goldbach conjecture for odd numbers.
 * @details This function calculates the goldbach conjecture for odd numbers.
 * Calls the goldbach_odd_append function to aid in the calculation.
 * @param numberToCheck The number to check.
 * @param prime_numbers The array with the prime numbers.
 * @return Error code.
 */
int goldbach_odd_calculation(number_t* numberToCheck,
array_prime_numbers_t* prime_numbers);

/**
 * @brief This function calculates the goldbach conjecture for even numbers.
 * @details This function calculates the goldbach conjecture for even numbers.
 * @param numberToCheck The number to check.
 * @param prime_numbers The array with the prime numbers.
 * @return Error code.
 */
int goldbach_even_calculation(number_t* numberToCheck,
array_prime_numbers_t* prime_numbers);

/**
 * @brief This function checks if a number is prime.
 * @details This function checks if a number is prime.
 * @param numberToCheck The number to check.
 * @param prime_numbers The array with the prime numbers.
 * @return True if the number is prime, false otherwise.
 */
bool is_prime(uint64_t numberToCheck,
array_prime_numbers_t* prime_numbers) {
    return !prime_numbers->crive[numberToCheck];
}

void calculateGoldbach(array_number_t* numbers, array_prime_numbers_t*
    prime_numbers, uint64_t threads) {
    assert(numbers != NULL);
    assert(prime_numbers != NULL);
    #pragma omp parallel for schedule(dynamic) default(none) shared(numbers, \
     prime_numbers) num_threads(threads)
    for (int number = 0; number < numbers->count; number++) {
        number_t* myNumber = &numbers->elements[number];
        goldbach_calculation(myNumber, prime_numbers);
    }
}

// procedure goldbach_odd_calculation (var Number : 64-bit integer,
// var prime numbers : dynamic 64-bit unsigned integer array):
int goldbach_odd_calculation(number_t* numberToCheck,
array_prime_numbers_t* prime_numbers) {
    array_sums_t* sums = malloc(sizeof(array_sums_t));
    array_sums_init(sums);
    numberToCheck->sums = sums;
    // Let primeRange be Number/2
    uint64_t primeRange = numberToCheck->number / 2;
    // Let firstAddend be 0
    uint64_t firstAddendCounter = 0;
    // Let secondAddend be 0
    uint64_t secondAddendCounter = 0;
    // while firstAddend < primeRange
    while (prime_numbers->elements[firstAddendCounter].number < primeRange) {
    //     Let sum be firstAddend + secondAddend
        uint64_t sum = prime_numbers->elements[firstAddendCounter].number +
        prime_numbers->elements[secondAddendCounter].number;
    //     let difference be Number - sum
        uint64_t difference = numberToCheck->number - sum;
        if (difference < prime_numbers->elements[firstAddendCounter].number
        || difference < prime_numbers->elements[secondAddendCounter].number) {
            firstAddendCounter++;
            secondAddendCounter = firstAddendCounter;
            continue;
        }
    //     if difference is prime
        if (is_prime(difference, prime_numbers)) {
    //         if Number is negative
            if (numberToCheck->is_negative) {
                uint64_t* sum = malloc(3 * sizeof(uint64_t));
                if (sum) {
                    sum[0] = prime_numbers->elements[firstAddendCounter].number;
                    sum[1] = prime_numbers->
                    elements[secondAddendCounter].number;
                    sum[2] = difference;
                    array_sums_append(numberToCheck->sums, sum);
                } else {
                    fprintf(stderr,
                        "Error: memory allocation failed for sums array");
                    return EXIT_FAILURE;
                }
            }
    //      Number.sumNumber++
            numberToCheck->sumNumber++;
    //     end if
        }
    //     secondAddend increased to next prime number
        secondAddendCounter++;
    //     if secondAddend is greater than primeRange
        if (prime_numbers->elements[secondAddendCounter].number >= primeRange) {
    //         firstAddend increased to next prime number
            firstAddendCounter++;
    //         secondAddend = firstAddend
            secondAddendCounter = firstAddendCounter;
    //     end if
        }
    //     if firstAddend = 2 and secondAddend != 2
        if (firstAddendCounter == 0 && secondAddendCounter != 0) {
    //         firstAddend increased to next prime number
            firstAddendCounter++;
    //         secondAddend = firstAddend
            secondAddendCounter = firstAddendCounter;
    //     end if
        }
    // end while
    }
    // end procedure
    return EXIT_SUCCESS;
}
// end odd_calculation


// procedure goldbach_odd_calculation (var Number : 64-bit integer,
int goldbach_even_calculation(number_t* numberToCheck,
array_prime_numbers_t* prime_numbers) {
    assert(numberToCheck);
    assert(prime_numbers);
    array_sums_t* sums = malloc(sizeof(array_sums_t));
    array_sums_init(sums);
    numberToCheck->sums = sums;
    // Create new variables for readability
    uint64_t targetNumber = numberToCheck->number;
    uint64_t primeSize = prime_numbers->count;
    // Let primeRange be Number/2
    uint64_t primeRange = targetNumber / 2;
    // Let firstAddend be 0
    uint64_t firstAddendCounter = 0;
    // Let secondAddend be prime number size
    uint64_t secondAddendCounter = primeSize - 1;
    uint64_t firstAddend = prime_numbers->elements[firstAddendCounter].number;
    uint64_t secondAddend = prime_numbers->elements[secondAddendCounter].number;
    // while firstAddend is less than primeRange
    while (firstAddend <= primeRange) {  // While the first addend is less than
    // the prime range
        // Let sum be firstAddend + secondAddend
        uint64_t sum = firstAddend + secondAddend;  // Calculate the sum
        // if sum is equal to Number
        if (sum == targetNumber) {  // If the sum is equal to the target number
            if (numberToCheck->is_negative) {  // If the number is negative
                // Add sum to sums with negative flag
                uint64_t* sums = malloc(2 * sizeof(uint64_t));  // Create the
                // array of sums
                if (sums) {
                    sums[0] = firstAddend;  // Set the first addend
                    sums[1] = secondAddend;  // Set the second addend
                    array_sums_append(numberToCheck->sums, sums);  // Append the
                    // sums
                } else {
                    fprintf(stderr,
                    "Error: memory allocation failed for sums array");
                    return EXIT_FAILURE;
                }
            }
            numberToCheck->sumNumber++;  // Increment the number of sums
            secondAddendCounter--;  // Seco
            firstAddendCounter++;  // Increment the first addend counter
        } else if (sum > targetNumber) {  // If the sum is greater
        // than the target
            secondAddendCounter--;  // Decrement the second addend counter
        } else {
            firstAddendCounter++;  // Increment the first addend counter
        }
        firstAddend = prime_numbers->elements[firstAddendCounter].number;
        // Set the first addend
        secondAddend = prime_numbers->elements[secondAddendCounter].number;
        // Set the second addend
    }
    // end while
    // end even_calculation
    return EXIT_SUCCESS;
}

// procedure goldbach_calculation (var Number : Number structure,
// var prime numbers : dynamic 64-bit unsigned integer array):
int goldbach_calculation(number_t* number,
array_prime_numbers_t* prime_numbers) {
    assert(number);
    assert(prime_numbers);
    int error = EXIT_SUCCESS;
    number_t* numberToCheck = number;
    if (!(numberToCheck->is_out_of_range)) {  // If the number is not
    // out of range
        if ((numberToCheck->number % 2) == 0) {  // If the number is even
            numberToCheck->is_even = true;
            // goldbach_even_calculation(number, prime numbers)
            error = goldbach_even_calculation(numberToCheck, prime_numbers);
        } else {  // else if number is odd
            numberToCheck->is_even = false;
            // goldbach_odd_calculation(number, prime numbers)
            error = goldbach_odd_calculation(numberToCheck, prime_numbers);
        }
    }
    // end goldbach_calculation
    return error;
}

// procedure prime_numbers_calculation (var max number: 64-bit integer):
int prime_numbers_calculation(int64_t maxNumber,
array_prime_numbers_t* prime_numbers) {  // Adapted from the sieve of
// Eratosthenes algorithm. https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
    assert(prime_numbers);
    int64_t maxNumberSqrt = (int64_t) sqrt(maxNumber) + 1;
    // Let bool be a Boolean array initially set to all true
    bool* is_prime = calloc(maxNumber + 1, sizeof(bool));
    // for i = 2 to sqrt(max number):
    for (int64_t index = 2; index < maxNumberSqrt; index++) {
        // if bool[i] is true:
        if (!(is_prime[index])) {  // If the number is prime.
            // for j = (i power 2) to max number by i:
            for (int64_t j = pow(index, 2); j < maxNumber; j += index) {
                // if j is divisible by i
                if (j % index == 0) {  // If the number is a multiple of the
                // prime number.
                    // bool[j] = false
                    is_prime[j] = true;
                }
            }
        }
        // end for
    }
    // for i=2 to max number
    for (int64_t index = 2; index < maxNumber; index++) {
        // if bool[i] is true
        if (!(is_prime[index])) {  // If the number is prime.
            // add i to prime numbers
            prime_number_t prime_number = {index, 0};
            // Append the prime number to the array of prime numbers.
            array_prime_numbers_append(prime_numbers, prime_number);
        }
    }
    prime_numbers->crive = is_prime;
    return EXIT_SUCCESS;
}
