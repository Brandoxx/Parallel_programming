// Copyright 2022 Brandon Mora Umana
#include "user_interaction.h"

/**
 * @brief This function prints the total result of the calculation, including
 * every number and the number of sums.
 * @param numbers The array of structs of type number_t
 * @return EXIT_SUCCESS if the input is valid, EXIT_FAILURE otherwise
 */
int print_total_result(array_number_t* numbers);

/**
 * @brief This function prints the calculation of an even number
 * @param numberToPrint A number_t containing the number and the result of the calculation
 * @return EXIT_SUCCESS if the input is valid, EXIT_FAILURE otherwise
 */
int even_number_print(number_t* numberToPrint);

/**
 * @brief This function prints the calculation of an odd number
 * @param numberToPrint A number_t containing the number and the result of the calculation
 * @return EXIT_SUCCESS if the input is valid, EXIT_FAILURE otherwise
 */
int odd_number_print(number_t* numberToPrint);

/**
 * @brief This function calls the correct print function depending on the number
 * @param numbers The number array, inputed by the user
 * @return EXIT_SUCCESS if the input is valid, EXIT_FAILURE otherwise
 */
int print_numbers(array_number_t* numbers);

int read_numbers(array_number_t* numbers) {
    assert(numbers);
    errno = 0;
    int error = EXIT_SUCCESS;
    char *buffer = NULL;
    char *endptr = NULL;
    int64_t number = 0;
    // While not end of file:
    while (scanf("%ms", &buffer) != EOF) {  // Read the input
        bool is_negative = false;
        bool is_out_of_range = false;
        number = strtoll(buffer, &endptr, 10);  // Convert the input to a number
        if ((endptr == buffer) || (*endptr != '\0')) {  // If the input is not a
        // number.
            // print error message
            fprintf(stderr, "Invalid number: %s\n", buffer);
            // exit
            free(buffer);
            return EXIT_FAILURE;
        }
        // else
        // if number is out of range
        if (errno == ERANGE) {
            errno = 0;
            // set out of range flag to 1
            is_out_of_range = true;
        }
        if (number >= GOLDBACH_LOWER_BOUND && number <= GOLDBACH_UPPER_BOUND) {
            is_out_of_range = true;
        }
        // if number is negative
        if (number < 0) {
            // number = -number
            number = -number;
            // set negative flag to 1
            is_negative = true;
        }
        // Let sums be a dynamic 64-bit unsigned integer arrays array
        array_sums_t *sums = malloc(sizeof(array_sums_t));
        if (sums) {
            error = array_sums_init(sums);
            // Add number to Numbers with all flags
            number_t numberToAdd = {number, is_negative, is_out_of_range, false,
            0, sums};
            array_number_append(numbers, numberToAdd);
        } else {
            fprintf(stderr, "Error allocating memory for sums\n");
            return EXIT_FAILURE;
        }
        free(buffer);
    }
    // end read_numbers
    return error;
}

// procedure print_result (var Numbers : dynamic 64-bit unsigned integer array):
int print_result(array_number_t* numbers) {
    assert(numbers);
    int error = EXIT_SUCCESS;
    if (error == EXIT_SUCCESS) {
        // call print_numbers(Numbers)
        error = print_numbers(numbers);
    }
    return EXIT_SUCCESS;
}

int print_numbers(array_number_t* numbers) {
    assert(numbers);
    int error = EXIT_SUCCESS;
    // for number in Numbers
    for (int64_t number = 0; number < numbers->count; number++) {
        number_t* numberToPrint = &(numbers->elements[number]);
        // if number is out of range
        if (numberToPrint->is_out_of_range) {
            // print NA
            if (numberToPrint->is_negative) {
                // print number
                printf("-%" PRId64 ": NA\n", numberToPrint->number);
            } else {
                printf("%" PRId64 ": NA\n", numberToPrint->number);
            }
        } else {
            // if number is negative
            if (numberToPrint->is_negative) {
                printf("-%" PRId64 , numberToPrint->number);
                // if number is even
                if (numberToPrint->is_even) {
                    // call even_print(number)
                    printf(": %" PRId64 " sums" , numberToPrint->sumNumber);
                    error = even_number_print(numberToPrint);
                } else {
                    // else if number is odd
                    // call odd_print(number)
                    printf(": %" PRId64 " sums" , numberToPrint->sumNumber);
                    error = odd_number_print(numberToPrint);
                }
            } else {
                printf("%" PRId64 ": %" PRId64 " sums",
                numberToPrint->number, numberToPrint->sumNumber);
            }
            printf("\n");
        }
    }
    // end for
    // end print_numbers
    return error;
}

// Procedure even_print (var Number : 64-bit integer):
int even_number_print(number_t* numberToPrint) {
    assert(numberToPrint);
    printf(": ");
    // for counter = 0 to length(Number addition array)
    for (int64_t sum = 0; sum < numberToPrint->sums->count-1; sum++) {
        // print Number addition array[counter][1]
        printf("%" PRId64, numberToPrint->sums->elements[sum][0]);
        // print +
        // print Number addition array[counter][2]
        printf(" + %" PRId64 ", ", numberToPrint->sums->elements[sum][1]);
    }
    // end for
    printf("%" PRId64, numberToPrint->sums->
     elements[numberToPrint->sumNumber-1][0]);
    printf(" + %" PRId64, numberToPrint->sums->
     elements[numberToPrint->sumNumber-1][1]);
    // end even_print
    return EXIT_SUCCESS;
}

// Procedure odd_print (var Number : 64-bit integer):
int odd_number_print(number_t* numberToPrint) {
    assert(numberToPrint);
    printf(": ");
    // for counter = 0 to length(Number addition array)
    for (uint64_t sum = 0; sum < numberToPrint->sumNumber - 1; sum++) {
        // print Number addition array[counter][1]
        printf("%" PRId64, numberToPrint->sums->elements[sum][0]);
        // print +
        // print Number addition array[counter][2]
        printf(" + %" PRId64, numberToPrint->sums->elements[sum][1]);
        // print +
        // print Number addition array[counter][3]
        printf(" + %" PRId64, numberToPrint->sums->elements[sum][2]);
        printf(", ");
    }
    // end for
    printf("%" PRId64, numberToPrint->sums->
     elements[numberToPrint->sumNumber-1][0]);
    printf(" + %" PRId64, numberToPrint->sums->
     elements[numberToPrint->sumNumber-1][1]);
    printf(" + %" PRId64, numberToPrint->sums->
    elements[numberToPrint->sumNumber-1][2]);
    // end odd_print
    return EXIT_SUCCESS;
}

// procedure print_total_result (var Numbers :
// dynamic 64-bit unsigned integer array):
int print_total_result(array_number_t* numbers) {
    // Let total be a 64-bit integer initially set to 0
    uint64_t totalNumbers = numbers->count;
    // Let total_numbers be a 64-bit integer initially set to Numbers length
    uint64_t totalSumNumbers = 0;
    // for every number in Numbers
    for (int64_t number = 0; number < numbers->count; number++) {  // Count the
    // total number of sums.
        if (!(numbers->elements[number].is_out_of_range)) {  // If the number is
        // in the range.
            // total = total + Number_sum_number
            totalSumNumbers += numbers->elements[number].sumNumber;
        }
    }
    // end for
    printf("Total %" PRIu64 " numbers, ", totalNumbers);
    printf("%" PRIu64 " sums\n", totalSumNumbers);
    // end print_total_result
    return EXIT_SUCCESS;
}
