#ifndef ARRAY_DOUBLE_H
#define ARRAY_DOUBLE_H

#include <stddef.h>

typedef struct {
    size_t capacity;
    size_t count;
    double* elements;

} array_double_t;

int array_double_init(array_double_t* array);

void array_double_destroy(array_double_t* array);

int array_double_append(array_double_t* array, double element);




#endif // ARRAY_DOUBLE_H
