#include <stdio.h>
#include <stdbool.h>

#define MAX_P 20
#define MAX_R 20

int P, R;
int allocation[MAX_P][MAX_R], need[MAX_P][MAX_R], max[MAX_P][MAX_R], available[MAX_R];

// Calculate the Need matrix
void calculateNeed() {
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

// Check if the system is in a safe state
bool isSafe() {
    int Work[MAX_R];
    bool Finish[MAX_P];
    int safeSeq[MAX_P];
    int count = 0;

    for (int i = 0; i < R; i++)
        Work[i] = available[i];

    for (int i = 0; i < P; i++)
        Finish[i] = false;

    while (count < P) {
        bool found = false;
        for (int p = 0; p < P; p++) {
            if (!Finish[p]) {
                int j;
                for (j = 0; j < R; j++) {
                    if (need[p][j] > Work[j])
                        break;
                }
                if (j == R) {
                    for (int k = 0; k < R; k++)
                        Work[k] += allocation[p][k];
                    safeSeq[count++] = p;
                    Finish[p] = true;
                    found = true;
                }
            }
        }
        if (!found) {
            printf("System is NOT in a safe state.\n");
            return false;
        }
    }

    printf("System is in a SAFE state.\nSafe sequence: < ");
    for (int i = 0; i < P; i++)
        printf("P%d ", safeSeq[i]);
    printf(">\n");
    return true;
}

// Request resources for a process
void reqResources(int process, int request[MAX_R]) {
    printf("\nProcess P%d is requesting resources: ", process);
    for (int i = 0; i < R; i++)
        printf("%d ", request[i]);
    printf("\n");

    // Check if request exceeds need
    for (int i = 0; i < R; i++) {
        if (request[i] > need[process][i]) {
            printf("Error: Process has exceeded its maximum claim.\n");
            return;
        }
    }

    // Check if resources are available
    for (int i = 0; i < R; i++) {
        if (request[i] > available[i]) {
            printf("Process must wait, resources not available.\n");
            return;
        }
    }

    // Tentatively allocate resources
    for (int i = 0; i < R; i++) {
        available[i] -= request[i];
        allocation[process][i] += request[i];
        need[process][i] -= request[i];
    }

    // Check if state is safe
    if (isSafe()) {
        printf("Request can be granted.\n");
    } else {
        printf("Request cannot be granted, restoring state.\n");
        for (int i = 0; i < R; i++) {
            available[i] += request[i];
            allocation[process][i] -= request[i];
            need[process][i] += request[i];
        }
    }
}

int main() {
    printf("Enter the number of processes: ");
    scanf("%d", &P);
    printf("Enter the number of resources: ");
    scanf("%d", &R);

    printf("Enter Allocation Matrix (%d x %d):\n", P, R);
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }

    printf("Enter Max Matrix (%d x %d):\n", P, R);
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    printf("Enter Available Resources (%d):\n", R);
    for (int i = 0; i < R; i++) {
        scanf("%d", &available[i]);
    }

    calculateNeed();
    printf("Checking if system is in a safe state...\n");
    isSafe();

    int choice;
    do {
        printf("\n1. Request Resource\n0. Exit\nEnter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            int process;
            printf("Enter the Process Number: ");
            scanf("%d", &process);

            int request[MAX_R];
            printf("Enter request Resource Vector: ");
            for (int i = 0; i < R; i++)
                scanf("%d", &request[i]);

            reqResources(process, request);
        }
    } while (choice == 1);

    return 0;
}
