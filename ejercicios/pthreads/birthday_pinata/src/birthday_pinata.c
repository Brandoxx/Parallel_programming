// Copyright: Brandon Mora Umana
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// A shared data struct with a pinata and a counter
// for the number of times it has been hit.
// Protected by a mutex.
typedef struct {
    int pinataHealth;
    int threadHited;
    pthread_mutex_t mutex;
} pinata_t;

typedef struct {
    pinata_t *pinata;
    int hitsMade;
    int ID;
} thread_data_t;

void* hit_pinata(void* data);

int main(int argc, char *argv[]) {
    int num_threads = 0;
    int pinata_hits = 0;
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number of threads> <number of hits>\n"
         , argv[0]);
        return 1;
    }
    if (sscanf(argv[1], "%d", &num_threads) != 1) {
        fprintf(stderr, "Invalid number of threads: %s \n", argv[1]);
        return 1;
    }
    if (sscanf(argv[2], "%d", &pinata_hits) != 1) {
        fprintf(stderr, "Invalid number of hits: %s \n", argv[2]);
        return 1;
    }
    if (num_threads < 1) {
        fprintf(stderr, "Number of threads must be at least 1\n");
        return 1;
    }
    if (pinata_hits < 1) {
        fprintf(stderr, "Number of hits must be at least 1\n");
        return 1;
    }
    pthread_t* threads = malloc(num_threads * sizeof(pthread_t));
    thread_data_t* thread_data = malloc(num_threads * sizeof(thread_data_t));
    pinata_t pinata = {pinata_hits, 0, PTHREAD_MUTEX_INITIALIZER};
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].pinata = &pinata;
        thread_data[i].hitsMade = 0;
        thread_data[i].ID = i;
        pthread_create(&threads[i], NULL, hit_pinata, &thread_data[i]);
    }
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    free(threads);
    free(thread_data);
    return 0;
}

void* hit_pinata(void* data) {
    thread_data_t* thread_data = (thread_data_t*) data;
    pinata_t* pinata = thread_data->pinata;
    while (1) {
        pthread_mutex_lock(&pinata->mutex);
        if (pinata->pinataHealth == 0) {
            pthread_mutex_unlock(&pinata->mutex);
            break;
        }
        pinata->pinataHealth--;
        pinata->threadHited = thread_data->ID;
        pthread_mutex_unlock(&pinata->mutex);
        thread_data->hitsMade++;
    }
    if (pinata->threadHited == thread_data->ID) {
        printf("Thread %d hit the pinata %d times, and broke it!\n",
         thread_data->ID, thread_data->hitsMade);
    } else {
        printf("Thread %d hit the pinata %d times\n" ,
        thread_data->ID, thread_data->hitsMade);
    }
    return NULL;
}
