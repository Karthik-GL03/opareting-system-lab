#include <stdio.h>

int main()
{
    int at[10];
    int bt[10];
    int n;
    int finished[10] = {0};

    printf("Enter number of processes:\n");
    scanf("%d", &n);

    printf("Enter arrival time and bursting time of processes:\n");

    for(int i = 0; i < n; i++)
    {
        printf("Process %d\n", i + 1);

        printf("Enter AT: ");
        scanf("%d", &at[i]);

        printf("Enter BT: ");
        scanf("%d", &bt[i]);
    }

    for(int i = 0; i < n; i++)
    {
        for(int j = i + 1; j < n; j++)
        {
            if(at[i] > at[j])
            {
                int temp = at[i];
                at[i] = at[j];
                at[j] = temp;

                int temp1 = bt[i];
                bt[i] = bt[j];
                bt[j] = temp1;
            }
        }
    }

    int ct[10], tat[10], wt[10];
    int curr_time = 0;
    int comp = 0;

    while(comp < n)
    {
        int idx = -1;
        int min_bt = 9999;

        for(int i = 0; i < n; i++)
        {
            if(at[i] <= curr_time && finished[i] == 0)
            {
                if(bt[i] < min_bt)
                {
                    min_bt = bt[i];
                    idx = i;
                }
            }
        }

        if(idx == -1)
        {
            curr_time++;
        }
        else
        {
            ct[idx] = curr_time + bt[idx];
            tat[idx] = ct[idx] - at[idx];
            wt[idx] = tat[idx] - bt[idx];

            curr_time = ct[idx];
            finished[idx] = 1;
            comp++;
        }
    }

    float avg_wt, avg_tat;
    float total_tat = 0;
    float total_wt = 0;

    for(int i = 0; i < n; i++)
    {
        total_tat += tat[i];
        total_wt += wt[i];
    }

    avg_tat = total_tat / n;
    avg_wt = total_wt / n;

    printf("Process AT BT CT TAT WT\n");

    for(int i = 0; i < n; i++)
    {
        printf("%d %d %d %d %d %d\n", i+1, at[i], bt[i], ct[i], tat[i], wt[i]);
    }

    printf("Average TAT = %.2f  Average WT = %.2f\n", avg_tat, avg_wt);

    return 0;
}