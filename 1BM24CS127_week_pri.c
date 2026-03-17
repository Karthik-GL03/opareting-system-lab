#include <stdio.h>
#include <limits.h>

#define MAX 100

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int is_completed;
};

void preemptivePriority(struct Process p[], int n)
{
    int completed    = 0;
    int current_time = 0;

    for (int i = 0; i < n; i++) {
        p[i].remaining_time = p[i].burst_time;
    }

    while (completed < n)
    {
        int idx = -1;
        int best_priority = INT_MAX;

        for (int i = 0; i < n; i++)
        {
            if (p[i].arrival_time <= current_time && p[i].remaining_time > 0)
            {
                if (p[i].priority < best_priority)
                {
                    best_priority = p[i].priority;
                    idx = i;
                }
                else if (p[i].priority == best_priority && idx != -1 && p[i].arrival_time < p[idx].arrival_time)
                {
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            current_time++;
            continue;
        }

        p[idx].remaining_time--;
        current_time++;

        if (p[idx].remaining_time == 0)
        {
            completed++;
            p[idx].completion_time  = current_time;
            p[idx].turnaround_time  = p[idx].completion_time - p[idx].arrival_time;
            p[idx].waiting_time     = p[idx].turnaround_time - p[idx].burst_time;
        }
    }
}

void printGanttChartPreemptive(struct Process p[], int n)
{
    int completed = 0;
    int current_time = 0;
    int prev = -1;
    int rem[MAX];
   
    for (int i = 0; i < n; i++)
        rem[i] = p[i].burst_time;

    printf("\nPreemptive Gantt Chart:\n|");

    int gantt_pid[1000], gantt_start[1000], gantt_end[1000];
    int gc = 0;

    while (completed < n)
    {
        int idx = -1;
        int best = INT_MAX;
        for (int i = 0; i < n; i++)
        {
            if (p[i].arrival_time <= current_time && rem[i] > 0)
            {
                if (p[i].priority < best)
                { best = p[i].priority; idx = i; }
                else if (p[i].priority == best && idx != -1 && p[i].arrival_time < p[idx].arrival_time)
                    idx = i;
            }
        }

        if (idx == -1) { current_time++; continue; }

        if (prev != idx)
        {
            if (gc > 0) gantt_end[gc-1] = current_time;
            gantt_pid[gc]   = p[idx].pid;
            gantt_start[gc] = current_time;
            gc++;
            prev = idx;
        }

        rem[idx]--;
        current_time++;
        if (rem[idx] == 0) completed++;
    }
    if (gc > 0) gantt_end[gc-1] = current_time;

    for (int i = 0; i < gc; i++)
        printf(" P%d |", gantt_pid[i]);
    printf("\n");
    printf("%d", gantt_start[0]);
    for (int i = 0; i < gc; i++)
        printf("    %d", gantt_end[i]);
    printf("\n");
}

void nonPreemptivePriority(struct Process p[], int n)
{
    int completed = 0;
    int current_time = 0;

    for (int i = 0; i < n; i++)
        p[i].is_completed = 0;

    while (completed < n)
    {
        int idx = -1;
        int best_priority = INT_MAX;

        for (int i = 0; i < n; i++)
        {
            if (p[i].arrival_time <= current_time && !p[i].is_completed)
            {
                if (p[i].priority < best_priority)
                {
                    best_priority = p[i].priority;
                    idx = i;
                }
                else if (p[i].priority == best_priority)
                {
                    if (idx != -1 && p[i].arrival_time < p[idx].arrival_time)
                        idx = i;
                }
            }
        }

        if (idx == -1)
        {
            current_time++;
        }
        else
        {
            p[idx].completion_time  = current_time + p[idx].burst_time;
            p[idx].turnaround_time  = p[idx].completion_time - p[idx].arrival_time;
            p[idx].waiting_time     = p[idx].turnaround_time - p[idx].burst_time;
            p[idx].is_completed     = 1;

            current_time = p[idx].completion_time;
            completed++;
        }
    }
}

void displayResults(struct Process p[], int n, const char* title)
{
    float total_tat = 0, total_wt = 0;

    printf("\n--- %s ---\n", title);
    printf("%-8s %-5s %-5s %-5s %-5s %-5s %-5s\n",
           "Process","AT","BT","Pri","CT","TAT","WT");
    printf("------------------------------------------------\n");

    for (int i = 0; i < n; i++)
    {
        printf("P%-7d %-5d %-5d %-5d %-5d %-5d %-5d\n",
               p[i].pid, p[i].arrival_time, p[i].burst_time,
               p[i].priority, p[i].completion_time,
               p[i].turnaround_time, p[i].waiting_time);
        total_tat += p[i].turnaround_time;
        total_wt  += p[i].waiting_time;
    }
    printf("------------------------------------------------\n");
    printf("Average Turnaround Time = %.2f\n", total_tat / n);
    printf("Average Waiting Time    = %.2f\n\n", total_wt  / n);
}

int main()
{
    int n;
    struct Process p_original[MAX], p_pre[MAX], p_non[MAX];

    printf("===== PRIORITY SCHEDULING COMPARISON =====\n");
    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {
        p_original[i].pid = i + 1;
        printf("P%d -> Arrival Time, Burst Time, Priority: ", i + 1);
        scanf("%d %d %d", &p_original[i].arrival_time,
                          &p_original[i].burst_time,
                          &p_original[i].priority);
                         
        p_pre[i] = p_original[i];
        p_non[i] = p_original[i];
    }

    preemptivePriority(p_pre, n);
    printGanttChartPreemptive(p_pre, n);
    displayResults(p_pre, n, "PREEMPTIVE RESULTS");

    nonPreemptivePriority(p_non, n);
    displayResults(p_non, n, "NON-PREEMPTIVE RESULTS");

    return 0;
}

