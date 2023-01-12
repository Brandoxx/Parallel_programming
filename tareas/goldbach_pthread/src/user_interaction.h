// Copyright 2022 Brandon Mora Umana
#ifndef SRC_USER_INTERACTION_H_
#define SRC_USER_INTERACTION_H_

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "array_number.h"

#define GOLDBACH_UPPER_BOUND 5
#define GOLDBACH_LOWER_BOUND -5

/**
 * @brief This function reads the input from the user and stores in an array of structs of type number_t
 * @param numbers The array of structs of type number_t
 * @return EXIT_SUCCESS if the input is valid, EXIT_FAILURE otherwise
 */    
int read_numbers(array_number_t* numbers);

/**
 * @brief This function prints the result of the goldbach conjecture
 * @param numbers The array of structs of type number_t
 * @return EXIT_SUCCESS if the input is valid, EXIT_FAILURE otherwise
 */
int print_result(array_number_t* numbers);


#endif  // SRC_USER_INTERACTION_H_
