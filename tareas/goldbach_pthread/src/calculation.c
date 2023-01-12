// Copyright 2022 Brandon Mora Umana
#include "calculation.h"


/**
 * @brief Calculates the Goldbach conjecture for a given number.
 * @details Calculates the Goldbach conjecture for a given number, and stores
 * the result in a variable inside the struct.
 * Calls two private functions, one to calculate even numbers, and one to
 * calculate odd numbers.
 * @param number An number to calculate the Goldbach conjecture for.
 * @param prime_numbers The array of prime numbers.
 * @return The error code.
 */
int goldbach_calculation(number_t* number,
array_prime_numbers_t* prime_numbers);

/**
 * @brief Append for the odd number calculation.
 * @details Mimics the even number calculation, but with the residue of the
 * goldbach_odd_calculation function.
 * @param fixedNumber Fixed prime number to add at the beggining of the sum.
 * @see goldbach_odd_calculation for more details.
 * @return Error code.
 */
int goldbach_odd_append(number_t* numberToCheck,
array_prime_numbers_t* prime_numbers, uint64_t fixedNumber);

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

// procedure goldbach_even_calculation (var Number : 64-bit integer,
// var prime numbers : dynamic 64-bit unsigned integer array):
int goldbach_odd_calculation(number_t* numberToCheck,
array_prime_numbers_t* prime_numbers) {
    assert(numberToCheck);
    assert(prime_numbers);
    uint64_t number = numberToCheck->number;  // Get the number
    uint64_t primeRange = number / 2;
    // for firstPrime = 0 to length(prime numbers)
    for (int64_t prime = 0; prime_numbers->elements[prime].number < primeRange;
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
                array_sums_append(numberToCheck->sums,
                numberToAdd.sums->elements[addition]);  // Append the sum
            }
        }
        numberToCheck->sumNumber += numberToAdd.sumNumber;
         // Add the number of sums to the number
        free(differenceSums->elements);
        free(differenceSums);
    }
    // end for
    // end odd_calculation
    return EXIT_SUCCESS;
}

int goldbach_odd_append(number_t* numberToCheck,
array_prime_numbers_t* prime_numbers, uint64_t fixedNumber) {
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
        if (firstAddend->number < fixedNumber) {
            firstAddendCounter++;
            firstAddend = &prime_numbers->elements[firstAddendCounter];
            continue;
        }
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

// procedure goldbach_odd_calculation (var Number : 64-bit integer,
int goldbach_even_calculation(number_t* numberToCheck,
array_prime_numbers_t* prime_numbers) {
    assert(numberToCheck);
    assert(prime_numbers);
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
    free(is_prime);
    return EXIT_SUCCESS;
}

// procedure threadCalculation(Numbers):
void* threadCalculation(void* data) {
    assert(data);
    thread_data_t* thread_data = (thread_data_t*) data;
    // Let Number be an structure
    number_t* number = NULL;
    while (1) {
        pthread_mutex_lock(thread_data->shared_data->mutex);  // Lock
        // the mutex
        int64_t index = thread_data->shared_data->consumed_numbers;
        // Let index be the number of consumed numbers
        // If index is less than the number of numbers
        if (index < thread_data->shared_data->numbers->count) {
            // Let Number be the next number
            number = &thread_data->shared_data->numbers->elements[index];
            // Increment the number of consumed numbers
            thread_data->shared_data->consumed_numbers++;
        } else {
            // Else
            // unlock the mutex
            pthread_mutex_unlock(thread_data->shared_data->mutex);
            break;
        }
        // unlock the mutex
        pthread_mutex_unlock(thread_data->shared_data->mutex);
        // goldbach_calculation(Number, prime numbers)
        goldbach_calculation(number, thread_data->shared_data->prime_numbers);
    }
    // end threadCalculation
    return NULL;
}
