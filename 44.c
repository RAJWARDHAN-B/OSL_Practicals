#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int read_count = 0;
pthread_mutex_t mutex;     // Protects read_count
sem_t rw_mutex;            // Controls access to shared data

void *reader(void *arg) {
    int id = *(int *)arg;

    while (1) {
        pthread_mutex_lock(&mutex);
        read_count++;
        if (read_count == 1) {
            sem_wait(&rw_mutex); // First reader locks the resource
        }
        pthread_mutex_unlock(&mutex);

        // Reading section
        printf("📖 Reader %d is reading flight info...\n", id);
        sleep(rand() % 2 + 1);

        pthread_mutex_lock(&mutex);
        read_count--;
        if (read_count == 0) {
            sem_post(&rw_mutex); // Last reader unlocks the resource
        }
        pthread_mutex_unlock(&mutex);

        sleep(rand() % 3 + 1); // Wait before reading again
    }
    return NULL;
}

void *writer(void *arg) {
    int id = *(int *)arg;

    while (1) {
        sem_wait(&rw_mutex); // Writer gets exclusive access

        // Writing section
        printf("✈️ Writer %d is updating flight information!\n", id);
        sleep(rand() % 3 + 1);
        printf("✅ Writer %d finished updating flights.\n", id);

        sem_post(&rw_mutex); // Release resource
        sleep(rand() % 4 + 1);
    }
    return NULL;
}

int main() {
    srand(time(NULL));

    int num_readers = 3, num_writers = 2;
    pthread_t readers[num_readers], writers[num_writers];
    int reader_ids[num_readers], writer_ids[num_writers];

    pthread_mutex_init(&mutex, NULL);
    sem_init(&rw_mutex, 0, 1);

    for (int i = 0; i < num_readers; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    for (int i = 0; i < num_writers; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    for (int i = 0; i < num_readers; i++)
        pthread_join(readers[i], NULL);
    for (int i = 0; i < num_writers; i++)
        pthread_join(writers[i], NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&rw_mutex);

    return 0;
}
