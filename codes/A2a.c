#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Bubble Sort function
void bubble_sort(int arr[], int n) {
    int temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Print array
void print_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr1[n], arr2[n];
    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr1[i]);
        arr2[i] = arr1[i]; // Copy for child process
    }

    pid_t pid = fork();

    if (pid < 0) {
        // Fork failed
        printf("Error: Fork failed\n");
        exit(1);
    } 
    else if (pid == 0) {
        // Child process
        printf("Child process (PID: %d) is sorting the array...\n", getpid());
        bubble_sort(arr2, n);
        printf("Child process (PID: %d) sorting completed.\n", getpid());
        print_array(arr2, n);

        // Optional: Demonstrate orphan process
        // printf("Child sleeping for 15 seconds to demonstrate orphan process...\n");
        // sleep(15);
        // printf("Child waking up as orphan adopted by init process.\n");

        printf("Child process exiting...\n");
        exit(0);
    } 
    else {
        // Parent process
        printf("Parent process (PID: %d) is sorting the array...\n", getpid());
        bubble_sort(arr1, n);
        printf("Parent process (PID: %d) sorting completed.\n", getpid());
        print_array(arr1, n);

        // Optional: Demonstrate zombie process
        // printf("Parent sleeping for 10 seconds to allow child to become zombie...\n");
        // sleep(10);

        wait(NULL); // Wait for child process to finish
        printf("Parent finished waiting for child.\n");
        printf("Parent process exiting...\n");
        exit(0);
    }

    return 0;
}
