//SJF Preemptive and Non Prem
#include <stdio.h>
#include <limits.h>

struct Process {
    int pid;
    int at, bt;          // Arrival time, Burst time
    int ct, tat, wt;     // Completion, Turnaround, Waiting times
    int remaining_bt;    // For preemptive SJF
};

// Function to print the scheduling table
void printTable(struct Process p[], int n) {
    printf("\n%-8s %-12s %-10s %-15s %-15s %-12s\n",
           "Process", "Arrival", "Burst", "Completion", "TurnAround", "Waiting");
    for (int i = 0; i < n; i++) {
        printf("P%-7d %-12d %-10d %-15d %-15d %-12d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
}

// Non-preemptive SJF
void sjf_non_preemptive(struct Process p[], int n) {
    int time = 0, completed = 0;
    int visited[100] = {0};

    while (completed < n) {
        int idx = -1, min_bt = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!visited[i] && p[i].at <= time) {
                if (p[i].bt < min_bt) {
                    min_bt = p[i].bt;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            time++;
        } else {
            time += p[idx].bt;
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            visited[idx] = 1;
            completed++;
        }
    }

    printf("Non-preemptive SJF scheduling table is as follows:\n");
    printTable(p, n);
}

// Preemptive SJF (Shortest Remaining Time First)
void sjf_preemptive(struct Process p[], int n) {
    int time = 0, completed = 0;

    for (int i = 0; i < n; i++) {
        p[i].remaining_bt = p[i].bt;
    }

    while (completed < n) {
        int min_remaining = INT_MAX;
        int idx = -1;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].remaining_bt > 0) {
                if (p[i].remaining_bt < min_remaining) {
                    min_remaining = p[i].remaining_bt;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            time++;
        } else {
            p[idx].remaining_bt--;
            time++;
            if (p[idx].remaining_bt == 0) {
                p[idx].ct = time;
                p[idx].tat = p[idx].ct - p[idx].at;
                p[idx].wt = p[idx].tat - p[idx].bt;
                completed++;
            }
        }
    }

    printf("Preemptive SJF scheduling (SRTF) table is as follows:\n");
    printTable(p, n);
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process p1[100], p2[100];

    for (int i = 0; i < n; i++) {
        printf("Enter the arrival and burst time for process %d: ", i + 1);
        scanf("%d %d", &p1[i].at, &p1[i].bt);
        p1[i].pid = i + 1;
        p2[i] = p1[i];  // Copy for preemptive SJF
    }

    sjf_non_preemptive(p1, n);
    sjf_preemptive(p2, n);

    return 0;
}



// ---------------------------------------------------------------------------------------



//round robin
#include <stdio.h>

struct Process {
    int id;
    int at;  // Arrival Time
    int bt;  // Burst Time
    int rt;  // Remaining Time
    int ct;  // Completion Time
    int tat; // Turnaround Time
    int wt;  // Waiting Time
};

int main() {
    int n, tq;

    // Input number of processes
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("Enter Arrival Time and Burst Time for P%d: ", i + 1);
        scanf("%d %d", &p[i].at, &p[i].bt);
        p[i].rt = p[i].bt;
    }

    // Input Time Quantum
    printf("Enter Time Quantum: ");
    scanf("%d", &tq);

    int time = 0;
    int completed = 0;
    int queue[100], front = 0, rear = 0;
    int visited[n];

    // Initialize visited array 
    for (int i = 0; i < n; i++) visited[i] = 0;

    // Add first process to queue
    queue[rear++] = 0;
    visited[0] = 1;

    // Round Robin scheduling loop
    while (completed < n) {
        int idx = queue[front++];

        if (p[idx].rt > tq) {
            time += tq;
            p[idx].rt -= tq;
        } else {
            time += p[idx].rt;
            p[idx].rt = 0;
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            completed++;
        }

        // Add newly arrived processes to queue
        for (int i = 0; i < n; i++) {
            if (!visited[i] && p[i].at <= time) {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }

        // If process still has remaining time, re-add to queue
        if (p[idx].rt > 0) queue[rear++] = idx;

        // If queue is empty, jump to next unvisited process
        if (front == rear) {
            for (int i = 0; i < n; i++) {
                if (!visited[i]) {
                    time = p[i].at;
                    queue[rear++] = i;
                    visited[i] = 1;
                    break;
                }
            }
        }
    }

    // Print process table
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }

    return 0;
}


// ---------------------------------------------------------------------------------------


//Non-Preemptive Priority Scheduling 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 100

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;
    int start_time;
    int finished;
    int priority;
} Process;

// Comparator for qsort: sort by arrival time, then by priority
int compare_priority(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    if (p1->arrival_time == p2->arrival_time)
        return p1->priority - p2->priority;
    return p1->arrival_time - p2->arrival_time;
}

// Non-preemptive Priority Scheduling
void priority_scheduling(Process p[], int n) {
    int current_time = 0;
    int completed = 0;
    int i, highest;

    printf("\n--- Priority Scheduling (Non-preemptive) ---\n");

    while (completed < n) {
        highest = -1;
        int highest_priority = 1e9;

        // Find the process with the highest priority available at current time
        for (i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && !p[i].finished) {
                if (p[i].priority < highest_priority) {
                    highest_priority = p[i].priority;
                    highest = i;
                } else if (p[i].priority == highest_priority) {
                    if (p[i].arrival_time < p[highest].arrival_time) {
                        highest = i;
                    }
                }
            }
        }

        if (highest == -1) {
            current_time++;  // If no process is ready, increment time
            continue;
        }

        // Schedule the selected process
        p[highest].start_time = current_time;
        p[highest].completion_time = current_time + p[highest].burst_time;
        p[highest].turnaround_time = p[highest].completion_time - p[highest].arrival_time;
        p[highest].waiting_time = p[highest].turnaround_time - p[highest].burst_time;
        p[highest].finished = 1;
        completed++;

        current_time = p[highest].completion_time;
    }

    // Print results
    printf("PID\tAT\tBT\tPR\tCT\tTAT\tWT\n");
    for (i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].arrival_time, p[i].burst_time, p[i].priority,
               p[i].completion_time, p[i].turnaround_time, p[i].waiting_time);
    }
}

int main() {
    int n;
    Process processes[MAX_PROCESSES];

    // Input number of processes
    printf("Enter number of processes: ");
    scanf("%d", &n);

    // Input process details
    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        printf("Enter arrival time, burst time and priority for process %d: ", i + 1);
        scanf("%d %d %d", &processes[i].arrival_time, &processes[i].burst_time, &processes[i].priority);
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].finished = 0;
    }

    // Sort processes by arrival time, then priority
    qsort(processes, n, sizeof(Process), compare_priority);

    // Run priority scheduling
    priority_scheduling(processes, n);

    return 0;
}




// ---------------------------------------------------------------------------------------


//Preemptive Priority Scheduling
#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid;
    int bt;  // Burst Time
    int at;  // Arrival Time
    int tat; // Turnaround Time
    int wt;  // Waiting Time
    int rt;  // Running Time
    int complete;
    int pri; // Priority (lower number = higher priority)
};

// Queue structure
struct queue {
    struct Process q[10];
    int front;
    int rear;
    int l;
};

// Initialize a queue
struct queue* queueInit() {
    struct queue *q = (struct queue*) malloc(sizeof(struct queue));
    q->front = -1;
    q->rear = 0;
    q->l = 10;
    return q;
}

// Enqueue element
void enqueue(struct queue* q, struct Process elem) {
    q->q[q->rear] = elem;
    q->rear = (q->rear + 1) % (q->l);
}

// Check if queue is empty
int isEmpty(struct queue *q) {
    return (q->rear == (q->front + 1) % (q->l));
}

// Dequeue element
struct Process dequeue(struct queue *q) {
    struct Process dummy = {-1,0,0,0,0,0,0,0};
    if (isEmpty(q)) return dummy;
    q->front = (q->front + 1) % q->l;
    return q->q[q->front];
}

// Swap two processes
void swap(struct Process *a, struct Process *b) {
    struct Process t = *a;
    *a = *b;
    *b = t;
}

// Sort processes by arrival time
void sort_by_arrival(struct Process proc[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (proc[j].at > proc[j + 1].at) {
                swap(&proc[j], &proc[j + 1]);
            }
        }
    }
}

// Accept process details from user
int acceptor(struct Process arr[]) {
    int n;
    printf("Enter Number of Processes: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        struct Process p;
        p.pid = i + 1;
        printf("Enter Burst Time: ");
        scanf("%d", &p.bt);
        printf("Enter Arrival Time: ");
        scanf("%d", &p.at);
        printf("Enter Priority: ");
        scanf("%d", &p.pri);
        p.wt = 0;
        p.rt = 0;
        p.complete = 0;
        arr[i] = p;
    }
    return n;
}

// Add process to corresponding priority queue
void Parse(struct queue *Low, struct queue *Mid, struct queue *High, struct Process p) {
    switch (p.pri) {
        case 1: enqueue(High, p); break;
        case 2: enqueue(Mid, p); break;
        case 3: enqueue(Low, p); break;
    }
}

// Preemptive Priority Scheduling
void Preemptive_Priority_Scheduling() {
    struct Process prc[10];
    int no_of_proc = acceptor(prc);
    int n_rem = no_of_proc;

    struct queue *Low = queueInit();
    struct queue *Mid = queueInit();
    struct queue *High = queueInit();

    struct Process running;
    running.pri = 4; // no process running initially
    int run_empty = 1;

    int time;
    for (time = 0; n_rem > 0 || !isEmpty(Low) || !isEmpty(Mid) || !isEmpty(High) || !run_empty; time++) {
        // Address new arrivals
        while (n_rem > 0 && prc[no_of_proc - n_rem].at == time) {
            Parse(Low, Mid, High, prc[no_of_proc - n_rem]);
            n_rem--;
        }

        // Assign CPU or preempt
        if (!isEmpty(High)) {
            if (run_empty) running = dequeue(High);
            else if (running.pri > 1) {
                Parse(Low, Mid, High, running);
                running = dequeue(High);
            }
        } else if (!isEmpty(Mid)) {
            if (run_empty) running = dequeue(Mid);
            else if (running.pri > 2) {
                Parse(Low, Mid, High, running);
                running = dequeue(Mid);
            }
        } else if (!isEmpty(Low)) {
            if (run_empty) running = dequeue(Low);
            else if (running.pri > 3) {
                Parse(Low, Mid, High, running);
                running = dequeue(Low);
            }
        }

        // Update waiting time and running time
        for (int i = 0; i < no_of_proc; i++) {
            if (prc[i].at > time) break;
            if (prc[i].pid != running.pid) {
                if (!prc[i].complete) prc[i].wt++;
            } else {
                prc[i].rt++;
                if (prc[i].rt == prc[i].bt) {
                    prc[i].tat = prc[i].wt + prc[i].bt;
                    prc[i].complete = 1;
                    run_empty = 1;
                } else {
                    run_empty = 0;
                }
            }
        }
    }

    // Print process table
    for (int i = 0; i < no_of_proc; i++) {
        printf("PID: %d\nPriority: %d\nWait Time: %d\nArrival Time: %d\nBurst Time: %d\nTurnaround Time: %d\n\n",
               prc[i].pid, prc[i].pri, prc[i].wt, prc[i].at, prc[i].bt, prc[i].tat);
    }
}

int main() {
    Preemptive_Priority_Scheduling();
    return 0;
}
