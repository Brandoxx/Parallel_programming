#include "Mpi.hpp"
#include "mpi_error.hpp"
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
    Mpi::~Mpi(){
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
