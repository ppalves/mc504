#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
    sleep(rand()%2 + 1) ;
    pthread_mutex_lock(&print_mutex);

    /*Clear console https://stackoverflow.com/questions/2347770/how-do-you-clear-the-console-screen-in-c */
    printf("\e[1;1H\e[2J");
    
    printf("0xygens in bequer: %d\n", oxygen);
    printf("Hydrogens in bequer: %d\n", hydrogen);
    printf("H20 molecules generated: %d\n\n", h20);

    // imprime o becker
    for (int i = N_OXYGEN; i > 0; i--) {
        printf("|");
        if (i <= oxygen)
            printf("O   ");
        else
            printf("    ");
        if (i <= hydrogen)
            printf("    H");
        else
            printf("     ");
        printf("|\n");
    }
    printf("----   ----\n");

    //imprime as aguas geradas
    for (int i = 0; i < h20; i++)
    {
        printf("   ~~~~~\n");
        printf("   ~H20~\n");
        printf("   ~~~~~\n");

    }
    printf("\n\n");
    
    pthread_mutex_unlock(&print_mutex);
}

void bond() {
    h20++;
    print_becker();
}

void* f_oxygen(void* v) {

    //garante que a thread f_oxygen não entrará sempre antes de f_hydrogen na zona critica
    sleep(rand()%2) ;
    pthread_mutex_lock(&mutex);
    oxygen += 1;
    print_becker();
    if (hydrogen >= 2) {
        sem_post(&sem_hydro);
        sem_post(&sem_hydro);
        hydrogen -= 2;
        sem_post(&sem_oxy);
        oxygen -= 1;
    } else {
        pthread_mutex_unlock(&mutex);
    }
     
    sem_wait(&sem_oxy);
    bond();
    pthread_barrier_wait(&barrier);
    pthread_mutex_unlock(&mutex);
}

void* f_hydrogen(void* v) {
    sleep(rand()% 2);
    pthread_mutex_lock(&mutex);
    hydrogen += 1;
    print_becker();
    if (hydrogen >= 2 && oxygen >= 1) {
        sem_post(&sem_hydro);
        sem_post(&sem_hydro);
        hydrogen -= 2;
        sem_post(&sem_oxy);
        oxygen -= 1;
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