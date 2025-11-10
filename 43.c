#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

sem_t sem_empty;  // Indicates buffer (file) is empty
sem_t sem_full;   // Indicates buffer (file) is full
pthread_mutex_t mutex;  // Protects file access

void *producer(void *arg) {
    FILE *fp;
    int num;

    while (1) {
        sem_wait(&sem_empty);          // Wait if file not yet read
        pthread_mutex_lock(&mutex);

        num = rand() % 10;
        fp = fopen("shared_data.txt", "w");
        if (fp == NULL) {
            perror("File open failed by Producer");
            exit(1);
        }
        fprintf(fp, "%d\n", num);
        fclose(fp);

        printf("🧑‍🍳 Producer produced: %d\n", num);

        pthread_mutex_unlock(&mutex);
        sem_post(&sem_full);           // Signal consumer that data is ready

        // Sleep for 0–100 ms
        usleep((rand() % 101) * 1000);
    }
    return NULL;
}

void *consumer(void *arg) {
    FILE *fp;
    int num;

    while (1) {
        sem_wait(&sem_full);           // Wait until Producer writes a value
        pthread_mutex_lock(&mutex);

        fp = fopen("shared_data.txt", "r");
        if (fp == NULL) {
            perror("File open failed by Consumer");
            exit(1);
        }
        fscanf(fp, "%d", &num);
        fclose(fp);

        printf("😋 Consumer consumed: %d\n", num);

        pthread_mutex_unlock(&mutex);
        sem_post(&sem_empty);          // Signal producer that file is empty
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t prod_thread, cons_thread;

    sem_init(&sem_empty, 0, 1);  // File initially empty
    sem_init(&sem_full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);

    return 0;
}
