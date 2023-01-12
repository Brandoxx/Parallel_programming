// Copyright 2022 Brandon Mora Umana GNU GENERAL PUBLIC LICENSE
#include <omp.h>
#include <iostream>
#include "UniformRandom.hpp"
/**
 * @brief Generates pseudorandom numbers using a uniform distribution
 * @param amount is the amount of numbers to generate
 * @param numbers is the array where the numbers will be stored
 */
void genRandNumbers(int amount, double* numbers);
/**
 * @brief Sorts an array of numbers using the odd-even sort algorithm
 * @param n is the amount of numbers to sort
 * @param arr The array of numbers to sort
 * @param threadAmount threads to use
 * @details The odd-even sort algorithm is a parallel sorting algorithm
  * that sorts the numbers in an array by comparing each pair of adjacent
  * elements and swapping them if they are in the wrong order.
 */
void oddEvenSort(size_t n, double* arr, int threadAmount);
/**
 * @brief Changes the position of two numbers in an array
 * @param a is the first number
 * @param b is the second number
 */
void swap(double* a, double* b);
/**
 * @brief Prints an array of numbers
 * @param numbers array of numbers to print
 * @param numberAmount size of the array
 */
void printNumbs(double* numbers, int numberAmount);
/**
 * @brief Returns true if the array is sorted
 * @see swap
 * @return true if the array is in order, false otherwise
 */
bool isOrdered(double* numbers, int numberAmount);

int main(int argc, char* argv[]) {
  int numberAmount = 0;
  int threadCount = omp_get_max_threads();
  if (argc >= 2) {
    numberAmount = atoi(argv[1]);
    if (argc >= 3) {
      threadCount = atoi(argv[2]);
    }
    double* numbers = new double[numberAmount];
    genRandNumbers(numberAmount, numbers);
    printNumbs(numbers, numberAmount);
    oddEvenSort(numberAmount, numbers, threadCount);
    std::cout << "ordered" << std::endl;
    printNumbs(numbers, numberAmount);
    std::cout <<
    (isOrdered(numbers, numberAmount)? "All in order" : "Not in order")
    << std::endl;
    delete[] numbers;
  }
  return 0;
}

void genRandNumbers(int amount, double* numbers) {
  size_t upper_limit = 99;
  UniformRandom<float> uniformRandom;
  uniformRandom.between(0, 99);
  for (int num = 0; num < amount; num++) {
    numbers[num] = uniformRandom.between(0, 100);
  }
}

void oddEvenSort(size_t n, double* arr, int threadCount) {
  for (int phase = 0; phase < n; ++phase) {
    if (phase % 2 == 0) {
      #pragma omp parallel for num_threads(threadCount) \
      default(none) shared(arr, n)
      for (int i = 1; i < n; i += 2) {
        if (arr[i - 1] > arr[i]) {
          #pragma omp critical(canAccessArr)
          swap(&arr[i - 1], &arr[i]);
        }
      }
    } else {
      #pragma omp parallel for num_threads(threadCount) \
      default(none) shared(arr, n)
      for (int i = 1; i < n - 1; i += 2) {
        if (arr[i] > arr[i + 1]) {
          #pragma omp critical(canAccessArr)
          swap(&arr[i], &arr[i + 1]);
        }
      }
    }
  }
}

void swap(double* a, double* b) {
  double temp = *a;
  *a = *b;
  *b = temp;
}
void printNumbs(double* numbers, int numberAmount) {
  std::cout << "numbers:" << std::endl;
  for (int num = 0; num < numberAmount; ++num) {
    std::cout << numbers[num] << std::endl;
  }
}
bool isOrdered(double* numbers, int numberAmount) {
  for (int number = 1; number < numberAmount; number++) {
    if (numbers[number - 1] > numbers[number]) {
      return false;
    }
  }

  return true;
}
