#include <mpi.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#define fail(msg) throw std::runtime_error(msg)
/**
 * @brief Simulates a relay rae with processes
 * 
 * @param argc Number of arguments
 * @param argv Arguments
 * @param process_number Process number 
 * @param process_count Process count
 */
void simulate_relay_race(int argc, char* argv[], int process_number
  , int process_count);

/**
 * @brief Stage 1 of the race - run a lap
 * 
 * @param stage1_delay Delay in stage 1
 * @param process_number Process number
 * @param team_count Team count
 */
void run_stage1(int stage1_delay, int process_number, int team_count);
/**
 * @brief Stage 2 of the race - run a lap
 * 
 * @param stage2_delay Delay in stage 2
 * @param process_number Process number
 * @param team_count Team count
 */
void run_stage2(int stage2_delay, int process_number, int team_count);
/**
 * @brief The referee of the race
 * 
 * @param team_count Team count
 */
void referee(int team_count);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {  // Initialize MPI
    try {
      int process_number = -1;  // Process number
      MPI_Comm_rank(MPI_COMM_WORLD, &process_number);  // Get process number

      int process_count = -1;  // Process count
      MPI_Comm_size(MPI_COMM_WORLD, &process_count);  // Get process count

      char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };  // Hostname
      int hostname_length = -1;  // Hostname length
      MPI_Get_processor_name(process_hostname, &hostname_length);  // Get hostname

      simulate_relay_race(argc, argv, process_number, process_count);  // Simulate relay
    } catch (const std::runtime_error& exception) {
      std::cout << exception.what() << std::endl;
      error = EXIT_FAILURE;
    }
    MPI_Finalize();  // Finalize MPI
  } else {
    std::cerr << "error: could not init MPI" << std::endl;
    error = EXIT_FAILURE;
  }
  return error;
}

void simulate_relay_race(int argc, char* argv[], int process_number
  , int process_count) {  // Simulate relay
  if (argc == 3) {  // Check if there are 3 arguments
    if (process_count >= 3 && process_count % 2 == 1) {  // Check if there are enough processes
      const int team_count = process_count / 2;  // Team count
      const int stage1_delay = atoi(argv[1]);  // Stage 1 delay
      const int stage2_delay = atoi(argv[2]);  // Stage 2 delay

      if (process_number == 0) {  // Referee
        referee(team_count);  // Run referee
      } else if (process_number <= team_count) {  // Stage 1
        run_stage1(stage1_delay, process_number, team_count);  // Run stage 1
      } else {  // Stage 2
        run_stage2(stage2_delay, process_number, team_count);  // Run stage 2
      }
    } else {
      fail("error: process count must be odd and greater than 3");
    }
  } else {
    fail("usage: relay_race_dist stage1_delay stage2_delay");
  }
}

void run_stage1(int stage1_delay, int process_number, int team_count) {
  // wait_barrier()
  if(MPI_Barrier(MPI_COMM_WORLD) != MPI_SUCCESS) {  // Wait for all processes
    fail("error: could wait for barrier");  // Print error message
  }
  usleep(1000 * stage1_delay);  // Wait for stage 1 delay
  const int peer = process_number + team_count;  // Peer process number
  bool baton = true;  // Baton
  // send(&baton, 1, peer)
  if (MPI_Send(&baton, /*count*/ 1, MPI_C_BOOL, peer, /*tag*/ 0
    , MPI_COMM_WORLD) != MPI_SUCCESS) {  // Send baton
    fail("error: could not send baton");
  }
}

void run_stage2(int stage2_delay, int process_number, int team_count) {
  // wait_barrier()
  if(MPI_Barrier(MPI_COMM_WORLD) != MPI_SUCCESS) {  // Wait for all processes
    fail("error: could wait for barrier");  // Print error message
  }
  int peer = process_number - team_count;  // Peer process number
  bool baton = false;  // Baton
  // receive(&baton, 1, peer)
  if (MPI_Recv(&baton, /*capacity*/ 1, MPI_C_BOOL, /*source*/ peer
    , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {  // Receive baton
    fail("could not receive baton");
  }
  usleep(1000 * stage2_delay);  // Wait for stage 2 delay
  // send(&peer, 1, 0)
  if (MPI_Send(&peer, /*count*/ 1, MPI_INT, /*target*/ 0, /*tag*/ 0
    , MPI_COMM_WORLD) != MPI_SUCCESS) {  // Send baton
    fail("error: could not send team number to referee");
  }
}

void referee(int team_count) {
  const double start_time = MPI_Wtime();  // Start time
  // wait_barrier()
  if(MPI_Barrier(MPI_COMM_WORLD) != MPI_SUCCESS) {  // Wait for all processes
    fail("error: could wait for barrier");  // Print error message
  }
  int team = 0;  // Team number
  int place = 0;  // Place
  for (int index = 0; index < team_count; ++index) {  // For each team
    // receive(&team, 1, any_process)
    if (MPI_Recv(&team, /*capacity*/ 1, MPI_INT, MPI_ANY_SOURCE
      , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
        // Receive team number
      fail("error: could not receive team number");  // Print error message
    }
    const double elapsed = MPI_Wtime() - start_time;  // Elapsed time
    ++place;  // Increment place
    std::cout << "Place " << place << ": team " << team << " in " << elapsed
      << "s" << std::endl;  // Print place
  }
}
