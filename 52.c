#include <stdio.h>
#include <stdlib.h>

#define MAX 10

int n, m;
int alloc[MAX][MAX], max[MAX][MAX], avail[MAX], need[MAX][MAX];

// Function to calculate the need matrix
void calculateNeed() {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];
}

// Safety algorithm to check if the system is in safe state
int isSafe(int safeSeq[]) {
    int work[MAX];
    int finish[MAX] = {0};
    for (int i = 0; i < m; i++)
        work[i] = avail[i];

    int count = 0;
    while (count < n) {
        int found = 0;
        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                int canAllocate = 1;
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        canAllocate = 0;
                        break;
                    }
                }
                if (canAllocate) {
                    for (int k = 0; k < m; k++)
                        work[k] += alloc[i][k];
                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if (!found) {
            return 0; // Unsafe state
        }
    }
    return 1; // Safe state
}

int main() {
    FILE *fp = fopen("state.txt", "r");
    if (fp == NULL) {
        perror("Failed to open state.txt");
        return 1;
    }

    // Read number of processes and resources
    fscanf(fp, "%d", &n);
    fscanf(fp, "%d", &m);

    // Read Allocation Matrix
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            fscanf(fp, "%d", &alloc[i][j]);

    // Read Max Matrix
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            fscanf(fp, "%d", &max[i][j]);

    // Read Available Matrix
    for (int i = 0; i < m; i++)
        fscanf(fp, "%d", &avail[i]);

    fclose(fp);

    calculateNeed();

    printf("Number of Processes: %d\n", n);
    printf("Number of Resources: %d\n", m);

    printf("\nAllocation Matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            printf("%d ", alloc[i][j]);
        printf("\n");
    }

    printf("\nMax Matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            printf("%d ", max[i][j]);
        printf("\n");
    }

    printf("\nAvailable Resources:\n");
    for (int i = 0; i < m; i++)
        printf("%d ", avail[i]);
    printf("\n");

    printf("\nNeed Matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }

    int safeSeq[MAX];
    if (isSafe(safeSeq)) {
        printf("\nSystem is in a SAFE state.\nSafe Sequence: ");
        for (int i = 0; i < n; i++)
            printf("P%d ", safeSeq[i]);
        printf("\n");
    } else {
        printf("\nSystem is in an UNSAFE state!\n");
    }

    // Now demonstrate granting or denying a new request
    int reqProc;
    int request[MAX];
    printf("\nEnter process number making the request (0 to %d): ", n - 1);
    scanf("%d", &reqProc);

    if (reqProc < 0 || reqProc >= n) {
        printf("Invalid process number!\n");
        return 1;
    }

    printf("Enter the request vector (space separated %d resource values): ", m);
    for (int i = 0; i < m; i++)
        scanf("%d", &request[i]);

    // Check if request <= need
    for (int i = 0; i < m; i++) {
        if (request[i] > need[reqProc][i]) {
            printf("Error: Process has exceeded its maximum claim.\n");
            return 1;
        }
    }

    // Check if request <= available
    for (int i = 0; i < m; i++) {
        if (request[i] > avail[i]) {
            printf("Resources are not available, process must wait.\n");
            return 0;
        }
    }

    // Pretend to allocate requested resources
    for (int i = 0; i < m; i++) {
        avail[i] -= request[i];
        alloc[reqProc][i] += request[i];
        need[reqProc][i] -= request[i];
    }

    // Check if system is still safe after allocation
    if (isSafe(safeSeq)) {
        printf("Request can be granted.\nSafe Sequence after granting request: ");
        for (int i = 0; i < n; i++)
            printf("P%d ", safeSeq[i]);
        printf("\n");
    } else {
        printf("Request cannot be granted as it leads to an unsafe state.\n");

        // Rollback allocation
        for (int i = 0; i < m; i++) {
            avail[i] += request[i];
            alloc[reqProc][i] -= request[i];
            need[reqProc][i] += request[i];
        }
    }

    return 0;
}
