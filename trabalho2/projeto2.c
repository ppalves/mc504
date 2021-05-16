#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define N_OXIGENIO 5
#define n_HIDROGENIO 10

int oxigenio = 0;
int hidrogenio = 0;

sem_t sem_hidro;
sem_t sem_oxi;

void* f_oxigenio(void* v) {
    mutex.wait();
    oxygen += 1;
    if (hydrogen >= 2) {
        hydroQueue.signal(2);
        hydrogen -= 2;
        oxyQueue.signal();
        oxygen -= 1;
    } else {
        mutex.signal();
    }
     
    oxyQueue.wait();
    bond();
    barrier.wait();
    mutex.signal();
}

void* f_hidrogenio(void* v) {
    mutex.wait();
    hydrogen += 1;
    if (hydrogen >= 2 && oxygen >= 1) {
        hydroQueue.signal(2);
        hydrogen -= 2;
        oxyQueue.signal();
        oxygen -= 1;
    } else {
        mutex.signal();
    }
    
    hydroQueue.wait();
    bond();
    barrier.wait();
}

int main() {
    int i = 0;
    int id[N_OXIGENIO];

    pthread_t thr_oxi[N_OXIGENIO], thr_hidro[N_HIDROGENIO];

    sem_init(&sem_hidro, 0, 0);
    sem_init(&sem_oxi, 0, 0);

    for (i = 0; i < 30; i++) {
        if (i % 3 == 0) {
            pthread_create(&thr_oxigenio[i], NULL, f_oxigenio, (void*) &idi[i]);
        } else {
            pthread_create(&thr_hidrogenio[i], NULL, f_hidrogenio, (void*) &idi[i]);
        }
    }

    for (i = 0; i < 30; i++) {
        pthread_join(thr_oxigenio[i], NULL);
        pthread_join(thr_hidrogenio[i], NULL);
    }

}