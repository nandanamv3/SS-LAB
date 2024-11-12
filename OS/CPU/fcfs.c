#include<stdio.h>
#include<stdlib.h>

typedef struct {
    int pid;
    int at;  // Arrival Time
    int bt;  // Burst Time
    int ct;  // Completion Time
    int wt;  // Waiting Time
    int tat; // Turnaround Time
} Process;

void sort(int n, Process p[n]) {
    Process temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].at > p[j + 1].at) {
                temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

void print(int n, Process p[n]) {
    float avgT = 0, avgW = 0;
    printf("Process ID\tArrival Time\tBurst Time\tCompletion Time\tTurn Around Time\tWait Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
        avgT += p[i].tat;
        avgW += p[i].wt;
    }
    printf("Average Turn Around Time: %.2f\n", avgT / n);
    printf("Average Waiting Time: %.2f\n", avgW / n);
}

void gantt(int n,Process p[n]){
    printf("GAntt Chart:\n");
    for (int i = 0; i < n; i++) 
      {
        printf("| P%d\t", p[i].pid);
      }
      printf("|\n"); 
     printf("%d\t", p[0].at);
      for (int i = 0; i < n; i++) 
      {
        printf("%d\t", p[i].ct);
      }

}

void fcfs(int n, Process p[n]) {
    int time = 0;
    for (int i = 0; i < n; i++) {
        if (i == 0 || p[i].at > time) {
            time = p[i].at;
        }
        time += p[i].bt;
        p[i].ct = time;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
    }
    print(n, p);
    gantt(n, p);
}

int main() {
    int n;
   
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    Process p[n];
    for (int i = 0; i < n; i++) {
        printf("Enter the pid for process %d: ", i + 1);
        scanf("%d", &p[i].pid);
        printf("Enter the arrival time (AT) for process %d: ", i + 1);
        scanf("%d", &p[i].at);
        printf("Enter the burst time (BT) for process %d: ", i + 1);
        scanf("%d", &p[i].bt);
    }
    
    sort(n, p);
    fcfs(n, p);
    return 0;
}
