// Copyright 2022 Brandon Mora Umana
#include "goldbach_calculation.h"

/**
 * @brief Append for the odd number calculation.
 * @details Mimics the even number calculation, but with the residue of the
 * goldbach_odd_calculation function.
 * @param fixedNumber Fixed prime number to add at the beggining of the sum.
 * @see goldbach_odd_calculation for more details.
 * @return Error code.
 */
int goldbach_odd_append(number_t* numberToCheck,
array_prime_numbers_t* prime_numbers, int64_t fixedNumber);

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

// procedure goldbach_odd_calculation (var Number : 64-bit integer,
// var prime numbers : dynamic 64-bit unsigned integer array):
// Reset prime numbers repeat counter
// for firstPrime = 0 to length(prime numbers)
//    if firstPrime is greater than Number
//        break
//    end if
//    Let difference be Number - firstPrime
//    Let differenceSums be a dynamic 64-bit unsigned sum array
//    Let numberToAdd be a number with differenceSums as an attribute, and
//    difference as a value
//    call goldbach_even_calculation(numberToAdd, prime_numbers, firstPrime)
//    if Number is negative
//        for i = 0 to length(differenceSums)
//        if differenceSums[i] has not repeated
//            add differenceSums[i] to Numbers addition array
//        end if
//        end for
//    end if
//    Number.sumNumber = Number.sumNumber + numberToAdd.sumNumber -
//    firstPrime.timesRepeated
// end for

// end odd_calculation

int goldbach_odd_calculation(number_t* numberToCheck,
array_prime_numbers_t* prime_numbers) {
    assert(numberToCheck);
    assert(prime_numbers);
    array_prime_numbers_reset_counter(prime_numbers);  // Reset the counter
    uint64_t number = numberToCheck->number;  // Get the number
    for (int64_t prime = 0; prime < prime_numbers->count;
    prime++) {  // Iterate through the prime numbers
        if (prime_numbers->elements[prime].number > number) {  // If the
        // prime number is greater than the number
            break;  // Break the loop
        }
        prime_number_t primeNumber = prime_numbers->elements[prime];  // Get
        // the prime number
        uint64_t difference = number - primeNumber.number;  // Get the
        // difference
        array_sums_t* differenceSums = malloc(sizeof(array_sums_t));  // Create
        // the array of sums
        if (differenceSums == NULL) {  // If the array of sums could not be
        // created
            fprintf(stderr,
            "Error: malloc failed in goldbach_odd_calculation function");
            return EXIT_FAILURE;
        }
        array_sums_init(differenceSums);  // Initialize the array of sums
        number_t numberToAdd = {difference, numberToCheck->is_negative, false,
        false, 0, differenceSums};  // Create a dummy number to calculate the
        // sums
        goldbach_odd_append(&numberToAdd, prime_numbers,
        primeNumber.number);  // Calculate the sums for the difference
        if (numberToCheck->is_negative) {  // If the number is negative
            for (int64_t addition = 0;  // Iterate through the sums
            addition < numberToAdd.sums->count; addition++) {
                if ((uint64_t) addition < primeNumber.timesRepeated) {  // If
                // the addition is less than the times the prime number is
                // repeated
                    free(numberToAdd.sums->elements[addition]);  // Free the
                    // sum
                } else {
                    array_sums_append(numberToCheck->sums,
                    numberToAdd.sums->elements[addition]);  // Append the sum
                }
            }
        }
        if (primeNumber.timesRepeated > numberToAdd.sumNumber) {  // If the
        // times the prime number is repeated is greater than the number of
        // sums
            primeNumber.timesRepeated = numberToAdd.sumNumber;
        }
        numberToCheck->sumNumber += numberToAdd.sumNumber -
        primeNumber.timesRepeated;  // Add the number of sums to the number
        free(differenceSums->elements);
        free(differenceSums);
    }
    return EXIT_SUCCESS;
}

int goldbach_odd_append(number_t* numberToCheck,
array_prime_numbers_t* prime_numbers, int64_t fixedNumber) {
    // See the goldbach_even_calculation function for more details
    assert(numberToCheck);
    assert(prime_numbers);
    uint64_t targetNumber = numberToCheck->number;
    uint64_t primeRange = targetNumber / 2;
    uint64_t firstAddendCounter = 0;
    uint64_t secondAddendCounter = prime_numbers->count - 1;
    prime_number_t* firstAddend = &prime_numbers->elements[firstAddendCounter];
    prime_number_t* secondAddend =
    &prime_numbers->elements[secondAddendCounter];
    while (firstAddend->number <= primeRange) {
        uint64_t sum = firstAddend->number + secondAddend->number;
        if (sum == targetNumber) {  // If the sum is equal to the target number
            uint64_t* sums = malloc(3 * sizeof(uint64_t));
            if (sums) {
                if (numberToCheck->is_negative) {
                    sums[0] = fixedNumber;  // Add the fixed number
                    sums[1] = firstAddend->number;
                    sums[2] = secondAddend->number;
                    array_sums_append(numberToCheck->sums, sums);
                } else {
                    free(sums);
                }
                firstAddend->timesRepeated++;  // Increment the times the
                // prime number is repeated
                secondAddend->timesRepeated++;  // Increment the times the
                // prime number is repeated
            } else {
                fprintf(stderr,
                 "Error: memory allocation failed for sums array");
                return EXIT_FAILURE;
            }
            numberToCheck->sumNumber++;  // Increment the number of sums
            secondAddendCounter--;  // Decrement the second addend counter
            firstAddendCounter++;  // Increment the first addend counter
        } else if (sum > targetNumber) {  // If the sum is greater
        // than the target
            secondAddendCounter--;  // Decrement the second addend counter
        } else {
            firstAddendCounter++;  // Increment the first addend counter
        }
        firstAddend = &prime_numbers->elements[firstAddendCounter];
        secondAddend = &prime_numbers->elements[secondAddendCounter];
    }
    return EXIT_SUCCESS;
}

// procedure goldbach_even_calculation (var Number : 64-bit integer,
// var prime numbers : dynamic 64-bit unsigned integer array):
// Let primeRange be Number/2
// Let firstAddend be 0
// Let secondAddend be prime number size
// while firstAddend is less than primeRange
//    Let sum be firstAddend + secondAddend
//    if sum is equal to Number
//        if Number is negative
//            Add sum to sums with negative flag
//        end if
//        Number.sumNumber++
//        firstAddend increased to next prime number
//        secondAddend decreased to next prime number
//    else if sum is greater than Number
//        secondAddend decreased to next prime number
//    else
//        firstAddend increased to next prime number
//    end if
// end while
// end even_calculation

int goldbach_even_calculation(number_t* numberToCheck,
array_prime_numbers_t* prime_numbers) {
    assert(numberToCheck);
    assert(prime_numbers);
    // Create new variables for readability
    uint64_t targetNumber = numberToCheck->number;
    uint64_t primeSize = prime_numbers->count;
    uint64_t primeRange = targetNumber / 2;
    uint64_t firstAddendCounter = 0;
    uint64_t secondAddendCounter = primeSize - 1;
    uint64_t firstAddend = prime_numbers->elements[firstAddendCounter].number;
    uint64_t secondAddend = prime_numbers->elements[secondAddendCounter].number;
    while (firstAddend <= primeRange) {  // While the first addend is less than
    // the prime range
        uint64_t sum = firstAddend + secondAddend;  // Calculate the sum
        if (sum == targetNumber) {  // If the sum is equal to the target number
            if (numberToCheck->is_negative) {  // If the number is negative
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
            secondAddendCounter--;  // Decrement the second addend counter
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
    return EXIT_SUCCESS;
}

// procedure goldbach_calculation (var Numbers :
// dynamic 64-bit unsigned integer array,
// var prime numbers : dynamic 64-bit unsigned integer array):
// for number in Numbers
//    if number is even
//        goldbach_even_calculation(number, prime numbers)
//    else if number is odd
//        goldbach_odd_calculation(number, prime numbers)
// end for
// end goldbach_calculation

int goldbach_calculation(array_number_t* numbers,
array_prime_numbers_t* prime_numbers) {
    assert(numbers);
    assert(prime_numbers);
    int error = EXIT_SUCCESS;
    for (int64_t number = 0; number < numbers->count; number++) {
        number_t* numberToCheck = &(numbers->elements[number]);
        if (!(numberToCheck->is_out_of_range)) {  // If the number is not
        // out of range
            if ((numberToCheck->number % 2) == 0) {  // If the number is even
                numberToCheck->is_even = true;
                error = goldbach_even_calculation(numberToCheck, prime_numbers);
            } else {
                numberToCheck->is_even = false;
                error = goldbach_odd_calculation(numberToCheck, prime_numbers);
            }
        }
    }
    return error;
}
