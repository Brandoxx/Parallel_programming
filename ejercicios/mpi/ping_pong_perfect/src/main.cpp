// Copyright  2022 Brandon Mora Umana GNU GENERAL PUBLIC LICENSE
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include "Mpi.hpp"
#include "MpiError.hpp"
/**
 * @brief play the ping pong game with two processes
 * 
 * @param process_number the process number
 * @param process_count the amount of processes
 * @param number_of_pings_pongs the number of pings and pongs to do
 */
void playPingPong(Mpi& mpi, int process_number, int process_count,
  int number_of_pings_pongs);
/**
 * @brief simulates a ping pong game with two processes
 * does 100 pings and pongs by default
 * 
 * @param argc number of arguments
 * @param argv in arg 1 the number of pings and pongs to do
 * @return int 
 */
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  try {
    Mpi mpi(argc, argv);
    int process_number = mpi.getProcessNumber();
    int process_count = mpi.getProcessCount();
    if (process_count != 2) {
      std::stringstream message;
      message << "This program requires exactly 2 processes, not "
        << process_count;
      report(MpiError(message.str()));
    }
    if (argc == 1) {
      playPingPong(mpi, process_number, process_count, 100);
    } else {
      try {
        int number_of_pings_pongs = std::stoi(argv[1]);
        playPingPong(mpi, process_number, process_count,
          number_of_pings_pongs);
      } catch (std::invalid_argument& e) {
        std::stringstream message;
        message << "The argument must be an integer, not " << argv[1];
        throw std::invalid_argument(message.str());
      }
    }
  } catch (const MpiError& mpierror) {
    std::cerr << "error: " << mpierror.what() << std::endl;
    error = EXIT_FAILURE;
  }
  return error;
}

void playPingPong(Mpi& mpi, int process_number, int process_count,
  int number_of_pings_pongs) {
  int signal = 1;
  const int previous_process = (process_count + process_number + 1)
    % process_count; // previous process
  const int next_process = (process_number + 1) % process_count;
  for (int i = 0; i < number_of_pings_pongs; i++) {
    try {
      if (process_number == 0) {
        mpi.send(signal, next_process, 0);
        std::cout << "- p[" << process_number << "] hit PING " << std::endl;
        mpi.receive(signal, next_process);
      } else {
        mpi.receive(signal, previous_process);
        mpi.send(signal, previous_process, 0);
        std::cout << "- p["<< process_number <<"] hit PONG" << std::endl;
      }
    } catch (const MpiError& mpierror) {
      std::cerr << "error: " << mpierror.what() << std::endl;
    }
  }
}