/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#include "ProducerTest.hpp"
#include "Log.hpp"
#include "Util.hpp"
void ProducerTest::setId(int id) {
  this->id = id;
}
int ProducerTest::getId() {
  return this->id;
}
ProducerTest::ProducerTest(size_t packageCount, int productorDelay
  , size_t consumerCount, int id, producersShareData* sharedData)
  : packageCount(packageCount)
  , productorDelay(productorDelay)
  , consumerCount(consumerCount)
  , sharedData(sharedData)
  , id(id) {
}

int ProducerTest::run() {
  // Produce each asked message
  /// id of the message to be produced
  size_t index = 0;
  /// Number of packages produced by this producer
  size_t messagesProduced = 0;

  while (true) {
    // lock mutex to access and modify packageCount
    this->sharedData->mutex.lock();
    index = sharedData->Count;
    if (sharedData->Count > 0) {
          sharedData->Count--;
    }
    this->sharedData->mutex.unlock();
    // Check if the message is the last one
    if (index == 0) {
      this->produce(NetworkMessage());
      break;
    }
    // std::cout << "Producer " << this->getId() << " produced message "
    // << index + "\n";
    this->produce(this->createMessage(index));
    messagesProduced++;
  }
  // Report production is done
  Log::append(Log::INFO, (std::to_string(this->getId()) + " Producer"), 
    std::to_string(messagesProduced)
    + " menssages sent");
  return EXIT_SUCCESS;
}

NetworkMessage ProducerTest::createMessage(size_t index) const {
  // Source is always 1: this producer
  const uint16_t source = 1;
  // Target is selected by random
  const uint16_t target = 1 + Util::random(0
    , static_cast<int>(this->consumerCount));
  // IMPORTANT: This simulation uses sleep() to mimics the process of
  // producing a message. However, you must NEVER use sleep() for real projects
  Util::sleepFor(this->productorDelay);
  // Create and return a copy of the network message
  return NetworkMessage(target, source, index);
}
