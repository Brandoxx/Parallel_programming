/// @copyright 2022 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes 
/// <jeisson.hidalgo@ucr.ac.cr>

#ifndef UNIFIER_HPP
#define UNIFIER_HPP

#include <exception>
#include <map>

#include "Producer.hpp"

/**
 * @brief A dispatcher is both, a producer of its own queue, and a consumer
 * of many queues.For each data that is consumed by other queues, the dispatcher
 * incorporates the data to it's own queue, using key-values.
 */
template <typename KeyType, typename DataType>
class Unifier : public Producer<DataType> {
  /// Objects of this class cannot be copied
  DISABLE_COPY(Unifier);

 protected:
  /// This thread will incorporate elements from the following queues to it's 
  // own queue
  std::map<KeyType, Queue<DataType>*> fromQueues;
  /// Alias to the inherited queue for a more meaninguful identifier
  Queue<DataType>*& toQueue = Producer<DataType>::producingQueue;

 public:
  /// Constructor
  explicit Unifier(Queue<DataType>* producingQueue = nullptr
    , const DataType& stopCondition = DataType())
    : Producer<DataType>(producingQueue, stopCondition) {
  }

  /// Destructor
  virtual ~Unifier() {
  }
  /// Creates a new empty queue owned by this particular producer
  void createOwnQueue() {
    assert(this->producingQueue == nullptr);
    this->producingQueue = new Queue<DataType>();
  }

  /// Register a map. When the data to be "produced" by this queue and consumed
  // from the fromQueue has this key, it will be redirected to this queue
  inline void registerRedirect(const KeyType& key, Queue<DataType>* fromQueue) {
    this->fromQueues[key] = fromQueue;
  }

  /// Override this method to produce any data extracted from the queues
  void produce(DataType data) override {
    const KeyType& key = this->extractKey(data);
    const auto& itr = this->fromQueues.find(key);
    if ( itr == this->fromQueues.end() ) {
      throw std::runtime_error("Unifier: queue's key not found");
    }
    if(!(itr->second->getConsumingQueue.empty())){
      toQueue.push(itr->second->pop(data));
    }
  }

  /// Override this method to extract the key from a data stored in fromQueues
  virtual KeyType extractKey(const DataType& data) const = 0;
};

#endif  // Unifier_HPP
