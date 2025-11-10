// =======================================================
// Program 8: Disk Scheduling Algorithms
// Implements SSTF, SCAN, and C-LOOK algorithms
// =======================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// ---------- Utility Function: Sort ----------
void sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// ---------- SSTF (Shortest Seek Time First) ----------
void SSTF(int request[], int n, int head) {
    int seek_time = 0;
    int visited[n];

    for (int i = 0; i < n; i++)
        visited[i] = 0;

    printf("\nSSTF Disk Scheduling:\nSeek Sequence: %d", head);

    for (int i = 0; i < n; i++) {
        int min = 1e9, pos = -1;

        // Find the nearest unvisited request
        for (int j = 0; j < n; j++) {
            if (!visited[j]) {
                int diff = abs(head - request[j]);
                if (diff < min) {
                    min = diff;
                    pos = j;
                }
            }
        }

        // Move head
        seek_time += min;
        head = request[pos];
        visited[pos] = 1;
        printf(" -> %d", head);
    }

    printf("\nTotal Seek Time = %d\n", seek_time);
}

// ---------- SCAN (Elevator Algorithm) ----------
void SCAN(int request[], int n, int head, int disk_size) {
    int seek_time = 0, dir;
    printf("\nEnter Head Direction (0 for Left, 1 for Right): ");
    scanf("%d", &dir);

    int temp[n + 1], index = 0;
    for (int i = 0; i < n; i++)
        temp[i] = request[i];
    temp[n] = head;

    sort(temp, n + 1);

    int pos;
    for (pos = 0; pos < n + 1; pos++)
        if (temp[pos] == head)
            break;

    printf("\nSCAN Disk Scheduling:\nSeek Sequence: %d", head);

    if (dir == 0) {
        // ---- Move left first ----
        for (int i = pos - 1; i >= 0; i--) {
            seek_time += abs(head - temp[i]);
            head = temp[i];
            printf(" -> %d", head);
        }

        // Go to 0 end if not already
        if (head != 0) {
            seek_time += abs(head - 0);
            head = 0;
        }

        // ---- Then move right ----
        for (int i = pos + 1; i < n + 1; i++) {
            seek_time += abs(head - temp[i]);
            head = temp[i];
            printf(" -> %d", head);
        }

    } else {
        // ---- Move right first ----
        for (int i = pos + 1; i < n + 1; i++) {
            seek_time += abs(head - temp[i]);
            head = temp[i];
            printf(" -> %d", head);
        }

        // Go to end if not already
        if (head != disk_size - 1) {
            seek_time += abs(head - (disk_size - 1));
            head = disk_size - 1;
        }

        // ---- Then move left ----
        for (int i = pos - 1; i >= 0; i--) {
            seek_time += abs(head - temp[i]);
            head = temp[i];
            printf(" -> %d", head);
        }
    }

    printf("\nTotal Seek Time = %d\n", seek_time);
}

// ---------- C-LOOK (Circular LOOK) ----------
void CLOOK(int request[], int n, int head) {
    int seek_time = 0;
    int temp[n + 1];

    for (int i = 0; i < n; i++)
        temp[i] = request[i];
    temp[n] = head;

    sort(temp, n + 1);

    int pos;
    for (pos = 0; pos < n + 1; pos++)
        if (temp[pos] == head)
            break;

    printf("\nC-LOOK Disk Scheduling:\nSeek Sequence: %d", head);

    // ---- Move right first ----
    for (int i = pos + 1; i < n + 1; i++) {
        seek_time += abs(head - temp[i]);
        head = temp[i];
        printf(" -> %d", head);
    }

    // ---- Wrap around to smallest ----
    for (int i = 0; i < pos; i++) {
        seek_time += abs(head - temp[i]);
        head = temp[i];
        printf(" -> %d", head);
    }

    printf("\nTotal Seek Time = %d\n", seek_time);
}

// ---------- Main Function ----------
int main() {
    int n, head, disk_size;

    printf("Enter number of requests: ");
    scanf("%d", &n);

    int request[n];
    printf("Enter request queue: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &request[i]);

    printf("Enter initial head position: ");
    scanf("%d", &head);

    printf("Enter disk size: ");
    scanf("%d", &disk_size);

    // --- Execute Algorithms ---
    SSTF(request, n, head);
    SCAN(request, n, head, disk_size);
    CLOOK(request, n, head);

    return 0;
}
