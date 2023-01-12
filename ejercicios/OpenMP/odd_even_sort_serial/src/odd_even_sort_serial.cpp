// Copyright 2022 Brandon Mora Umana GNU GENERAL PUBLIC LICENSE
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
void serialOddEvenSort(size_t n, double* arr);
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
void printNumbs(double* numbers, int number_amount);

int main(int argc, char* argv[]) {
  int number_amount = 0;
  if (argc >= 2) {
    number_amount = atoi(argv[1]);
    double* numbers = new double[number_amount];
    genRandNumbers(number_amount, numbers);
    printNumbs(numbers, number_amount);
    serialOddEvenSort(number_amount, numbers);
    std::cout << "ordered" << std::endl;
    printNumbs(numbers, number_amount);
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

void serialOddEvenSort(size_t n, double* arr) {
  bool isSorted = false;
  while (!isSorted) {
    isSorted = true;
    for (int i = 1; i <= n - 2; i = i + 2) {
      if (arr[i] > arr[i + 1]) {
        swap(&arr[i], &arr[i + 1]);
        isSorted = false;
      }
    }
    for (int i = 0; i <= n - 2; i = i + 2) {
      if (arr[i] > arr[i + 1]) {
        swap(&arr[i], &arr[i + 1]);
        isSorted = false;
      }
    }
  }
}
void swap(double* a, double* b) {
  double temp = *a;
  *a = *b;
  *b = temp;
}
void printNumbs(double* numbers, int number_amount) {
  std::cout << "numbers:" << std::endl;
  for (int num = 0; num < number_amount; ++num) {
    std::cout << numbers[num] << std::endl;
  }
}
