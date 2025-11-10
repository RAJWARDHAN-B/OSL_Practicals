// gcc producer_consumer.c -o producer_consumer -lpthread
// ./producer_consumer


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5  // Pizza counter capacity

int counter[BUFFER_SIZE];  // Shared pizza counter
int in = 0, out = 0;       // Indexes for producer and consumer

sem_t empty;   // Semaphore for empty slots
sem_t full;    // Semaphore for filled slots
pthread_mutex_t mutex;  // Mutex lock for critical section

// ---------- PRODUCER FUNCTION (Mr. Simpson) ----------
void *producer(void *arg) {
    int item = 0;
    while (1) {
        // Produce a pizza
        item++;
        sleep(rand() % 3);  // Simulate baking time

        sem_wait(&empty);            // Wait for an empty slot
        pthread_mutex_lock(&mutex);  // Enter critical section

        // Place pizza on counter
        counter[in] = item;
        printf("🍕 Mr. Simpson baked Pizza #%d and placed it on counter slot %d.\n", item, in);
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);  // Leave critical section
        sem_post(&full);               // Increase count of full slots
    }
    return NULL;
}

// ---------- CONSUMER FUNCTION (Joey Tribbiani) ----------
void *consumer(void *arg) {
    int item;
    while (1) {
        sem_wait(&full);              // Wait for available pizza
        pthread_mutex_lock(&mutex);   // Enter critical section

        // Consume pizza from counter
        item = counter[out];
        printf("😋 Joey ate Pizza #%d from counter slot %d.\n", item, out);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex); // Leave critical section
        sem_post(&empty);             // Increase count of empty slots

        sleep(rand() % 4);            // Simulate eating time
    }
    return NULL;
}

// ---------- MAIN FUNCTION ----------
int main() {
    pthread_t prod_thread, cons_thread;

    // Initialize semaphores and mutex
    sem_init(&empty, 0, BUFFER_SIZE);  // Counter starts empty
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    printf("🍽️  Simulation started: Mr. Simpson (Producer) and Joey (Consumer)\n");
    printf("----------------------------------------------------------\n");

    // Create threads for producer and consumer
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    // Join threads (infinite simulation — use Ctrl+C to stop)
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    // Destroy semaphores and mutex (won’t be reached in this infinite example)
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
