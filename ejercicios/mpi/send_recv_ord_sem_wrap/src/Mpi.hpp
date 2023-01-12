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
    static inline MPI_Datatype map(bool) { return MPI_C_BOOL; }
    static inline MPI_Datatype map(char) { return MPI_CHAR; }
    static inline MPI_Datatype map(unsigned char) { 
      return MPI_UNSIGNED_CHAR; }
    static inline MPI_Datatype map(short) { return MPI_SHORT; }
    static inline MPI_Datatype map(unsigned short) { 
      return MPI_UNSIGNED_SHORT; }
    static inline MPI_Datatype map(int) { return MPI_INT; }
    static inline MPI_Datatype map(unsigned) { return MPI_UNSIGNED; }
    static inline MPI_Datatype map(long) { return MPI_LONG; }
    static inline MPI_Datatype map(unsigned long) { return MPI_UNSIGNED_LONG; }
    static inline MPI_Datatype map(long long) { return MPI_LONG_LONG; }
    static inline MPI_Datatype map(unsigned long long) { 
      return MPI_UNSIGNED_LONG_LONG; }
    static inline MPI_Datatype map(float) { return MPI_FLOAT; }
    static inline MPI_Datatype map(double) { return MPI_DOUBLE; }
    static inline MPI_Datatype map(long double) { return MPI_LONG_DOUBLE; }

    template <typename data>
    /**
     * @brief Send a message to a specific process
     * call MPI_Send
     * @param data the data to send
     * @param toProcess the destination process
     * @param tag the tag of the message
     */
    void send(const data& value, int toProcess, int tag=0) {
      if ( MPI_Send(&value, 1, map(value), toProcess, tag, MPI_COMM_WORLD)
        != MPI_SUCCESS ) {
        throw MpiError("MPI_Send failed");
      }
    }

    template <typename data>
    /**
     * @brief Receive a message from a specific process
     * calls MPI_Recv
     * @param data the data to receive
     * @param fromProcess the source process
     * @param tag the tag of the message
     */
    void receive(data& value, int fromProcess = MPI_ANY_SOURCE, int tag = MPI_ANY_SOURCE) {
      if( MPI_Recv(&value, 1, map(value), fromProcess, 
        tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
          throw MpiError("MPI_Recv failed");
      }
    }
};
#endif // MPI_WRAPPER_MPI_H