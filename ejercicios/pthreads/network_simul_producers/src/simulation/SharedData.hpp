#ifndef SHAREDDATA_HPP
#define SHAREDDATA_HPP
#include <cstddef>
#include <cstdlib>
#include <mutex>

#include "common.hpp"
struct producersShareData {
  std::mutex mutex;
  /// counter of the number of packages left to be produced
  int Count = 0;
  /// constructor
  producersShareData(int count): Count(count) {}
  /// destructor
  ~producersShareData() {}
  /// gets the index of the next package to be produced
  /// decrements the counter
};
#endif // SHAREDDATA_HPP