// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <random>
#include "Mpi.hpp"
#include "MpiError.hpp"


const std::string usage = 
  std::string("Usage:\n- First Argument number points to win a game\n")+ 
  std::string( 
    "- Second Argument is the probability to hit the ball of the process 0 \n")+
  std::string(
    "- Third Argument is the probability to hit the ball of the process 1 \n");
/**
 * @brief play the ping pong game with two processes
 * 
 * @param processNumber the process number
 * @param processCount the amount of processes
 * @param 
 */
void playPingPong(Mpi& mpi,int processNumber, int processCount,
  int scoreToWin, double probOfHitFirst,
  double probOfHitSecond);
/**
 * @brief fills the paramters with the arguments and checks if they are valid
 * @details assumes 3 arguments
 * @param argv is the array of arguments
 * @param scoreToWin is the number of points to win a game
 * @param probHitFirst is the probability to hit the ball of the process 0
 * @param probHitSecond is the probability to hit the ball of the process 1
 *
 */
void getArgs(char* argv[], int& scoreToWin, double& probOfHitFirst,
  double& probOfHitSecond);
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
  if (argc == 4) {
    try {
      Mpi mpi(argc, argv);
      int processNumber = mpi.getProcessNumber();
      int processCount = mpi.getProcessCount();
      if (processCount != 2) {
        std::stringstream message;
        message << "This program requires exactly 2 processes, not "
          << processCount;
        report(MpiError(message.str()));
      }
      int scoreToWin = 0;
      double probOfHitFirst = 0.0;
      double probOfHitSecond = 0.0;
      getArgs(argv, scoreToWin, probOfHitFirst, probOfHitSecond);
      playPingPong(mpi, processNumber, processCount, scoreToWin, probOfHitFirst, 
        probOfHitSecond);
    } catch (const MpiError& mpierror) {
      std::cerr << "error: " << mpierror.what() << std::endl;
      error = EXIT_FAILURE;
    }
  } else {
    std::cerr << usage << std::endl;
    error = EXIT_FAILURE;
  }
  return error;
}

void playPingPong(Mpi& mpi, int processNumber, int processCount,
  int scoreToWin, double probOfHitFirst, double probOfHitSecond) {
  const int previous_process = (processCount + processNumber + 1)
    % processCount; // previous process
  const int next_process = (processNumber + 1) % processCount;
  int gamesWonFirst = 0;
  int gamesWonSecond = 0;
  for ( int game = 1; game <= 3; game++ ) {
    int successfulHit = 0;
    int pointsFirst = 0;
    int pointsSecond = 0;
    int numberOfHits = 0;
    double chance = 0;
    try {
      while (pointsFirst < scoreToWin && pointsSecond < scoreToWin) {
        std::random_device                  rand_dev;
        std::mt19937                        generator(rand_dev());
        std::uniform_real_distribution<double>  distr(0, 100);
        if (processNumber == 0) {
          chance = distr(generator);
          // if the chance is less than the probability of hit then
          successfulHit = (chance <= probOfHitFirst) ? 1 : 0;
          mpi.send(successfulHit, next_process);
          mpi.receive(successfulHit, previous_process);
          if (successfulHit == 0) {
            pointsFirst = pointsFirst + 1;
          }
        } else {
          mpi.receive(successfulHit, previous_process);
          if (successfulHit == 0) {
            pointsSecond = pointsSecond + 1;
          }
          chance = distr(generator);
          // if the chance is less than the probability of hit then
          successfulHit = (chance <= probOfHitSecond) ? 1 : 0;
          mpi.send(successfulHit, next_process);
        }
        numberOfHits = numberOfHits + 1;
        // update the score
        if (processNumber == 1) {
          mpi.receive(pointsFirst, 0);
          mpi.send(pointsSecond, 0);
        } else {
          mpi.send(pointsFirst, 1);
          mpi.receive(pointsSecond, 1);
        }
      }
      int winner = (pointsFirst < pointsSecond) ? 1 : 0;
      if (processNumber == 0) {
        std::cout << game << ": " << numberOfHits << " " << winner << std::endl;
        if (winner == 0) {
          gamesWonFirst = gamesWonFirst + 1;
        } else {
          gamesWonSecond = gamesWonSecond + 1;
        }
      }
    } catch (const MpiError& mpierror) {
      report(mpierror);
    }
  }
  if (processNumber == 0) {
    int processWinner = (gamesWonFirst > gamesWonSecond) ? 0 : 1;
    std::string outMessage;
    outMessage += std::to_string(processWinner) + " wins " + std::to_string(gamesWonFirst)
    + " to " + std::to_string(gamesWonSecond);
    std::cout << outMessage << std::endl; 
  }
}
void getArgs(char* argv[], int& scoreToWin, double& probHitFirst,
  double& probHitSecond ){
  try {
    scoreToWin = std::stoi(argv[1]);
    probHitFirst = std::stod(argv[2]);
    probHitSecond = std::stod(argv[3]);
  } catch (const std::invalid_argument& ia) {
    std::cerr << "Invalid argument: " << ia.what() << std::endl;
    std::cerr << usage << std::endl;
    exit(EXIT_FAILURE);
  } 
}
  