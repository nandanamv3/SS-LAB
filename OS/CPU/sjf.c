#include <stdio.h>
#include <string.h>

typedef struct{
    int id;
    int at;
    int bt;
    int wt;
    int ct;
    int tat;
}Process ;

void print(int n,Process p[n]){
    int avgT=0,avgW=0;
    printf("\nProcess id:\tArrival Time\tBurst Time\tCompletion Time\tTurnAround Time\tWaiting Time\n");
    for(int i=0;i<n;i++){
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",p[i].id,p[i].at,p[i].bt,p[i].ct,p[i].tat,p[i].wt);
        avgT+=p[i].tat;
        avgW+=p[i].wt;
    }
    printf("Average TAt:%f\n",(float)avgT/n);
    printf("Average Wt:%f\n",(float)avgW/n);
}

void sjf(int n,Process p[n]){
    int flag,completed=0,time=0,completedProcesses[n];
    memset(completedProcesses,0,sizeof(completedProcesses));
    int wait=0;
    printf("Gantt Chart: 0");
    while(completed<n){
        int flag=0;
        for(int i=0;i<n;i++){
            if(p[i].at<=time&& completedProcesses[i]!=1){
                if(wait){
                    printf("_%d",time);
                    wait=0;
                }
                time+=p[i].bt;
                p[i].ct=time;
                 printf("P%d %d ", p[i].id, time);
                p[i].tat=time-p[i].at;
                p[i].wt=p[i].tat-p[i].bt;
                completedProcesses[i]=1;
                completed++;
                flag=1;
                break;
            }
        }
        if(!flag){
            time++;
            wait=1;
        }
    }
    
    print(n,p);
}

void sort(int n,Process p[n]){
    Process temp;
    for(int i=0;i<n-1;i++){
        for(int j=0;j<n-i-1;j++){
            if(p[j].bt>p[j+1].bt){
                temp=p[j];
                p[j]=p[j+1];
                p[j+1]=temp;
            }
        }
    }
}

void main(){
    int n;
    printf("Enter the no. of processes:");
    scanf("%d",&n);
    Process p[n];
    
    for(int i=0;i<n;i++){
        printf("Enter the arrival time %d",i+1);
        scanf("%d",&p[i].at);
        printf("Enter the burst time %d",i+1);
        scanf("%d",&p[i].bt);
        p[i].id=i+1;
    }
    
    sort(n,p);
    sjf(n,p);
}