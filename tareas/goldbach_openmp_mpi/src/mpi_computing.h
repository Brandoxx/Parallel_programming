#ifndef MPI_COMPUTING_H
#define MPI_COMPUTING_H

#include <assert.h>
#include <time.h>
#include <omp.h>
#include <mpi.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "array_number.h"
#include "array_sums.h"
#include "array_prime_numbers.h"
#include "calculation.h"
#include "user_interaction.h"

/**
 * @brief The function to be executed by process 0.
 * @details The function to be executed by process 0.
 * @return int 
 */
int mainProcess();

/**
 * @brief The function to be executed by the other processes.
 * 
 * @param threadNumber Number of threads to be used.
 * @param rank The rank of the process.
 * @return int 
 */
int calcProcess(uint64_t threadNumber, int rank);

/**
 * @brief Processing with only one process.
 * @details No MPI is used.
 */
void aloneProcess(uint64_t threadNumber);

#endif // MPI_COMPUTING_H