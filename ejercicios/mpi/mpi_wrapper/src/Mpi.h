#ifndef MPI_WRAPPER_MPI_H
#define MPI_WRAPPER_MPI_H

#include "mpi.h"
#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>
class Mpi;

/// Declare four of the rule-of-the-five methods
#define DECLARE_RULE4(Class, action) \
  Class(const Class& other) = action; \
  Class(Class&& other) = action; \
  Class& operator=(const Class& other) = action; \
  Class& operator=(Class&& other) = action

/// Disable default methods for copying objects
#define DISABLE_COPY(Class) \
  DECLARE_RULE4(Class, delete)


#define fail(msg) throw std::runtime_error(msg)
/**
 * @brief A wrapper for the MPI library.
 * @attribute: processCount The number of processes in the MPI world.
 * @attribute: processNumber The rank of the current process in the MPI world.
 * @attribute: hostname The hostname of the current process.
 */
class Mpi {
  private:
  // /*! \cond PRIVATE */
  // copies are not allow for this class, disables the 5 points rule
  //DISABLE_COPY(Mpi);
  //*! \endcond */

    int processNumber;
    int processCount;
    std::string hostname;

    
  public:
  /**
   * @brief Construct a new Mpi wrapper object
   * 
   * @param argc a reference to the argc argument count of the main function
   * @param argv a reference to the argv array of the main function
   *
   */
    Mpi(int &argc, char** &argv) {
      if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
        MPI_Comm_size(MPI_COMM_WORLD, &processCount);
        MPI_Comm_rank(MPI_COMM_WORLD, &processNumber);
        char name[MPI_MAX_PROCESSOR_NAME];
        int nameLength;
        MPI_Get_processor_name(name, &nameLength);
        hostname = std::string(name, nameLength);
    } else {
      fail("MPI_Init failed");
    }
  }
  /**
   * @brief Destroy the Mpi object  
   * 
   */
    ~Mpi(){
      MPI_Finalize();
    }
    /**
     * @brief Get the Process Number MPI
     * 
     * @return int the process number
     */
    inline int getProcessNumber() const {
      return processNumber;
    }
    /**
     * @brief Get the Process Count MPI
     * 
     * @return int the process count
     */
    inline int getProcessCount() const {
      return processCount;
    }
    /**
     * @brief Get the Hostname MPI
     * 
     * @return std::string the hostname
     */
    inline std::string getHostname() const {
      return hostname;
    }

};
#endif // MPI_WRAPPER_MPI_H