// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#ifndef PRODUCER_H
#define PRODUCER_H
/**
 * @brief Produces the units in the queue
 * 
 * @param data The shared data for all threads
 * @return void* 
 */
void* produce(void* data);

#endif  // PRODUCER_H
