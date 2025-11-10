#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FRAMES 7
#define MAX_PAGES 100
#define PAGE_RANGE 10

// Generate a random page reference string of length n with pages 0-9
void generatePageReferenceString(int pages[], int n) {
    for (int i = 0; i < n; i++) {
        pages[i] = rand() % PAGE_RANGE;
    }
}

// FIFO Page Replacement Algorithm
int fifo(int pages[], int n, int frames) {
    int page_faults = 0;
    int frame_arr[MAX_FRAMES];
    int index = 0;

    for (int i = 0; i < frames; i++)
        frame_arr[i] = -1;

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = 0;

        for (int j = 0; j < frames; j++) {
            if (frame_arr[j] == page) {
                found = 1;
                break;
            }
        }

        if (!found) {
            frame_arr[index] = page;
            index = (index + 1) % frames;
            page_faults++;
        }
    }
    return page_faults;
}

// LRU Page Replacement Algorithm
int lru(int pages[], int n, int frames) {
    int page_faults = 0;
    int frame_arr[MAX_FRAMES];
    int recent[MAX_FRAMES];  // Tracks last used time for each frame

    for (int i = 0; i < frames; i++) {
        frame_arr[i] = -1;
        recent[i] = -1;
    }

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = -1;

        for (int j = 0; j < frames; j++) {
            if (frame_arr[j] == page) {
                found = j;
                break;
            }
        }

        if (found != -1) {
            recent[found] = i;  // Update last used time
        } else {
            page_faults++;
            int lru_index = 0;
            int min_recent = recent[0];
            // Find the least recently used frame
            for (int j = 1; j < frames; j++) {
                if (recent[j] < min_recent) {
                    min_recent = recent[j];
                    lru_index = j;
                }
            }
            frame_arr[lru_index] = page;
            recent[lru_index] = i;
        }
    }
    return page_faults;
}

int main() {
    srand(time(NULL));

    int pages[MAX_PAGES];
    int n;

    printf("Enter length of the page reference string (max 100): ");
    scanf("%d", &n);

    if (n <= 0 || n > MAX_PAGES) {
        printf("Invalid length.\n");
        return 1;
    }

    generatePageReferenceString(pages, n);

    printf("Generated Page Reference String:\n");
    for (int i = 0; i < n; i++)
        printf("%d ", pages[i]);
    printf("\n\n");

    for (int frames = 1; frames <= MAX_FRAMES; frames++) {
        int fifo_faults = fifo(pages, n, frames);
        int lru_faults = lru(pages, n, frames);
        printf("Frames: %d | FIFO Page Faults: %d | LRU Page Faults: %d\n", frames, fifo_faults, lru_faults);
    }

    return 0;
}
