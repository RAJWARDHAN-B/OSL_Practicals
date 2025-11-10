// gcc sort_fork.c -o sort_fork
// ./sort_fork

// to demonstrate on terminal

// ps -el | grep Z
// ps -ef | grep sort_fork


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Function to perform Bubble Sort
void bubble_sort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Function to perform Insertion Sort
void insertion_sort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

int main() {
    int n, i;
    pid_t pid;

    printf("Enter number of integers: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter %d integers:\n", n);
    for (i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    pid = fork();  // Create a child process

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    else if (pid == 0) {
        // ---------- CHILD PROCESS ----------
        printf("\n[Child] Process ID: %d | Parent ID: %d\n", getpid(), getppid());
        printf("[Child] Performing Insertion Sort...\n");

        insertion_sort(arr, n);

        printf("[Child] Sorted array (Insertion Sort): ");
        for (i = 0; i < n; i++)
            printf("%d ", arr[i]);
        printf("\n");

        // Uncomment below line to demonstrate orphan process
        // sleep(5);

        printf("[Child] Exiting now...\n");
        exit(0);
    }
    else {
        // ---------- PARENT PROCESS ----------
        printf("\n[Parent] Process ID: %d | Child ID: %d\n", getpid(), pid);

        // Uncomment below line to demonstrate zombie process
        // sleep(10);

        wait(NULL);  // Wait for child to finish
        printf("[Parent] Child completed. Performing Bubble Sort...\n");

        bubble_sort(arr, n);

        printf("[Parent] Sorted array (Bubble Sort): ");
        for (i = 0; i < n; i++)
            printf("%d ", arr[i]);
        printf("\n");

        printf("[Parent] Exiting now...\n");
    }

    return 0;
}
