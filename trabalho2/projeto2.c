#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define N_OXYGEN 10
#define N_HYDROGEN 20

int oxygen = 0;
int hydrogen = 0;
int h20 = 0;

sem_t sem_hydro;
sem_t sem_oxy;
pthread_mutex_t mutex;
pthread_mutex_t print_mutex;
pthread_barrier_t barrier;
int count = 0;

void print_becker() {
    for (int i = N_OXYGEN; i > 0; i--) {
        printf("|");
        if (i <= oxygen)
            printf("O   ");
        else
            printf("    ");
        if (i <= hydrogen)
            printf("   H");
        else
            printf("    ");
        printf("|\n");
    }
    printf("----  ----\n");
    printf("h20: %d\n\n\n", h20);
}

void bond() {
    h20++;
    print_becker();
}

void* f_oxygen(void* v) {
    pthread_mutex_lock(&mutex);
    oxygen += 1;
    print_becker();
    if (hydrogen >= 2) {
        sem_post(&sem_hydro);
        sem_post(&sem_hydro);
        hydrogen -= 2;
        sem_post(&sem_oxy);
        oxygen -= 1;
        print_becker();
    } else {
        pthread_mutex_unlock(&mutex);
    }
     
    sem_wait(&sem_oxy);
    pthread_mutex_lock(&print_mutex);
    bond();
    pthread_mutex_unlock(&print_mutex);
    pthread_barrier_wait(&barrier);
    pthread_mutex_unlock(&mutex);
}

void* f_hydrogen(void* v) {
    pthread_mutex_lock(&mutex);
    hydrogen += 1;
    print_becker();
    if (hydrogen >= 2 && oxygen >= 1) {
        sem_post(&sem_hydro);
        sem_post(&sem_hydro);
        hydrogen -= 2;
        sem_post(&sem_oxy);
        oxygen -= 1;
        print_becker();
    } else {
        pthread_mutex_unlock(&mutex);
    }
    
    sem_wait(&sem_hydro);
    pthread_barrier_wait(&barrier);
}

int main() {
    int i = 0,j=0;
    int id_oxy[N_OXYGEN];
    int id_hydro[N_HYDROGEN];

    pthread_t thr_oxy[N_OXYGEN], thr_hydro[N_HYDROGEN];

    sem_init(&sem_hydro, 0, 0);
    sem_init(&sem_oxy, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&print_mutex, NULL);
    pthread_barrier_init(&barrier, 0, 3);

    for (i = 0; i < N_OXYGEN; i++) {
        pthread_create(&thr_oxy[i], NULL, f_oxygen, (void*) &id_oxy[i]);
    }
    for ( i = 0; i < N_HYDROGEN; i++) {
        pthread_create(&thr_hydro[i], NULL, f_hydrogen, (void*) &id_hydro[i]);
    }
    
    for (i = 0; i < N_OXYGEN; i++) {
        pthread_join(thr_oxy[i], NULL);
    }
    for ( i = 0; i < N_HYDROGEN; i++) {
        pthread_join(thr_hydro[i], NULL);
    }

}