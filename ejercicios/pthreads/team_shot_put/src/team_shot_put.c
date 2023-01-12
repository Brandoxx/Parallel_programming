// Copyright Brandon Mora Umana 2022
#include <stdio.h>
#include <pthread.h>  // invokes posix threads
#include <stdlib.h>
#include <unistd.h>

#define MAX_SHOTS 3
#define MAX_DISTANCE 25.0
#define MAX_TEAMS 2

void *shot_put_player(void *arg);

int main(int argc, char *argv[]) {
    if ((argc != 2) || (strtol(argv[1], NULL, 10) % 2 == 0)) {
        fprintf(stderr, "error: athlete count must be an odd number");
        return 1;
    }
    int numberOfPlayers = strtol(argv[1], NULL, 10);
    pthread_t* FirsTeamAthletes = calloc(numberOfPlayers, sizeof(pthread_t));
    pthread_t* SecondTeamAthletes = calloc(numberOfPlayers, sizeof(pthread_t));
    double firstTeam = 1.0;
    double secondTeam = 2.0;
    void* firstTeamDistance = NULL;
    void *secondTeamDistance = NULL;
    int firstTeamScore = 0;
    int secondTeamScore = 0;
    for (int i = 0; i < numberOfPlayers; i++) {
        for (int j = 0; j < MAX_TEAMS; j++) {
            if (j == 0) {
                firstTeam += 0.1;
                double* playerNumber = malloc(sizeof(double));
                *playerNumber = firstTeam;
                pthread_create(&FirsTeamAthletes[i], NULL,
                shot_put_player, playerNumber);
            } else {
                secondTeam += 0.1;
                double* playerNumber = malloc(sizeof(double));
                *playerNumber = secondTeam;
                pthread_create(&SecondTeamAthletes[i], NULL,
                shot_put_player, playerNumber);
            }
        }
    }
    for (int i = 0; i < numberOfPlayers; i++) {
        pthread_join(FirsTeamAthletes[i], &firstTeamDistance);
        pthread_join(SecondTeamAthletes[i], &secondTeamDistance);
        double* firstTeamDistanceDouble = ((double*)firstTeamDistance);
        double* secondTeamDistanceDouble = ((double*)secondTeamDistance);
        if (*firstTeamDistanceDouble > *secondTeamDistanceDouble) {
            firstTeamScore++;
        } else if (*firstTeamDistanceDouble < *secondTeamDistanceDouble) {
            secondTeamScore++;
        }
        free(firstTeamDistanceDouble);
        free(secondTeamDistanceDouble);
    }
    printf("result: %d:%d", firstTeamScore, secondTeamScore);
    if (firstTeamScore > secondTeamScore) {
        printf(" (team 1 wins) \n");
    } else if (firstTeamScore < secondTeamScore) {
        printf(" (team 2 wins) \n");
    } else {
        printf(" (draw) \n");
    }
    free(FirsTeamAthletes);
    free(SecondTeamAthletes);
}

void *shot_put_player(void *arg) {
    unsigned int seed = pthread_self() + (unsigned int)time(NULL);
    double *playerNumber = (double*) arg;
    srand(seed);
    double* bestShot = calloc(1, sizeof(double));
    for (int shotNum = 0; shotNum < MAX_SHOTS; shotNum++) {
        double shot = (double) rand_r(&seed) / RAND_MAX * MAX_DISTANCE;
        if (shot > *bestShot) {
            *bestShot = shot;
        }
    }
    printf("Player %.1f shot %.1f meters\n", *playerNumber, *bestShot);
    free(playerNumber);
    return bestShot;
}
