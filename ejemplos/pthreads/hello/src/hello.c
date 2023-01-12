#include <stdio.h> 
#include <pthread.h> // invokes posix threads
#include <stdlib.h>

/* @brief Prints hello from a secondary thread.
 * @param void
 * @return NULL
 */

void* greet (void* arg);


/* @brief Prints hello from the main thread.
 * @param void
 * @return NULL
 */
int main(void) {
    pthread_t thread;  // create a thread
    int error = pthread_create(&thread, NULL, greet, NULL);  // creates a thread, and passes in a function to run.
    if (error != 0) { // if error
        fprintf(stderr, "Error creating thread\n");  // error creating thread
    } else {
        printf("Hello from main thread\n");  // print from main thread
        pthread_join(thread, NULL);  // join the thread, and wait for it to finish.
    } 

    return error;  // return error 
}


void* greet (void* arg){
    printf("Hello from secondary thread\n");  // print from secondary thread
    return NULL;  // return null
}





// Un programa que imprima un saludo dependiendo de dos threads

// procedure main:
// create_thread(greet)

// print "Hello from main thread"

// end procedure


// procedure greet:

// print "Hello from secondary thread"

// end procedure