#include <stdio.h>
#include <stdlib.h>

#define CYLINDERS 500

int cmp(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void scan(int requests[], int n, int head) {
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

    printf("SCAN sequence: %d ", head);

    // Move right first
    for (i = 0; i < right_count; i++) {
        cur_track = right[i];
        distance = abs(cur_track - head);
        seek_count += distance;
        head = cur_track;
        printf("-> %d ", cur_track);
    }

    // Move to the end if not already at last cylinder
    if (head != CYLINDERS - 1) {
        distance = abs(CYLINDERS - 1 - head);
        seek_count += distance;
        head = CYLINDERS - 1;
        printf("-> %d ", head);
    }

    // Move left now
    for (i = left_count - 1; i >= 0; i--) {
        cur_track = left[i];
        distance = abs(cur_track - head);
        seek_count += distance;
        head = cur_track;
        printf("-> %d ", cur_track);
    }

    printf("\nTotal seek operations: %d\n", seek_count);
    printf("Average seek distance: %.2f\n\n", (float)seek_count / n);

    free(left);
    free(right);
}

void look(int requests[], int n, int head) {
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

    printf("LOOK sequence: %d ", head);

    // Move right first
    for (i = 0; i < right_count; i++) {
        cur_track = right[i];
        distance = abs(cur_track - head);
        seek_count += distance;
        head = cur_track;
        printf("-> %d ", cur_track);
    }

    // Move left now (no go to end)
    for (i = left_count - 1; i >= 0; i--) {
        cur_track = left[i];
        distance = abs(cur_track - head);
        seek_count += distance;
        head = cur_track;
        printf("-> %d ", cur_track);
    }

    printf("\nTotal seek operations: %d\n", seek_count);
    printf("Average seek distance: %.2f\n\n", (float)seek_count / n);

    free(left);
    free(right);
}

int main() {
    int requests[] = {20, 229, 39, 450, 18, 145, 120, 380, 20, 250};
    int n = sizeof(requests) / sizeof(requests[0]);
    int head = 185;

    printf("Disk Scheduling for SCAN and LOOK\n");
    scan(requests, n, head);
    look(requests, n, head);

    return 0;
}
