#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

#define SHM_SIZE 1024

int main() {
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    char* str = (char*) shmat(shmid, NULL, 0);
    if (str == (char*)-1) {
        perror("shmat");
        exit(1);
    }

    sem_t *sem_ready = sem_open("/sem_ready", O_CREAT, 0666, 0);
    sem_t *sem_processed = sem_open("/sem_processed", O_CREAT, 0666, 1);

    printf("Enter message for client: ");
    fgets(str, SHM_SIZE, stdin);

    // Signal client that data is ready
    sem_post(sem_ready);

    // Wait for client to process message
    sem_wait(sem_processed);

    shmdt(str);

    sem_close(sem_ready);
    sem_close(sem_processed);

    return 0;
}
