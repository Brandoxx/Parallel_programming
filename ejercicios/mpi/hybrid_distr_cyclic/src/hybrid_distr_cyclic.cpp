// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <mpi.h>
#include <omp.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

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
    std::vector<int> indexList;

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
    for (int index = process_number; index < end; index += process_count) {
      indexList.push_back(index);
    }
    //  Once calculating start and finish, get size
    const int process_size = indexList.size();

    std::cout << process_hostname << ':' << process_number << ": ";
    for (int iter = 0; iter < process_size; ++iter) {
      if (iter == 0) {
        std::cout << indexList[iter];
      } else {
        std::cout << ", " << indexList[iter];
      }
    }
    std::cout << " size " << process_size << std::endl;

    #pragma omp parallel default(none) shared(std::cout, process_hostname) \
    shared(process_number, process_size, indexList)
    {  // NOLINT(whitespace/braces)
      #pragma omp for schedule(static)
      for (int index = 0; index < process_size; ++index) {
        std::cout << '\t' << process_hostname << ':' << process_number << '.'
        << omp_get_thread_num() << indexList[index] << std::endl;
      }
    }

    double endTime = MPI_Wtime();
    std::cout << "Process " << process_number << " in time "
    << endTime - startTime << "s\n";

    MPI_Finalize();
  }

  return 0;
}