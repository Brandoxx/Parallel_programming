// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#ifndef CONSUMER_H
#define CONSUMER_H
/**
 * @brief Consumes the units in the queue
 * 
 * @param data The shared data for all threads
 * @return void* 
 */
void* consume(void* data);

#endif  // CONSUMER_H
