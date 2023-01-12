// Copyright  2022 Brandon Mora Umana GNU GENERAL PUBLIC LICENSE
#include "Mpi.hpp"
#include "MpiError.hpp"
#define sendMpiError(msg) throw MpiError(msg)
/**
 * @brief Construct a new Mpi wrapper object
 * 
 * @param argc a reference to the argc argument count of the main function
 * @param argv a reference to the argv array of the main function
 *
 */
Mpi::Mpi(int &argc, char** &argv) {
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &processNumber);
    char name[MPI_MAX_PROCESSOR_NAME];
    int nameLength;
    MPI_Get_processor_name(name, &nameLength);
    hostname = std::string(name, nameLength);
  } else {
  sendMpiError("MPI_Init failed");
  }
}
/**
 * @brief Destroy the Mpi object  
 * 
 */
Mpi::~Mpi() {
  MPI_Finalize();
}

int Mpi::getProcessNumber() const {
  return processNumber;
}

int Mpi::getProcessCount() const {
  return processCount;
}

std::string Mpi::getHostname() const {
  return hostname;
}
/**
   * @brief send an array of char to a specific process
   * 
   * @param text a pointer to the char array
   * @param toProcess the destination process
   * @param tag the tag of the message
   *
   */
    void Mpi::send(const std::string& text, int toProcess, int tag ) {
      if ( MPI_Send(text.data(), text.length(), map(text[0]), toProcess, tag,
      MPI_COMM_WORLD) != MPI_SUCCESS ) {
        throw MpiError("MPI_Send failed");
      }
    }
    /**
    * @brief Receive an array of char from a specific process
    * 
    * @param text the buffer to receive the data in
    * @param capacity the capacity of the buffer
    * @param fromProcess the source process
    * @param tag the
    */
    void Mpi::receive(std::string& text, int capacity,
      int fromProcess, int tag) {
      if ( MPI_Recv(&text[0], capacity, MPI_CHAR, fromProcess, tag,
        MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
        throw MpiError("MPI_Recv failed");
      }
    }
