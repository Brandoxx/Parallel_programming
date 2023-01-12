// Copyright 2022 Brandon Mora Umana
#define _POSIX_C_SOURCE 199309L

#include <assert.h>
#include <time.h>
#include <omp.h>
#include <mpi.h>
#include <errno.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "mpi_computing.h"

// procedure main:
int main(int argc, char** argv) {
    int error = EXIT_SUCCESS;
    uint64_t threadNumber = __sysconf(_SC_NPROCESSORS_ONLN);
    if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
        int size;
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        if (size > 1) {  // If there is more than one process
            int rank;  // The rank of the process
            if (argc > 1) {  // If there is an argument
                threadNumber = strtoull(argv[1], NULL, 10);
            }
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            if (rank == 0) {
                mainProcess();
            } else {
                calcProcess(threadNumber, rank);
            }
        } else {
            aloneProcess(threadNumber);
        }
    } else {
        error = EXIT_FAILURE;
    }
    MPI_Finalize();
    // Let Numbers be a dynamic 64-bit unsigned integer array
    return error;
}
