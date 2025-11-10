#include <stdio.h>
#include <stdlib.h>

#define CYLINDERS 500

// Compare function for qsort
int cmp(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void cscan(int requests[], int n, int head) {
    int seek_count = 0;
    int distance, cur_track;
    int i, j;

    int* left = (int*)malloc(n * sizeof(int));
    int* right = (int*)malloc(n * sizeof(int));
    int left_count = 0, right_count = 0;

    // Split requests into left and right of head
    for (i = 0; i < n; i++) {
        if (requests[i] < head)
            left[left_count++] = requests[i];
        else
            right[right_count++] = requests[i];
    }

    qsort(left, left_count, sizeof(int), cmp);
    qsort(right, right_count, sizeof(int), cmp);

    printf("C-SCAN sequence: %d ", head);

    // Go right first
    for (i = 0; i < right_count; i++) {
        cur_track = right[i];
        distance = abs(cur_track - head);
        seek_count += distance;
        head = cur_track;
        printf("-> %d ", cur_track);
    }

    // Jump to beginning
    if (left_count > 0) {
        distance = abs(CYLINDERS - 1 - head); // to end
        seek_count += distance;
        head = 0;
        seek_count += CYLINDERS - 1; // from end to start
        printf("-> %d ", CYLINDERS - 1);
        printf("-> %d ", head);

        for (i = 0; i < left_count; i++) {
            cur_track = left[i];
            distance = abs(cur_track - head);
            seek_count += distance;
            head = cur_track;
            printf("-> %d ", cur_track);
        }
    }
    printf("\nTotal seek operations: %d\n", seek_count);
    printf("Average seek distance: %.2f\n\n", (float)seek_count / n);

    free(left);
    free(right);
}

void clook(int requests[], int n, int head) {
    int seek_count = 0;
    int distance, cur_track;
    int i;

    int* left = (int*)malloc(n * sizeof(int));
    int* right = (int*)malloc(n * sizeof(int));
    int left_count = 0, right_count = 0;

    for (i = 0; i < n; i++) {
        if (requests[i] < head)
            left[left_count++] = requests[i];
        else
            right[right_count++] = requests[i];
    }

    qsort(left, left_count, sizeof(int), cmp);
    qsort(right, right_count, sizeof(int), cmp);

    printf("C-LOOK sequence: %d ", head);

    // Go right first
    for (i = 0; i < right_count; i++) {
        cur_track = right[i];
        distance = abs(cur_track - head);
        seek_count += distance;
        head = cur_track;
        printf("-> %d ", cur_track);
    }

    // Jump to smallest left
    if (left_count > 0) {
        distance = abs(head - left[0]);
        seek_count += distance;
        head = left[0];
        printf("-> %d ", head);

        for (i = 1; i < left_count; i++) {
            cur_track = left[i];
            distance = abs(cur_track - head);
            seek_count += distance;
            head = cur_track;
            printf("-> %d ", cur_track);
        }
    }
    printf("\nTotal seek operations: %d\n", seek_count);
    printf("Average seek distance: %.2f\n\n", (float)seek_count / n);

    free(left);
    free(right);
}

int main() {
    int requests[] = {10, 229, 39, 400, 18, 145, 120, 480, 20, 250};
    int n = sizeof(requests) / sizeof(requests[0]);
    int head = 85;

    printf("Disk Scheduling for C-SCAN and C-LOOK\n");
    cscan(requests, n, head);
    clook(requests, n, head);

    return 0;
}
