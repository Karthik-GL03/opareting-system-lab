#include <stdio.h>

struct Process {
    int pid;
    int at;
    int bt;
    int type;
    int ct, tat, wt, start;
};

void sortByArrival(struct Process p[], int n) {
    int i, j;
    struct Process temp;
    for(i = 0; i < n - 1; i++) {
        for(j = i + 1; j < n; j++) {
            if(p[i].at > p[j].at) {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
}

int main()
{
    int i,n;
    printf("enter the number of processers : ");
    scanf("%d",&n);
    struct Process p[n], systemQ[n], userQ[n];
    int sysCount = 0, userCount = 0;


    for(i = 0; i < n; i++) {
        printf("Enter PID, AT, BT, TYPE(0-System,1-User): ");
        scanf("%d %d %d %d", &p[i].pid, &p[i].at, &p[i].bt, &p[i].type);

        if(p[i].type == 0)
            systemQ[sysCount++] = p[i];
        else
            userQ[userCount++] = p[i];
    }
    sortByArrival(systemQ, sysCount);
    sortByArrival(userQ, userCount);

    int time = 0;

    for(i = 0; i < sysCount; i++) {
        if(time < systemQ[i].at)
            time = systemQ[i].at;

        systemQ[i].start = time;
        time += systemQ[i].bt;
        systemQ[i].ct = time;
        systemQ[i].tat = systemQ[i].ct - systemQ[i].at;
        systemQ[i].wt = systemQ[i].tat - systemQ[i].bt;
    }
    for(i = 0; i < userCount; i++) {
        if(time < userQ[i].at)
            time = userQ[i].at;

        userQ[i].start = time;
        time += userQ[i].bt;
        userQ[i].ct = time;
        userQ[i].tat = userQ[i].ct - userQ[i].at;
        userQ[i].wt = userQ[i].tat - userQ[i].bt;
    }

    printf("\nPID\tAT\tBT\tTYPE\tCT\tTAT\tWT\n");

    for(i = 0; i < sysCount; i++) {
        printf("%d\t%d\t%d\tSystem\t%d\t%d\t%d\n",
               systemQ[i].pid, systemQ[i].at, systemQ[i].bt,
               systemQ[i].ct, systemQ[i].tat, systemQ[i].wt);
    }

    for(i = 0; i < userCount; i++) {
        printf("%d\t%d\t%d\tUser\t%d\t%d\t%d\n",
               userQ[i].pid, userQ[i].at, userQ[i].bt,
               userQ[i].ct, userQ[i].tat, userQ[i].wt);
    }

    return 0;
}

