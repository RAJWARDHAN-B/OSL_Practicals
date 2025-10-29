#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>


#define BUFFER_SIZE 5   // fixed buffer size


sem_t full;
sem_t empty;
pthread_mutex_t mutex;


int in = 0;
int out = 0;
int item = 0;
int buffer[BUFFER_SIZE];


void *producer(void *arg) {
   while (1) {
       int produced_item = ++item;   // produce new item


       sem_wait(&empty);             // wait for empty slot
       pthread_mutex_lock(&mutex);   // enter critical section


       buffer[in] = produced_item;
       printf("Producer %lu produced: %d at buffer[%d]\n",
              pthread_self(), produced_item, in);
       in = (in + 1) % BUFFER_SIZE;  // circular increment


       pthread_mutex_unlock(&mutex); // exit critical section
       sem_post(&full);              // signal that buffer has item


       sleep(1); // slow down output
   }
   return NULL;
}


void *consumer(void *arg) {
   while (1) {
       sem_wait(&full);              // wait for full slot
       pthread_mutex_lock(&mutex);   // enter critical section


       int consumed_item = buffer[out];
 printf("Consumer %lu consumed: %d from buffer[%d]\n",
              pthread_self(), consumed_item, out);
       out = (out + 1) % BUFFER_SIZE; // circular increment


       pthread_mutex_unlock(&mutex); // exit critical section
       sem_post(&empty);             // signal empty slot available


       sleep(1); // slow down output
   }
   return NULL;
}


int main() {
   int n;
   printf("Enter number of producers/consumers: ");
   scanf("%d", &n);


   sem_init(&empty, 0, BUFFER_SIZE);
   sem_init(&full, 0, 0);
   pthread_mutex_init(&mutex, NULL);


   pthread_t *produce = malloc(n * sizeof(pthread_t));
   pthread_t *consum = malloc(n * sizeof(pthread_t));


   for (int i = 0; i < n; i++) {
       pthread_create(&produce[i], NULL, producer, NULL);
       pthread_create(&consum[i], NULL, consumer, NULL);
   }


   // Let them run for some time (infinite otherwise)
   sleep(15);
   printf("Main thread exiting...\n");


   free(produce);
   free(consum);
   sem_destroy(&empty);
   sem_destroy(&full);
   pthread_mutex_destroy(&mutex);


   return 0;
}


