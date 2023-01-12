/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#include <cstdlib>
#include <iostream>

#include "ProducerConsumerTest.hpp"
#include "ConsumerTest.hpp"
#include "DispatcherTest.hpp"
#include "ProducerTest.hpp"

ProducerConsumerTest::~ProducerConsumerTest() {
  for (ProducerTest* producer : producers) {
    delete producer;
  }
  delete this->dispatcher;
  for ( ConsumerTest* consumer : this->consumers )
    delete consumer;
}

int ProducerConsumerTest::start(int argc, char* argv[]) {
  // Parse arguments and store values in object members
  if ( int error = this->analyzeArguments(argc, argv) ) {
    return error;
  }
  // Create the shared data
  this->producerData = new producersShareData(this->packageCount);
    // Create objects for the simulation
    // Create producers
  if ( 0 != this->createProducers() ) {
      // Error creating producers write message in stderr
      std::cerr << "Error creating producers" << std::endl;
      return EXIT_FAILURE;
  }
  this->dispatcher = new DispatcherTest(this->dispatcherDelay);
  this->dispatcher->createOwnQueue();
  // Create each producer
  if ( 0 != this->createConsumers() ) {
    // Error creating producers write message in stderr
    std::cerr << "Error creating producers" << std::endl;
    return EXIT_FAILURE;
  }
  // Communicate simulation objects using queues
  // register consumers in dispatcher map
  if ( 0 != this->registerConsumers() ) {
    // Error creating consumers write message in stderr
    std::cerr << "Error creating consumers" << std::endl;
    return EXIT_FAILURE;
  }
  // set producers to dispatch
  if ( 0 != this->setProducersQueue() ) {
    // Error creating consumers write message in stderr
    std::cerr << "Error creating consumers" << std::endl;
    return EXIT_FAILURE;
  }
  // Start the simulation
  // start producers
  for ( size_t index = 0; index < this->producerCount; ++index ) {
    this->producers[index]->startThread();
  }
  // start dispatcher
  this->dispatcher->startThread();
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->consumers[index]->startThread();
  }

  // Wait for objets to finish the simulation
  // wait for producers
  for ( size_t index = 0; index < this->producerCount; ++index ) {
    this->producers[index]->waitToFinish();
  }
  // wait for dispatcher
  this->dispatcher->waitToFinish();
  // wait for consumers
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->consumers[index]->waitToFinish();
  }
  delete this->producerData;
  // Simulation finished
  return EXIT_SUCCESS;
}
int ProducerConsumerTest::createProducers() {
  // Create each producer
  //this->producers.resize(this->producerCount);
  for ( size_t index = 0; index < this->producerCount; ++index ) {
    this->producers.push_back(new ProducerTest(this->packageCount,
      this->productorDelay, this->consumerCount, index + 1,
      this->producerData));
  }
  if ( this->producers.size() != this->producerCount ) {
    // Error creating producers write message in stderr
    std::cerr << "Error creating producers" << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
int ProducerConsumerTest::createConsumers() {
  this->consumers.resize(this->consumerCount);
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->consumers[index] = new ConsumerTest(this->consumerDelay);
    assert(this->consumers[index]);
    this->consumers[index]->createOwnQueue();
  }
  if ( this->consumers.size() != this->consumerCount ) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int ProducerConsumerTest::registerConsumers() {
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->dispatcher->registerRedirect(index + 1
      , this->consumers[index]->getConsumingQueue());
  }
  return EXIT_SUCCESS;
}
int ProducerConsumerTest::setProducersQueue() {
  for ( size_t index = 0; index < this->producerCount; ++index ) {
    this->producers[index]->setProducingQueue(
      this->dispatcher->getConsumingQueue());
  }
  return EXIT_SUCCESS;
}

int ProducerConsumerTest::analyzeArguments(int argc, char* argv[]) {
  // 6 + 1 arguments are mandatory
  if ( argc != 7 ) {
    return EXIT_FAILURE;
  }

  int index = 1;
  this->packageCount = std::strtoull(argv[index++], nullptr, 10);
  std::cout << "Packages to produce: " << this->packageCount << std::endl;
  this->producerCount = std::strtoull(argv[index++], nullptr, 10);
  std::cout << "Producers Count: " << this->producerCount << std::endl;
  this->consumerCount = std::strtoull(argv[index++], nullptr, 10);
  std::cout << "Consumers Count: " << this->consumerCount << std::endl;
  this->productorDelay = std::atoi(argv[index++]);
  std::cout << "Producer delay: " << this->productorDelay << std::endl;
  this->dispatcherDelay = std::atoi(argv[index++]);
  std::cout << "Dispatcher delay: " << this->dispatcherDelay << std::endl;
  this->consumerDelay = std::atoi(argv[index++]);
  std::cout << "Consumer delay: " << this->consumerDelay << std::endl;

  // todo: Validate that given arguments are fine
  return EXIT_SUCCESS;
}
