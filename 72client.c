#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>

#define SHM_SIZE 1024

int main() {
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, SHM_SIZE, 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    char* str = (char*) shmat(shmid, NULL, 0);
    if (str == (char*)-1) {
        perror("shmat");
        exit(1);
    }

    sem_t *sem_ready = sem_open("/sem_ready", 0);
    sem_t *sem_processed = sem_open("/sem_processed", 0);

    // Wait for server to write
    sem_wait(sem_ready);

    printf("Message from server: %s\n", str);

    // Signal server that message read
    sem_post(sem_processed);

    shmdt(str);

    sem_close(sem_ready);
    sem_close(sem_processed);

    return 0;
}
