#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_STUDENTS 5
#define NUM_CHAIRS 3

// Shared variables
int waiting_students = 0;
sem_t sem_students; // Counts waiting students
sem_t sem_ta;       // Signals TA is ready to help
pthread_mutex_t mutex;

// ---------- TA THREAD FUNCTION ----------
void *ta_function(void *arg) {
    while (1) {
        // Sleep if no students waiting
        sem_wait(&sem_students);

        // Critical section
        pthread_mutex_lock(&mutex);
        waiting_students--;
        printf("🧑‍🏫 TA starts helping a student. Students waiting: %d\n", waiting_students);
        pthread_mutex_unlock(&mutex);

        // Simulate helping student
        sleep(rand() % 3 + 1);

        printf("🧑‍🏫 TA finished helping a student.\n");
        sem_post(&sem_ta); // Signal that TA is done helping
    }
    return NULL;
}

// ---------- STUDENT THREAD FUNCTION ----------
void *student_function(void *arg) {
    int id = *(int *)arg;

    while (1) {
        printf("🎓 Student %d is doing programming work.\n", id);
        sleep(rand() % 5 + 1); // Simulate work before asking for help

        pthread_mutex_lock(&mutex);
        if (waiting_students < NUM_CHAIRS) {
            waiting_students++;
            printf("🎓 Student %d is waiting. Waiting students = %d\n", id, waiting_students);
            pthread_mutex_unlock(&mutex);

            sem_post(&sem_students);  // Notify TA that a student is waiting
            sem_wait(&sem_ta);        // Wait until TA finishes helping

            printf("🎓 Student %d got help from the TA and leaves.\n", id);
        } else {
            pthread_mutex_unlock(&mutex);
            printf("🚪 Student %d found no empty chair and will come back later.\n", id);
        }
    }
    return NULL;
}

// ---------- MAIN FUNCTION ----------
int main() {
    srand(time(NULL));

    pthread_t ta_thread;
    pthread_t student_threads[NUM_STUDENTS];
    int student_ids[NUM_STUDENTS];

    // Initialize semaphores and mutex
    sem_init(&sem_students, 0, 0);
    sem_init(&sem_ta, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    // Create TA thread
    pthread_create(&ta_thread, NULL, ta_function, NULL);

    // Create student threads
    for (int i = 0; i < NUM_STUDENTS; i++) {
        student_ids[i] = i + 1;
        pthread_create(&student_threads[i], NULL, student_function, &student_ids[i]);
    }

    // Join threads (this program runs indefinitely; use Ctrl+C to stop)
    pthread_join(ta_thread, NULL);
    for (int i = 0; i < NUM_STUDENTS; i++)
        pthread_join(student_threads[i], NULL);

    // Cleanup (unreachable due to infinite loop)
    sem_destroy(&sem_students);
    sem_destroy(&sem_ta);
    pthread_mutex_destroy(&mutex);

    return 0;
}
