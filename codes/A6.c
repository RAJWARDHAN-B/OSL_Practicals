#include <stdio.h>
#include <stdlib.h>
#define MAX 100

void printFrames(int frames[], int frame_size) {
    for (int i = 0; i < frame_size; i++) {
        if (frames[i] == -1)
            printf(" - ");
        else
            printf(" %d ", frames[i]);
    }
    printf("\n");
}

int isPageInFrames(int frames[], int frame_size, int page) {
    for (int i = 0; i < frame_size; i++) {
        if (frames[i] == page)
            return 1;
    }
    return 0;
}

/* ---------------- FCFS ---------------- */
void fcfs(int pages[], int n, int frame_size) {
    int frames[frame_size];
    for (int i = 0; i < frame_size; i++)
        frames[i] = -1;

    int page_faults = 0, next_to_replace = 0;

    printf("\nFCFS Page Replacement:\n");
    for (int i = 0; i < n; i++) {
        int page = pages[i];
        if (!isPageInFrames(frames, frame_size, page)) {
            frames[next_to_replace] = page;
            next_to_replace = (next_to_replace + 1) % frame_size;
            page_faults++;
        }
        printFrames(frames, frame_size);
    }
    printf("Total Page Faults: %d\n", page_faults);
}

/* ---------------- LRU ---------------- */
int findLRU(int pages[], int frames[], int frame_size, int current_index) {
    int lru_index = -1, min = current_index;

    for (int i = 0; i < frame_size; i++) {
        int j;
        for (j = current_index - 1; j >= 0; j--) {
            if (frames[i] == pages[j]) {
                if (j < min) {
                    min = j;
                    lru_index = i;
                }
                break;
            }
        }
        if (j == -1) {
            return i;
        }
    }
    if (lru_index == -1)
        return 0;
    return lru_index;
}

void lru(int pages[], int n, int frame_size) {
    int frames[frame_size];
    for (int i = 0; i < frame_size; i++)
        frames[i] = -1;

    int page_faults = 0;

    printf("\nLRU Page Replacement:\n");
    for (int i = 0; i < n; i++) {
        int page = pages[i];
        if (!isPageInFrames(frames, frame_size, page)) {
            int replaced = 0;
            for (int j = 0; j < frame_size; j++) {
                if (frames[j] == -1) {
                    frames[j] = page;
                    replaced = 1;
                    break;
                }
            }
            if (!replaced) {
                int lru_index = findLRU(pages, frames, frame_size, i);
                frames[lru_index] = page;
            }
            page_faults++;
        }
        printFrames(frames, frame_size);
    }
    printf("Total Page Faults: %d\n", page_faults);
}

/* ---------------- OPTIMAL ---------------- */
int findOptimal(int pages[], int frames[], int frame_size, int current_index, int n) {
    int res = -1, farthest = current_index;

    for (int i = 0; i < frame_size; i++) {
        int j;
        for (j = current_index + 1; j < n; j++) {
            if (frames[i] == pages[j]) {
                if (j > farthest) {
                    farthest = j;
                    res = i;
                }
                break;
            }
        }
        if (j == n) {
            return i;
        }
    }
    if (res == -1)
        return 0;
    return res;
}

void optimal(int pages[], int n, int frame_size) {
    int frames[frame_size];
    for (int i = 0; i < frame_size; i++)
        frames[i] = -1;

    int page_faults = 0;

    printf("\nOptimal Page Replacement:\n");
    for (int i = 0; i < n; i++) {
        int page = pages[i];
        if (!isPageInFrames(frames, frame_size, page)) {
            int replaced = 0;
            for (int j = 0; j < frame_size; j++) {
                if (frames[j] == -1) {
                    frames[j] = page;
                    replaced = 1;
                    break;
                }
            }
            if (!replaced) {
                int opt_index = findOptimal(pages, frames, frame_size, i, n);
                frames[opt_index] = page;
            }
            page_faults++;
        }
        printFrames(frames, frame_size);
    }
    printf("Total Page Faults: %d\n", page_faults);
}

/* ---------------- MAIN ---------------- */
int main() {
    int n, frame_size, choice;

    printf("Enter number of pages: ");
    scanf("%d", &n);

    int pages[n];
    printf("Enter the pages (space separated): ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &pages[i]);
    }

    do {
        printf("Enter frame size (minimum 3): ");
        scanf("%d", &frame_size);
    } while (frame_size < 3);

    while (1) {
        printf("\nChoose Page Replacement Algorithm:\n");
        printf("1. FCFS\n");
        printf("2. LRU\n");
        printf("3. Optimal\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                fcfs(pages, n, frame_size);
                break;
            case 2:
                lru(pages, n, frame_size);
                break;
            case 3:
                optimal(pages, n, frame_size);
                break;
            case 4:
                printf("Exiting program.\n");
                exit(0);
            default:
                printf("Invalid choice! Please select 1-4.\n");
        }
    }
    return 0;
}
