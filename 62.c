#include <stdio.h>

#define MAX_FRAMES 7
#define REF_SIZE 13

int pages[REF_SIZE] = {1, 2, 3, 4, 1, 2, 5, 1, 1, 2, 3, 4, 5};

// Function to find the index of the page to replace in optimal algorithm
int findOptimalIndex(int pages[], int frames[], int frameCount, int currentIndex, int size) {
    int farthest = currentIndex;
    int index = -1;

    for (int i = 0; i < frameCount; i++) {
        int j;
        for (j = currentIndex; j < size; j++) {
            if (frames[i] == pages[j]) {
                if (j > farthest) {
                    farthest = j;
                    index = i;
                }
                break;
            }
        }
        if (j == size) {
            // Page not found in future, replace this frame
            return i;
        }
    }
    if (index == -1) return 0;
    else return index;
}

int optimal(int pages[], int size, int framesCount) {
    int frames[MAX_FRAMES];
    int pageFaults = 0;
    int filled = 0;

    for (int i = 0; i < framesCount; i++)
        frames[i] = -1;

    for (int i = 0; i < size; i++) {
        int found = 0;
        for (int j = 0; j < framesCount; j++) {
            if (frames[j] == pages[i]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            pageFaults++;
            if (filled < framesCount) {
                frames[filled++] = pages[i];
            } else {
                int idx = findOptimalIndex(pages, frames, framesCount, i + 1, size);
                frames[idx] = pages[i];
            }
        }
    }
    return pageFaults;
}

// LRU implementation similar to previous one
int lru(int pages[], int size, int framesCount) {
    int frames[MAX_FRAMES];
    int recent[MAX_FRAMES];
    int pageFaults = 0;

    for (int i = 0; i < framesCount; i++) {
        frames[i] = -1;
        recent[i] = -1;
    }

    for (int i = 0; i < size; i++) {
        int page = pages[i];
        int found = -1;

        for (int j = 0; j < framesCount; j++) {
            if (frames[j] == page) {
                found = j;
                break;
            }
        }

        if (found != -1) {
            recent[found] = i;
        } else {
            pageFaults++;
            int lru_index = 0;
            int min_recent = recent[0];
            for (int j = 1; j < framesCount; j++) {
                if (recent[j] < min_recent) {
                    min_recent = recent[j];
                    lru_index = j;
                }
            }
            frames[lru_index] = page;
            recent[lru_index] = i;
        }
    }
    return pageFaults;
}

int main() {
    int frame_sizes[] = {3, 4};
    int size = REF_SIZE;

    printf("Page reference string:\n");
    for (int i = 0; i < size; i++) printf("%d ", pages[i]);
    printf("\n\n");

    for (int i = 0; i < 2; i++) {
        int f = frame_sizes[i];
        int opt_faults = optimal(pages, size, f);
        int lru_faults = lru(pages, size, f);
        printf("Frame size: %d | Optimal Page Faults: %d | LRU Page Faults: %d\n", f, opt_faults, lru_faults);
    }
    return 0;
}
