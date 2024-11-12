#include <stdio.h>

typedef struct {
    int pid;    // Process ID
    int at;     // Arrival Time
    int bt;     // Burst Time
    int rt;     // Remaining Time
    int ct;     // Completion Time
    int tat;    // Turnaround Time
    int wt;     // Waiting Time
} Process;

typedef struct {
    int pid;    // Process ID in the Gantt Chart
    int start;  // Start Time
    int end;    // End Time
} GanttChartEntry;

void printGanttChart(int count, GanttChartEntry gantt[]) {
    printf("\nGantt Chart:\n");
    for (int i = 0; i < count; i++) {
        printf("|  P%d  ", gantt[i].pid);
    }
    printf("|\n");

    printf("0");
    for (int i = 0; i < count; i++) {
        printf("      %d", gantt[i].end);
    }
    printf("\n");
}

void printTable(int n, Process proc[]) {
    float totalTAT = 0, totalWT = 0;
    printf("\nProcess ID | Arrival Time | Burst Time | Completion Time | Turnaround Time | Waiting Time\n");
    for (int i = 0; i < n; i++) {
        proc[i].tat = proc[i].ct - proc[i].at;    // Turnaround Time
        proc[i].wt = proc[i].tat - proc[i].bt;    // Waiting Time
        totalTAT += proc[i].tat;
        totalWT += proc[i].wt;
        
        printf("    P%d     |      %d       |     %d      |       %d        |       %d        |      %d\n",
               proc[i].pid, proc[i].at, proc[i].bt, proc[i].ct, proc[i].tat, proc[i].wt);
    }
    printf("\nAverage Turnaround Time: %.2f\n", totalTAT / n);
    printf("Average Waiting Time: %.2f\n", totalWT / n);
}
void sort(int n,Process p[n]){
    Process temp;
    for(int i=0;i<n-1;i++){
        for(int j=0;j<n-i-1;j++){
            if(p[j].at>p[j+1].at){
                temp=p[j];
                p[j]=p[j+1];
                p[j+1]=temp;
            }
        }
    }
}

void roundRobin(Process proc[], int n, int timeQuantum) {
    int completed = 0, currentTime = 0;
    GanttChartEntry gantt[100];
    int ganttCount = 0;

    while (completed < n) {
        int allIdle = 1;
        for (int i = 0; i < n; i++) {
            if (proc[i].at <= currentTime && proc[i].rt > 0) {
                allIdle = 0; // Process ready to execute

                // Determine time to execute based on remaining time and time quantum
                int execTime = (proc[i].rt > timeQuantum) ? timeQuantum : proc[i].rt;
                
                gantt[ganttCount].pid = proc[i].pid;
                gantt[ganttCount].start = currentTime;
                currentTime += execTime;
                gantt[ganttCount].end = currentTime;
                ganttCount++;
                
                // Update remaining time
                proc[i].rt -= execTime;

                // Check if process is completed
                if (proc[i].rt == 0) {
                    proc[i].ct = currentTime; // Completion time
                    completed++;
                }
            }
        }
        // If no process is ready, increment time to avoid an infinite loop
        if (allIdle) {
            currentTime++;
        }
    }

    printGanttChart(ganttCount, gantt);
    printTable(n, proc);
}

int main() {
    int n, timeQuantum;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process proc[n];
    printf("Enter time quantum: ");
    scanf("%d", &timeQuantum);

    printf("Enter arrival time and burst time for each process:\n");
    for (int i = 0; i < n; i++) {
        proc[i].pid = i + 1;
        printf("P%d Arrival Time: ", proc[i].pid);
        scanf("%d", &proc[i].at);
        printf("P%d Burst Time: ", proc[i].pid);
        scanf("%d", &proc[i].bt);
        proc[i].rt = proc[i].bt;  // Initial remaining time is the burst time
    }
    sort (n,proc);
    roundRobin(proc, n, timeQuantum);

    return 0;
}