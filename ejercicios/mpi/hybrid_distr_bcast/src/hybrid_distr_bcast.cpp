// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <mpi.h>
#include <omp.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>

int calculate_start(int rank, int end, int workers, int begin);
int calculate_finish(int rank, int end, int workers, int begin);

int main(int argc, char* argv[]) {
  //  Inicialize MPI
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    int process_number = -1;  // rank
    MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

    //  Get the number of nodes devices
    int process_count = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);

    char process_hostname[MPI_MAX_PROCESSOR_NAME];
    int hostname_length = -1;
    MPI_Get_processor_name(process_hostname, &hostname_length);

    //  Start and end
    int start = 0;
    int end = 0;

    if (process_number == 0) {
      std::cout << "Introduce start index\n";
      std::cin >> start;
      std::cout << "Instroduce end index\n";
      std::cin >> end;
    }

    if (MPI_Bcast(&start, /*count*/ 1, MPI_UINT64_T, /*root*/ 0
    , MPI_COMM_WORLD) != MPI_SUCCESS ) {
      std::cout << "Error sending parameters\n";
    }

    if (MPI_Bcast(&end, /*count*/ 1, MPI_UINT64_T, /*root*/ 0
    , MPI_COMM_WORLD) != MPI_SUCCESS ) {
      std::cout << "Error sending parameters\n";
    }

    double startTime = MPI_Wtime();
    //  Mapping each start of each process
    const int process_start = calculate_start(process_number, end
    , process_count, start);
    //  Mapping each finish of each process
    const int process_finish = calculate_finish(process_number, end
    , process_count, start);
    //  Once calculating start and finish, get size
    const int process_size = process_finish - process_start;

    std::cout << process_hostname << ':' << process_number << ": range ["
    << process_start << ", " << process_finish << "[ size " << process_size
    << std::endl;

    #pragma omp parallel default(none) shared(std::cout, process_hostname) \
    shared(process_number, process_start, process_finish)
    {  // NOLINT(whitespace/braces)
      int thread_start = -1;
      int thread_finish = -1;

      #pragma omp for schedule(static)
      for (int index = process_start; index < process_finish; ++index) {
        if (thread_start == -1) {
          thread_start = index;
        }
        thread_finish = index;
      }

      ++thread_finish;
      const int thread_size = thread_finish - thread_start;

      #pragma omp critical(can_print)
      std::cout << '\t' << process_hostname << ':' << process_number << '.'
      << omp_get_thread_num() << ": range [" << thread_start << ", " <<
      thread_finish << "[ size " << thread_size << std::endl;
    }

    double endTime = MPI_Wtime();
    std::cout << "Process " << process_number << " in time "
    << endTime - startTime << "s\n";

    MPI_Finalize();
  }

  return 0;
}

int calculate_start(int rank, int end, int workers, int begin) {
  const int range = end - begin;
  return begin + rank * (range / workers) + std::min(rank, range % workers);
}

int calculate_finish(int rank, int end, int workers, int begin) {
  return calculate_start(rank + 1, end, workers, begin);
}