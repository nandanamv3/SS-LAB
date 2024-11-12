#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *fp1, *fp2;
char record[100];

int readhead(){
    char pgmname[10],verifyname[10];
    fscanf(fp1,"%s",record);
    
    printf("Enter Program Name: ");
    scanf("%s",verifyname);
    int i=2;
    while(record[i]!='^'){
        pgmname[i-2]=record[i];
        i++;
    }
    pgmname[i]='\0';
    
    if(strcmp(pgmname,verifyname)!=0){
        printf("Invalid");
        exit(0);
    }
    else{
        printf("Program Name:%s\n",pgmname);
    }
    fscanf(fp1,"%s",record);
}

int main(){
    fp1=fopen("input.txt","r");
    fp2=fopen("output.txt","w");
    
    readhead();
    
    while(record[0]!='E'){
        int i=2,start;
        char address[10];
        while(record[i]!='^'){
            address[i-2]=record[i];
            i++;
        }
        printf("Starting Address: %s\n", address);
        start=atoi(address);
        i+=4;
        while(i<strlen(record)-1){
            if(record[i]!='^'){
                printf("%06d\t%c%c\n", start, record[i], record[i+1]);
                fprintf(fp2, "%06d\t%c%c\n", start, record[i], record[i+1]);
                start++;
                i+=2;
            }
            else{
                i++;
            }
        }
        fprintf(fp2,"\n");
        printf("\n");
        fscanf(fp1, "%s", record);
    }
    fclose(fp1);
    fclose(fp2);
    return 0;
}