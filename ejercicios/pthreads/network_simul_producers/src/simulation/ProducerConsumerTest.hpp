/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#ifndef PRODUCERCONSUMERTEST_HPP
#define PRODUCERCONSUMERTEST_HPP

#include <vector>
#include <cstddef>
#include <cstdlib>
#include <mutex>

#include "common.hpp"
#include "SharedData.hpp"
/**
 * @brief producer_shared_data is a struct that contains the shared data between
 * the producer.
 */

// Forward declarations
class ConsumerTest;
class DispatcherTest;
class ProducerTest;
/**
 * @brief Controller class that simulates the message passing between
 * producer and cosumers through a dispatcher
 */
class ProducerConsumerTest {
  DISABLE_COPY(ProducerConsumerTest);

 private:
  /// Number of packages to be produced
  size_t packageCount = 0;
  /// Number of consumer threads
  size_t consumerCount = 0;
  /// Number of producer threads
  size_t producerCount = 0;
  producersShareData* producerData = nullptr;
  /// Delay of producer to create a package, negative for max random
  int productorDelay = 0;
  /// Delay of dispatcher to dispatch a package, negative for max random
  int dispatcherDelay = 0;
  /// Delay of consumer to consume a package, negative for max random
  int consumerDelay = 0;

  /// Producer of the simulated network messages
  std::vector<ProducerTest*> producers;
  /// A dispatcher of the of the simulated network messages
  DispatcherTest* dispatcher = nullptr;
  /// Consumers of the simulated network messages
  std::vector<ConsumerTest*> consumers;

  std::mutex countMutex;
 public:
  /// Constructor
  ProducerConsumerTest() = default;
  /// Destructor
  ~ProducerConsumerTest();
  /// Start the simulation
  int start(int argc, char* argv[]);

 private:
  /// Analyze the command line arguments
  int analyzeArguments(int argc, char* argv[]);
  /// Create the producers
  int createProducers();
  /// create consumers
  int createConsumers();
  /// register consumers in dispatcher map
  int registerConsumers();
  /// set producers queue
  int setProducersQueue();
};

#endif  // PRODUCERCONSUMERTEST_HPP
