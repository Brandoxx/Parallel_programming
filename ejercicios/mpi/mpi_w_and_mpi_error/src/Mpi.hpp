#ifndef MPI_WRAPPER_MPI_HPP
#define MPI_WRAPPER_MPI_HPP

#include "mpi.h"
#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

class MpiError;

/// Declare four of the rule-of-the-five methods
#define DECLARE_RULE4(Class, action) \
  Class(const Class& other) = action; \
  Class(Class&& other) = action; \
  Class& operator=(const Class& other) = action; \
  Class& operator=(Class&& other) = action

/// Disable default methods for copying objects
#define DISABLE_COPY(Class) \
  DECLARE_RULE4(Class, delete)

/**
 * @brief A wrapper for the MPI library.
 * @attribute: processCount The number of processes in the MPI world.
 * @attribute: processNumber The rank of the current process in the MPI world.
 * @attribute: hostname The hostname of the current process.
 */
class Mpi {
  private:
 
  // copies are not allow for this class, disables the 5 points rule
  DISABLE_COPY(Mpi);
  
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
    Mpi(int &argc, char** &argv);
  /**
   * @brief Destroy the Mpi object  
   * 
   */
    ~Mpi();
    /**
     * @brief Get the Process Number MPI
     * 
     * @return int the process number
     */
    int getProcessNumber() const;
    /**
     * @brief Get the Process Count MPI
     * 
     * @return int the process count
     */
    int getProcessCount() const;
    /**
     * @brief Get the Hostname MPI
     * 
     * @return std::string the hostname
     */
    std::string getHostname() const;

};
#endif // MPI_WRAPPER_MPI_H