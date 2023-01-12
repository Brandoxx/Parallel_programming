#ifndef MPI_ERROR_HPP
#define MPI_ERROR_HPP
#include <stdexcept>
#include <iostream>
class Mpi;

class MpiError : public std::runtime_error {
public:
  /**
   * @brief explicit Constructor a new Mpi Error,
   *  inheriting from std::runtime_error
   * @param message the error message
   *
   */
  explicit MpiError(const std::string& message) 
    : std::runtime_error(message){};
  /**
   * @brief Construct a new Mpi Error object, inheriting from std::runtime_error
   * @param message a string literal error message
   * @param mpi the Mpi object to use for error reporting
   */
  MpiError(const std::string& message, const Mpi& mpi);
  /**
   * @brief Construct a new Mpi Error object, inheriting from std::runtime_error
   * @param message a string literal error message
   * @param mpi the Mpi object to use for error reporting
   * @param theadNumber the thread number to use for error reporting
   *
   */ 
  MpiError(const std::string& message, const Mpi& mpi, const int threadNumber); 
};
#endif // MPI_ERROR_HPP