#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

// Function to check if a number is prime
int is_prime(int num) {
    if (num < 2)
        return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    pid_t pid;

    // Check if a command-line argument is provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <positive integer>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Convert argument to integer
    int n = atoi(argv[1]);

    // Validate input
    if (n <= 0) {
        fprintf(stderr, "Error: Please enter a positive integer greater than 0.\n");
        exit(EXIT_FAILURE);
    }

    // Create a child process
    pid = fork();

    if (pid < 0) {
        // fork() failed
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        // ---------- CHILD PROCESS ----------
        printf("\n[Child] Process ID: %d | Parent ID: %d\n", getpid(), getppid());
        printf("[Child] Generating first %d prime numbers:\n", n);

        int count = 0, num = 2;
        while (count < n) {
            if (is_prime(num)) {
                printf("%d ", num);
                count++;
            }
            num++;
        }
        printf("\n[Child] Prime generation complete.\n");

        exit(EXIT_SUCCESS);
    }
    else {
        // ---------- PARENT PROCESS ----------
        printf("[Parent] Process ID: %d | Waiting for child (PID: %d)...\n", getpid(), pid);

        // Wait for child to complete
        wait(NULL);

        printf("[Parent] Child process completed. Exiting now.\n");
    }

    return 0;
}
