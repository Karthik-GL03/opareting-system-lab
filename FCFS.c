#include <stdio.h>

int main()
{
    int i, j, n, temp;
    float avg_wt = 0, avg_tat = 0;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    int at[n], bt[n], ct[n], tat[n], wt[n];

    for(i = 0; i < n; i++)
    {
        printf("Enter arrival time of process %d: ", i+1);
        scanf("%d", &at[i]);

        printf("Enter burst time of process %d: ", i+1);
        scanf("%d", &bt[i]);
    }

    for(i = 0; i < n; i++)
    {
        for(j = i+1; j < n; j++)
        {
            if(at[i] > at[j])
            {
                temp = at[i];
                at[i] = at[j];
                at[j] = temp;

                temp = bt[i];
                bt[i] = bt[j];
                bt[j] = temp;
            }
        }
    }

    ct[0] = at[0] + bt[0];
    tat[0] = ct[0] - at[0];
    wt[0] = 0;

    for(i = 1; i < n; i++)
    {
        ct[i] = ct[i-1] + bt[i];
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
    }

    printf("\n-------------------------------------------------\n");
    printf("Process\tAT\tBT\tCT\tTAT\tWT\n");
    printf("-------------------------------------------------\n");

    for(i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
        i+1, at[i], bt[i], ct[i], tat[i], wt[i]);

        avg_tat += tat[i];
        avg_wt += wt[i];
    }

    avg_tat = avg_tat / n;
    avg_wt = avg_wt / n;

    printf("-------------------------------------------------\n");
    printf("Average Turnaround Time = %.2f\n", avg_tat);
    printf("Average Waiting Time = %.2f\n", avg_wt);

    return 0;
}