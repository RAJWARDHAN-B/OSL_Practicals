#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to perform Binary Search
int binary_search(int arr[], int n, int item) {
    int low = 0, high = n - 1, mid;

    while (low <= high) {
        mid = low + (high - low) / 2;

        // Check if item is present at mid
        if (arr[mid] == item)
            return mid;

        // If item is greater, ignore left half
        if (arr[mid] < item)
            low = mid + 1;

        // If item is smaller, ignore right half
        else
            high = mid - 1;
    }

    // Item not found
    return -1;
}

int main(int argc, char *argv[]) {
    int n, i, item, index;

    // Get the number of elements from the command line arguments
    n = argc - 2;  // Last argument is the item to search, so n is argc - 2

    int arr[n];

    // Convert the array elements passed as arguments into integers
    for (i = 0; i < n; i++) {
        arr[i] = atoi(argv[i + 1]);
    }

    // The item to search is passed as the last command line argument
    item = atoi(argv[n + 1]);

    // Perform binary search
    index = binary_search(arr, n, item);

    // Print the result of the search
    if (index != -1) {
        printf("Item %d found at index %d.\n", item, index);
    } else {
        printf("Item %d not found in the array.\n", item);
    }

    return 0;
}
