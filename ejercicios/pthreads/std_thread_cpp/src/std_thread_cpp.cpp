#include <inttypes.h>  // for PRIu64
#include <thread>  // std::thread
#include <vector>  // std::vector
#include <iostream>  // std::cout
#include <chrono>  // std::chrono::seconds
#include <memory>  // std::unique_ptr
#include <syncstream>  // std::osyncstream
struct shared_data_t {
  uint64_t thread_count;
};

struct private_data_t {
  uint64_t thread_number;
  std::shared_ptr<shared_data_t> shared_data;
};

void greet(std::unique_ptr<private_data_t> private_data);
int create_threads(std::shared_ptr<shared_data_t> shared_data);

int main(int argc, char** argv) {
  uint64_t thread_count = std::thread::hardware_concurrency();
  if (argc == 2) {
    try {
      thread_count = std::stoull(argv[1]);
    }
    catch(const std::invalid_argument& e) {
      std::cerr << "Use only numbers" << '\n';
      return 10;
    }
    catch(const std::out_of_range& e) {
      std::cerr << "Use numbers within a reasonable numerical range" << '\n';
      return 11;
    }
  }
  auto start_time = std::chrono::high_resolution_clock::now();
  try {
    auto shared_data = std::make_shared<shared_data_t>();
    shared_data->thread_count = thread_count;
    create_threads(shared_data);
  }
  catch(const std::bad_alloc& e) {
    std::osyncstream(std::cerr) << "Failed to allocate memory for " 
    << thread_count << " threads" << '\n';
    return 12;
  }
  catch(const std::system_error& e) {
    std::osyncstream(std::cerr) << "Error: " << e.what() << '\n';
    return 13;
  }
  catch(...) {
    std::osyncstream(std::cerr) << "Unknown error" << '\n';
    return 14;
  }
  auto finish_time = std::chrono::high_resolution_clock::now();
  auto elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>
  (finish_time - start_time).count();
  std::cout << "Execution time: " << elapsed_time << "ns" << '\n';
  return 0;
}
 
int create_threads(std::shared_ptr<shared_data_t> shared_data) {
  int error = EXIT_SUCCESS;
  std::vector<std::thread> threads;
  std::vector<std::unique_ptr<private_data_t>> private_data;
  for (uint64_t thread_number = 0; thread_number < shared_data->thread_count;
   ++thread_number) {
    private_data.emplace_back(std::make_unique<private_data_t>());
    private_data[thread_number]->thread_number = thread_number;
    private_data[thread_number]->shared_data = shared_data;
    threads.emplace_back(std::thread(greet, 
    std::move(private_data[thread_number])));
  }
  for (auto& thread : threads) {
    thread.join();
  }
  return error;
}

void greet(std::unique_ptr<private_data_t> private_data) {
  std::osyncstream(std::cout) << "Hello from thread " << 
  private_data->thread_number << " of " <<
  private_data->shared_data->thread_count << '\n';
}