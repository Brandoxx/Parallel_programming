// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#ifndef SIMULATION_H
#define SIMULATION_H
/**
 * @brief simulation data
 * 
 */
typedef struct simulation simulation_t;  // opaque record
/**
 * @brief Initializes the simulation
 * 
 */
simulation_t* simulation_create();
/**
 * @brief runs the simulation
 * 
 */
int simulation_run(simulation_t* simulation, int argc, char* argv[]);
/**
 * @brief destroys a simulation
 * 
 * @param simulation simulation to destroy
 */
void simulation_destroy(simulation_t* simulation);

#endif  // SIMULATION_H
