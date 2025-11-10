#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t wrt;
pthread_mutex_t mutex;

int readcount = 0;
int shared_data = 0;

void* reader(void* arg){
	int id = *((int*)arg);
	while(1){
		pthread_mutex_lock(&mutex);
		readcount++;
		if(readcount == 1){
			sem_wait(&wrt);
		}
		pthread_mutex_unlock(&mutex);
		printf("Reader %d is reading shared data =  %d\n" , id , shared_data);
		sleep(1);
		pthread_mutex_lock(&mutex);
		readcount--;
		if(readcount == 0){
			sem_post(&wrt);
		}
		pthread_mutex_unlock(&mutex);
	
		sleep(rand() % 3);
	}
	pthread_exit(NULL);
	
}

void* writer(void* arg){
	int id = *((int*)arg);
	while(1){
		sem_wait(&wrt);
		shared_data = rand()%1000;
		printf("Writer %d is writing shared data =  %d\n" , id , shared_data);
		sleep(2);
		sem_post(&wrt);
		sleep(rand() % 5);
	}

	pthread_exit(NULL);

}

int main(){
	int i;
	pthread_t r[5] , w[2];
	int reader_ids[5] , writer_ids[2];
	sem_init(&wrt , 0 , 1);
	pthread_mutex_init(&mutex , NULL);

	for(i = 0 ; i<5 ; i++){
		reader_ids[i] = i + 1;
		pthread_create(&r[i] , NULL , reader , &reader_ids[i]);
	
	}
	for(i = 0 ; i<2 ; i++){
		writer_ids[i] = i + 1;
		pthread_create(&w[i] , NULL , writer , &writer_ids[i]);
	
	}
	for(i = 0 ; i<5 ; i++) pthread_join(r[i] , NULL);
	for(i = 0 ; i<2 ; i++) pthread_join(w[i] , NULL);
	sem_destroy(&wrt);
	pthread_mutex_destroy(&mutex);
	return 0;
}


