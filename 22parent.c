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

int main() {
    int n, i, item;
    pid_t pid;

    // Accept array from user
    printf("Enter number of integers: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter %d integers:\n", n);
    for (i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    // Sort the array
    bubble_sort(arr, n);
    printf("\nSorted Array: ");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Fork the process
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    else if (pid == 0) {
        // Child process: Execute binary search program
        char arr_str[10];
        char item_str[10];
        
        // Convert array and item to strings for command line args
        char *args[n + 3];  // n elements + 2 for program name and item to search
        args[0] = "./binary_search_program";  // Path to binary search program

        for (i = 0; i < n; i++) {
            snprintf(arr_str, sizeof(arr_str), "%d", arr[i]);
            args[i + 1] = strdup(arr_str);
        }

        // Accept the item to search
        printf("\nEnter the integer to search in the sorted array: ");
        scanf("%d", &item);
        snprintf(item_str, sizeof(item_str), "%d", item);
        args[n + 1] = strdup(item_str);
        args[n + 2] = NULL;

        // Execute binary search program
        execve(args[0], args, NULL);

        // If execve fails
        perror("Execve failed");
        exit(1);
    }
    else {
        // Parent process: Wait for child to finish
        wait(NULL);
        printf("[Parent] Child completed.\n");
    }

    return 0;
}
