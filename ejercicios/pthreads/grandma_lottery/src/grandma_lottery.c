// Copyright 2022 Brandon Mora Umana
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

void* grandson_Lottery(void* arg);
int main(void) {
    int error =  EXIT_SUCCESS;
    void* firstReturn = NULL;
    void* secondReturn = NULL;
    pthread_t first_grandson;
    pthread_t second_grandson;
    error = pthread_create(&first_grandson, NULL, grandson_Lottery, NULL);
    if (error == EXIT_SUCCESS) {
        error = pthread_create(&second_grandson, NULL, grandson_Lottery, NULL);
    }

    if (error == EXIT_SUCCESS) {
        pthread_join(first_grandson, &firstReturn);
        pthread_join(second_grandson, &secondReturn);
    }
    return error;
}

#if 0
void* grandson_Lottery(void* arg) {
    unsigned int seed = pthread_self() + time(NULL);
    srand(seed);
    int* lottery_number = calloc(1, sizeof(int));
    *lottery_number = rand_r(&seed) % 101;
    return lottery_number;
}
#endif

#if 1
void* grandson_Lottery(void* arg) {
    unsigned int seed = pthread_self() + time(NULL);
    srand(seed);
    int* lottery_number = calloc(1, sizeof(int));
    *lottery_number = rand_r(&seed) % 101;
    void* address = *lottery_number;
    free(lottery_number);
    return address;
}
#endif
