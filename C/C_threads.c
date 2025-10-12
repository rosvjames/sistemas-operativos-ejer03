/* Ejemplo de productor-consumidor con semáforos */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 8  // tamaño del buffer circular

static int buffer[N];
static int in_idx = 0, out_idx = 0;
static sem_t empty_sem, full_sem, mutex;

void* producer(void* arg) {
    int x = 1;
    (void)arg;
    for (;;) {
        // Espera espacio libre y lock de sección crítica
        sem_wait(&empty_sem);
        sem_wait(&mutex);

        buffer[in_idx] = x;
        in_idx = (in_idx + 1) % N;

        sem_post(&mutex);
        sem_post(&full_sem);
        
        printf("produced %d\n", x);
        x++;
        usleep(100000); // 100 ms
    }
    return NULL;
}

void* consumer(void* arg) {
    (void)arg;
    for (;;) {
        // Espera elementos disponibles y lock de sección crítica
        sem_wait(&full_sem);
        sem_wait(&mutex);

        int v = buffer[out_idx];
        out_idx = (out_idx + 1) % N;

        sem_post(&mutex);
        sem_post(&empty_sem);

        printf("consumed %d\n", v);
        usleep(150000); // 150 ms
    }
    return NULL;
}

int main(void) {
    pthread_t prod_tid, cons_tid;

    // Inicializaciones:
    // - empty = N (N espacios libres al inicio)
    // - full  = 0 (no hay ítems al inicio)
    // - mutex = 1 (binario)
    if (sem_init(&empty_sem, 0, N) != 0 ||
        sem_init(&full_sem,  0, 0) != 0 ||
        sem_init(&mutex,     0, 1) != 0) {
        perror("sem_init");
        return 1;
    }

    if (pthread_create(&prod_tid, NULL, producer, NULL) != 0 ||
        pthread_create(&cons_tid, NULL, consumer, NULL) != 0) {
        perror("pthread_create");
        return 1;
    }

    // Para demo: corre hasta Ctrl+C
    pthread_join(prod_tid, NULL);
    pthread_join(cons_tid, NULL);

    // Nunca llega aquí en este demo, pero por prolijidad:
    sem_destroy(&empty_sem);
    sem_destroy(&full_sem);
    sem_destroy(&mutex);
    return 0;
}
