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



// procedure read_numbers (var Numbers: dynamic 64-bit unsigned integer array):
// While not end of file:
//    if number is not a valid integer
//        print error message
//        exit
//    else
//        if number is out of range
//            set out of range flag to 1
//        if number is negative
//            number = -number
//            set negative flag to 1
//        Let sums be a dynamic 64-bit unsigned integer arrays array
//        Add number to Numbers with all flags
// end read_numbers

int read_numbers(array_number_t* numbers) {
    assert(numbers);
    errno = 0;
    int error = EXIT_SUCCESS;
    char *buffer = NULL;
    char *endptr = NULL;
    int64_t number = 0;
    while (scanf("%ms", &buffer) != EOF) {  // Read the input
        bool is_negative = false;
        bool is_out_of_range = false;
        number = strtoll(buffer, &endptr, 10);  // Convert the input to a number
        if ((endptr == buffer) || (*endptr != '\0')) {  // If the input is not a
        // number.
            fprintf(stderr, "Invalid number: %s\n", buffer);
            return EXIT_FAILURE;
        }
        if (errno == ERANGE) {
            errno = 0;
            is_out_of_range = true;
        }
        if (number >= GOLDBACH_LOWER_BOUND && number <= GOLDBACH_UPPER_BOUND) {
            is_out_of_range = true;
        }
        if (number < 0) {
            number = -number;
            is_negative = true;
        }
        array_sums_t *sums = malloc(sizeof(array_sums_t));
        if (sums) {
            error = array_sums_init(sums);
            number_t numberToAdd = {number, is_negative, is_out_of_range, false,
            0, sums};
            array_number_append(numbers, numberToAdd);
        } else {
            fprintf(stderr, "Error allocating memory for sums\n");
            return EXIT_FAILURE;
        }
        free(buffer);
    }
    return error;
}

// procedure print_result (var Numbers : dynamic 64-bit unsigned integer array):
//    call print_total_results(Numbers)
//    call print_numbers(Numbers)
// end print_numbers

int print_result(array_number_t* numbers) {
    assert(numbers);
    int error = print_total_result(numbers);
    if (error == EXIT_SUCCESS) {
        error = print_numbers(numbers);
    }
    return EXIT_SUCCESS;
}

// procedure print_numbers (var Numbers :
// dynamic 64-bit unsigned integer array):
// for number in Numbers
//    print number
//    if number is out of range
//        print NA
//    else if number is negative
//        if number is even
//            call even_print(number)
//        else if number is odd
//            call odd_print(number)
//    else
//        print number of Goldbach additions
// end for
// end print_numbers

int print_numbers(array_number_t* numbers) {
    assert(numbers);
    int error = EXIT_SUCCESS;
    for (int64_t number = 0; number < numbers->count; number++) {
        number_t* numberToPrint = &(numbers->elements[number]);
        if (numberToPrint->is_out_of_range) {
            if (numberToPrint->is_negative) {
            printf("-%" PRId64 ": NA\n", numberToPrint->number);
            } else {
            printf("%" PRId64 ": NA\n", numberToPrint->number);
            }
        } else {
            if (numberToPrint->is_negative) {
                printf("-%" PRId64 , numberToPrint->number);
                if (numberToPrint->is_even) {
                    printf(": %" PRId64 " sums" , numberToPrint->sumNumber);
                    error = even_number_print(numberToPrint);
                } else {
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
    return error;
}

// Procedure even_print (var Number : 64-bit integer):
// for counter = 0 to length(Number addition array)
//    print Number addition array[counter][1]
//    print +
//    print Number addition array[counter][2]
// end for
// end even_print

int even_number_print(number_t* numberToPrint) {
    assert(numberToPrint);
    printf(": ");
    for (int64_t sum = 0; sum < numberToPrint->sums->count-1; sum++) {
        printf("%" PRId64, numberToPrint->sums->elements[sum][0]);
        printf(" + %" PRId64 ", ", numberToPrint->sums->elements[sum][1]);
    }
    printf("%" PRId64, numberToPrint->sums->
     elements[numberToPrint->sumNumber-1][0]);
    printf(" + %" PRId64, numberToPrint->sums->
     elements[numberToPrint->sumNumber-1][1]);
    return EXIT_SUCCESS;
}

// procedure odd_print (var Number : 64-bit integer):
// for counter = 0 to length(Number addition array)
//    print Number addition array[counter][1]
//    print +
//    print Number addition array[counter][2]
//    print +
//    print Number addition array[counter][3]
// end for
// end odd_print

int odd_number_print(number_t* numberToPrint) {
    assert(numberToPrint);
    printf(": ");
    for (uint64_t sum = 0; sum < numberToPrint->sumNumber - 1; sum++) {
        printf("%" PRId64, numberToPrint->sums->elements[sum][0]);
        printf(" + %" PRId64, numberToPrint->sums->elements[sum][1]);
        printf(" + %" PRId64, numberToPrint->sums->elements[sum][2]);
        printf(", ");
    }
    printf("%" PRId64, numberToPrint->sums->
     elements[numberToPrint->sumNumber-1][0]);
    printf(" + %" PRId64, numberToPrint->sums->
     elements[numberToPrint->sumNumber-1][1]);
    printf(" + %" PRId64, numberToPrint->sums->
    elements[numberToPrint->sumNumber-1][2]);
    return EXIT_SUCCESS;
}

// procedure print_total_result (var Numbers :
// dynamic 64-bit unsigned integer array):
// Let total be a 64-bit integer initially set to 0
// Let total_numbers be a 64-bit integer initially set to Numbers length
// for every number in Numbers
//        if number is not out of range
//            total = total + Number_sum_number
// end for

// end print_total_result

int print_total_result(array_number_t* numbers) {
    uint64_t totalNumbers = numbers->count;
    uint64_t totalSumNumbers = 0;
    for (int64_t number = 0; number < numbers->count; number++) {  // Count the
    // total number of sums.
        if (!(numbers->elements[number].is_out_of_range)) {  // If the number is
        // in the range.
            totalSumNumbers += numbers->elements[number].sumNumber;
        }
    }
    printf("Total %" PRIu64 " numbers, ", totalNumbers);
    printf("%" PRIu64 " sums\n", totalSumNumbers);
    return EXIT_SUCCESS;
}
