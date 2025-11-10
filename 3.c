#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Process {
    char pid[5];
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;
};

// Function to calculate average
float average(int total, int n) {
    return (float)total / n;
}

// ---------- FCFS ----------
void fcfs(struct Process p[], int n) {
    int total_wt = 0, total_tat = 0;
    int current_time = 0;

    printf("\nGANTT CHART:\n");
    printf("_____________________\n| ");

    for (int i = 0; i < n; i++) {
        if (current_time < p[i].arrival_time)
            current_time = p[i].arrival_time;

        p[i].completion_time = current_time + p[i].burst_time;
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;

        total_wt += p[i].waiting_time;
        total_tat += p[i].turnaround_time;

        current_time = p[i].completion_time;
        printf(" %s |", p[i].pid);
    }

    printf("\n_____________________\n0 ");
    current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < p[i].arrival_time)
            current_time = p[i].arrival_time;
        current_time += p[i].burst_time;
        printf("%d ", current_time);
    }

    printf("\n\nAverage Waiting Time (FCFS): %.2f", average(total_wt, n));
    printf("\nAverage Turnaround Time (FCFS): %.2f\n", average(total_tat, n));
}

// ---------- SJF NON-PREEMPTIVE ----------
void sjf_nonpreemptive(struct Process p[], int n) {
    int completed = 0, current_time = 0;
    int total_wt = 0, total_tat = 0;
    int is_completed[n];

    for (int i = 0; i < n; i++)
        is_completed[i] = 0;

    printf("\nGANTT CHART:\n");
    printf("_____________________\n| ");

    while (completed != n) {
        int idx = -1;
        int min_bt = 9999;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && is_completed[i] == 0) {
                if (p[i].burst_time < min_bt) {
                    min_bt = p[i].burst_time;
                    idx = i;
                }
                // If same burst time, choose earlier arrival
                else if (p[i].burst_time == min_bt && p[i].arrival_time < p[idx].arrival_time)
                    idx = i;
            }
        }

        if (idx != -1) {
            if (current_time < p[idx].arrival_time)
                current_time = p[idx].arrival_time;

            p[idx].completion_time = current_time + p[idx].burst_time;
            p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
            p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;

            total_wt += p[idx].waiting_time;
            total_tat += p[idx].turnaround_time;

            current_time = p[idx].completion_time;
            is_completed[idx] = 1;
            completed++;

            printf(" %s |", p[idx].pid);
        } else {
            current_time++;
        }
    }

    printf("\n_____________________\n");
    printf("\nAverage Waiting Time (SJF Non-preemptive): %.2f", average(total_wt, n));
    printf("\nAverage Turnaround Time (SJF Non-preemptive): %.2f\n", average(total_tat, n));
}

// ---------- SJF PREEMPTIVE ----------
void sjf_preemptive(struct Process p[], int n) {
    int completed = 0, current_time = 0, total_wt = 0, total_tat = 0;
    int is_completed[n];
    for (int i = 0; i < n; i++) {
        p[i].remaining_time = p[i].burst_time;
        is_completed[i] = 0;
    }

    printf("\nGANTT CHART:\n");
    printf("_____________________\n| ");

    while (completed != n) {
        int idx = -1;
        int min_rt = 9999;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && is_completed[i] == 0) {
                if (p[i].remaining_time < min_rt) {
                    min_rt = p[i].remaining_time;
                    idx = i;
                }
                else if (p[i].remaining_time == min_rt && p[i].arrival_time < p[idx].arrival_time)
                    idx = i;
            }
        }

        if (idx != -1) {
            p[idx].remaining_time--;
            printf(" %s |", p[idx].pid);
            current_time++;

            if (p[idx].remaining_time == 0) {
                p[idx].completion_time = current_time;
                p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
                p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;

                total_wt += p[idx].waiting_time;
                total_tat += p[idx].turnaround_time;
                is_completed[idx] = 1;
                completed++;
            }
        } else {
            current_time++;
        }
    }

    printf("\n_____________________\n");
    printf("\nAverage Waiting Time (SJF Preemptive): %.2f", average(total_wt, n));
    printf("\nAverage Turnaround Time (SJF Preemptive): %.2f\n", average(total_tat, n));
}

// ---------- ROUND ROBIN ----------
void round_robin(struct Process p[], int n, int quantum) {
    int total_wt = 0, total_tat = 0;
    int remaining[n];
    int completed = 0;
    int current_time = 0;
    int done;

    for (int i = 0; i < n; i++)
        remaining[i] = p[i].burst_time;

    printf("\nGANTT CHART:\n");
    printf("_____________________\n| ");

    while (completed < n) {
        done = 1;

        for (int i = 0; i < n; i++) {
            if (remaining[i] > 0) {
                done = 0;
                if (p[i].arrival_time <= current_time) {
                    printf(" %s |", p[i].pid);

                    if (remaining[i] > quantum) {
                        current_time += quantum;
                        remaining[i] -= quantum;
                    } else {
                        current_time += remaining[i];
                        p[i].completion_time = current_time;
                        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
                        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;

                        total_wt += p[i].waiting_time;
                        total_tat += p[i].turnaround_time;
                        remaining[i] = 0;
                        completed++;
                    }
                }
            }
        }

        if (done == 1)
            break;
    }

    printf("\n_____________________\n");
    printf("\nAverage Waiting Time (Round Robin): %.2f", average(total_wt, n));
    printf("\nAverage Turnaround Time (Round Robin): %.2f\n", average(total_tat, n));
}

// ---------- MAIN ----------
int main() {
    int n, choice, quantum;

    printf("Enter number of processes: ");
    scanf("%d", &n);
    struct Process p[n];

    printf("Enter Process ID, Arrival Time, and Burst Time:\n");
    for (int i = 0; i < n; i++) {
        scanf("%s %d %d", p[i].pid, &p[i].arrival_time, &p[i].burst_time);
    }

    do {
        printf("\nCPU Scheduling Menu:\n");
        printf("1. FCFS\n");
        printf("2. SJF (Non-preemptive)\n");
        printf("3. SJF (Preemptive)\n");
        printf("4. Round Robin\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                fcfs(p, n);
                break;
            case 2:
                sjf_nonpreemptive(p, n);
                break;
            case 3:
                sjf_preemptive(p, n);
                break;
            case 4:
                printf("Enter time quantum: ");
                scanf("%d", &quantum);
                round_robin(p, n, quantum);
                break;
            case 5:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice! Try again.\n");
        }
    } while (choice != 5);

    return 0;
}
