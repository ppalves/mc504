#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define N_HACKERS 10
#define N_SERFS 10
int hackers = 0;
int serfs = 0;
int hackers_pass = 0;
int serfs_pass = 0;
int boats = 0;

sem_t sem_serfs;
sem_t sem_hack;
pthread_mutex_t mutex;
pthread_mutex_t print_mutex;
pthread_mutex_t board_mutex;
pthread_barrier_t barrier;

void print_boat() {
    sleep(rand()%2 + 1) ;
    pthread_mutex_lock(&print_mutex);

    /*Clear console https://stackoverflow.com/questions/2347770/how-do-you-clear-the-console-screen-in-c */
    printf("\e[1;1H\e[2J");
    
    printf("hackers waiting: %d\n", hackers);
    printf("serfs waiting: %d\n", serfs);
    printf("boats departure: %d\n\n", boats);

    for (int i = 5; i > 0; i--) {
        printf("|");
        if (i <= hackers)
            printf("H   ");
        else
            printf("    ");
        if (i <= serfs)
            printf("      S");
        else
            printf("       ");
        printf("|\n");
    }
    printf("----     ----\n");

    printf("   ~~~~~~~\n");
    printf("   ~-----~\n");
    for (int k = 0; k < hackers_pass; k++){
        printf("   ~| H |~\n");        
    }
    for (int k = 0; k < serfs_pass; k++){
        printf("   ~| S |~\n");        
    }
    for (int k = 0; k < 4 - (hackers_pass + serfs_pass); k++){
        printf("   ~|   |~\n");        
    }
    printf("   ~-----~\n");
    printf("   ~~~~~~~\n");

    //imprime os barcos que partiram geradas
    for (int i = 0; i < boats; i++)
    {
        printf("   ~~~~~~~\n");
        printf("   ~-----~\n");
        printf("   ~| * |~\n");
        printf("   ~| * |~\n");
        printf("   ~| * |~\n");
        printf("   ~| * |~\n");
        printf("   ~-----~\n");
        printf("   ~~~~~~~\n");

    }
    printf("\n\n");
    
    pthread_mutex_unlock(&print_mutex);
}

void board(int type) {
    // type == 0 -> hacker
    // type == 1 -> serf
    pthread_mutex_lock(&board_mutex);
    if (type == 0) {
        hackers_pass += 1;
    } else if (type == 1){
        serfs_pass += 1;
    }
    pthread_mutex_unlock(&board_mutex);
    print_boat();
}

void row_boat() {
    boats++;
    hackers_pass = 0;
    serfs_pass = 0;
    print_boat();
}

void* f_hackers(void* v) {
    sleep(rand()%2);
    pthread_mutex_lock(&mutex);
    hackers += 1;
    int is_captain = 0;
    print_boat();
    if (hackers == 4) {
        sem_post(&sem_hack);
        sem_post(&sem_hack);
        sem_post(&sem_hack);
        sem_post(&sem_hack);
        hackers = 0;
        is_captain = 1;
    }
    else if (hackers == 2 && serfs >= 2) {
        sem_post(&sem_hack);
        sem_post(&sem_hack);
        sem_post(&sem_serfs);
        sem_post(&sem_serfs);
        serfs -= 2;
        hackers = 0;
        is_captain = 1;
    }
    else {
        pthread_mutex_unlock(&mutex);
    }
     
    sem_wait(&sem_hack);
    board(0);
    pthread_barrier_wait(&barrier);

    if (is_captain == 1){
        row_boat();
        pthread_mutex_unlock(&mutex);
    }
}

void* f_serfs(void* v) {
    sleep(rand()% 2);
    pthread_mutex_lock(&mutex);
    serfs += 1;
    int is_captain = 0;
    print_boat();

    if (serfs == 4) {
        sem_post(&sem_serfs);
        sem_post(&sem_serfs);
        sem_post(&sem_serfs);
        sem_post(&sem_serfs);
        serfs = 0;
        is_captain = 1;
    }
    else if (serfs == 2 && hackers >= 2) {
        sem_post(&sem_hack);
        sem_post(&sem_hack);
        sem_post(&sem_serfs);
        sem_post(&sem_serfs);
        hackers -= 2;
        serfs = 0;
        is_captain = 1;
    }
    else {
        pthread_mutex_unlock(&mutex);
    }
    
    sem_wait(&sem_serfs);
    board(1);
    pthread_barrier_wait(&barrier);

    if (is_captain == 1){
        row_boat();
        pthread_mutex_unlock(&mutex);
    }
}

int main() {
    int i;
    int id_hack[N_HACKERS];
    int id_serfs[N_SERFS];

    pthread_t thr_hack[N_HACKERS], thr_serfs[N_SERFS];

    sem_init(&sem_serfs, 0, 0);
    sem_init(&sem_hack, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&print_mutex, NULL);
    pthread_mutex_init(&board_mutex, NULL);
    pthread_barrier_init(&barrier, 0, 4);

    for (i = 0; i < N_HACKERS; i++) {
        pthread_create(&thr_hack[i], NULL, f_hackers, (void*) &id_hack[i]);
    }
    for (i = 0; i < N_SERFS; i++) {
        pthread_create(&thr_serfs[i], NULL, f_serfs, (void*) &id_serfs[i]);
    }
    
    for (i = 0; i < N_HACKERS; i++) {
        pthread_join(thr_hack[i], NULL);
    }
    for ( i = 0; i < N_SERFS; i++) {
        pthread_join(thr_serfs[i], NULL);
    }
}
