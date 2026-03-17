#include <stdio.h>
#include <string.h>

#define MAX 100
#define QSIZE 10000

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
};

void sortByArrival(struct Process p[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (p[j].arrival_time > p[j+1].arrival_time)
            {
                struct Process temp = p[j];
                p[j]   = p[j+1];
                p[j+1] = temp;
            }
        }
    }
}

void nonPreemptiveFCFS(struct Process p[], int n)
{
    sortByArrival(p, n);
    int current_time = 0;

    for (int i = 0; i < n; i++)
    {
        if (current_time < p[i].arrival_time)
            current_time = p[i].arrival_time;

        p[i].completion_time  = current_time + p[i].burst_time;
        p[i].turnaround_time  = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time     = p[i].turnaround_time - p[i].burst_time;

        current_time = p[i].completion_time;
    }
}

void preemptiveRoundRobin(struct Process p[], int n, int quantum)
{
    int queue[QSIZE];
    int front = 0, rear = 0;

    int in_queue[MAX];
    int is_completed[MAX];
    int completed = 0;
    int current_time = 0;

    memset(in_queue, 0, sizeof(in_queue));
    memset(is_completed, 0, sizeof(is_completed));

    for (int i = 0; i < n; i++)
        p[i].remaining_time = p[i].burst_time;

    sortByArrival(p, n);

    for (int i = 0; i < n; i++)
    {
        if (p[i].arrival_time == 0)
        {
            queue[rear++] = i;
            in_queue[i]   = 1;
        }
    }

    printf("\nRound Robin Gantt Chart:\n|");

    while (completed < n)
    {
        if (front == rear)
        {
            for (int i = 0; i < n; i++)
            {
                if (!is_completed[i] && !in_queue[i])
                {
                    current_time = p[i].arrival_time;
                    queue[rear++] = i;
                    in_queue[i]   = 1;
                    break;
                }
            }
            continue;
        }

        int idx = queue[front++];
        int exec_time;

        if (p[idx].remaining_time <= quantum)
            exec_time = p[idx].remaining_time;
        else
            exec_time = quantum;

        printf(" P%d(%d-%d) |", p[idx].pid, current_time, current_time + exec_time);

        current_time += exec_time;
        p[idx].remaining_time -= exec_time;

        for (int i = 0; i < n; i++)
        {
            if (!in_queue[i] && !is_completed[i] && p[i].arrival_time <= current_time)
            {
                queue[rear++] = i;
                in_queue[i]   = 1;
            }
        }

        if (p[idx].remaining_time == 0)
        {
            is_completed[idx]       = 1;
            completed++;
            p[idx].completion_time  = current_time;
            p[idx].turnaround_time  = current_time - p[idx].arrival_time;
            p[idx].waiting_time     = p[idx].turnaround_time - p[idx].burst_time;
        }
        else
        {
            queue[rear++] = idx;
        }
    }
    printf("\n");
}

void displayResults(struct Process p[], int n, const char* title)
{
    float total_tat = 0, total_wt = 0;

    printf("\n--- %s ---\n", title);
    printf("%-8s %-5s %-5s %-5s %-5s %-5s\n",
           "Process","AT","BT","CT","TAT","WT");
    printf("--------------------------------------------\n");

    for (int i = 0; i < n; i++)
    {
        printf("P%-7d %-5d %-5d %-5d %-5d %-5d\n",
               p[i].pid, p[i].arrival_time, p[i].burst_time,
               p[i].completion_time, p[i].turnaround_time,
               p[i].waiting_time);
        total_tat += p[i].turnaround_time;
        total_wt  += p[i].waiting_time;
    }
    printf("--------------------------------------------\n");
    printf("Average Turnaround Time = %.2f\n", total_tat / n);
    printf("Average Waiting Time    = %.2f\n\n", total_wt  / n);
}

int main()
{
    int n, quantum;
    struct Process p_original[MAX], p_fcfs[MAX], p_rr[MAX];

    printf("===== SCHEDULING COMPARISON (FCFS vs ROUND ROBIN) =====\n");
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter time quantum (for Round Robin): ");
    scanf("%d", &quantum);

    for (int i = 0; i < n; i++)
    {
        p_original[i].pid = i + 1;
        printf("P%d -> Arrival Time, Burst Time: ", i + 1);
        scanf("%d %d", &p_original[i].arrival_time, &p_original[i].burst_time);
        
        p_fcfs[i] = p_original[i];
        p_rr[i]   = p_original[i];
    }

    nonPreemptiveFCFS(p_fcfs, n);
    displayResults(p_fcfs, n, "NON-PREEMPTIVE RESULTS (FCFS)");

    preemptiveRoundRobin(p_rr, n, quantum);
    displayResults(p_rr, n, "PREEMPTIVE RESULTS (ROUND ROBIN)");

    return 0;
}
