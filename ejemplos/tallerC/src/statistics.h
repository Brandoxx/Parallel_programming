#ifndef STATISTICS_H
#define STATISTICS_H

/**
 * @brief   Class for statistics.
 *
 * 
 */

typedef struct statistics statistics_t;

/**
 * @brief   Constructor for statistics.
 * @details An object of this class is used to store statistics.
 * @param   Statistics object.
 * @return  Pointer to statistics.
 */
statistics_t *statistics_create(void);

int statistics_run(statistics_t *statistics, int argc, char *argv[]);
/**
 * @brief   Destructor for statistics.
 * @details Frees memory allocated for statistics.
 * @param   Statistics object.
 */
void statistics_destroy(statistics_t *statistics);

#endif // STATISTICS_H