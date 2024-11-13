#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *argtab, *namtab, *deftab, *output, *input;
char opcode[10], operand[10], label[10], parameter[10], ARGS[100][10];
int EXPANDING = 0, LENGTH=0, line=0;

int set_up_arguments_in_argtab();
int substitute_arguments();
int substitute_positional_notation();
void PROCESSLINE();
int DEFINE();
int EXPAND();
int GETLINE();


int main(){
    output = fopen("expanded_file.txt", "w");
    input = fopen("input.txt", "r");

    fscanf(input, "%s %s %s", label, opcode, operand);
    while(strcmp(opcode, "END") != 0){
        PROCESSLINE();
        GETLINE();
    }
    fclose(output);
    fclose(input);
}


void PROCESSLINE(){
    namtab = fopen("namtab.txt", "r");
    char name[10];
    while(fscanf(namtab, "%s", name) != EOF){
        if(strcmp(name, opcode) == 0){
            fclose(namtab);
            EXPAND();
            return;
        }
    }

    if(strcmp(opcode, "MACRO") == 0){
        DEFINE();
    } 
    else if(strcmp(opcode, "MEND") != 0){
        fprintf(output, "%s\t%s\t%s\n", label, opcode, operand);
    }
    
}

int substitute_arguments(){
    
    char arg[100][10];
    int i = 0, index;

    argtab = fopen("argtab.txt", "r");
    while(fscanf(argtab, "%s", arg[i]) != EOF) i++;
    fclose(argtab);  
    for(int i=1; i<strlen(operand); i++){
        parameter[i-1] = operand[i];
    }
    parameter[i-1] = '\0';
    index = atoi(parameter)-1;
    strcpy(operand, arg[index]);
}

int GETLINE(){
    if(EXPANDING){
        fscanf(deftab, "%s%s%s", label, opcode, operand);
        substitute_arguments();
    } else {
        fscanf(input, "%s %s %s", label, opcode, operand);
    }
}

int set_up_arguments_in_argtab(){
    argtab = fopen("argtab.txt", "a");
    char arg[10];
    int j = 0;

    for(int i=0; i<strlen(operand); i++){
        if(operand[i] == ','){
            arg[j] = '\0';
            fprintf(argtab, "%s\n", arg);
            j = 0;  
        } 
        else 
            arg[j++] = operand[i];
        
    }
    fprintf(argtab, "%s\n", arg);
    fclose(argtab);
}

int EXPAND(){
    EXPANDING = 1;
    deftab = fopen("deftab.txt", "r");
    namtab = fopen("namtab.txt", "r");

    char name[10];
    int start, end;
    fscanf(namtab, "%s\t%d\t%d", name, &start, &end);
    while(strcmp(name,opcode) != 0)
        fscanf(namtab, "%s\t%d\t%d", name, &start, &end);

    fprintf(output, ".\t%s\t%s\t%s\n", label, opcode, operand);
    set_up_arguments_in_argtab();  

    for(int i=0; i<=start; i++){
        fscanf(deftab, "%s%s%s", label, opcode, operand);
    
    }

    while(strcmp(opcode, "MEND") != 0){
        GETLINE();
        PROCESSLINE();
    }

    EXPANDING = 0;
    fclose(namtab);
    fclose(deftab);
    return 0;
}

int substitute_positional_notation(){
    
    parameter[0] = '\0';
    char * token = strtok(operand, ",");
    while(token != NULL){
        sprintf(parameter, "%s?%d",parameter, ++LENGTH);
        strcpy(ARGS[LENGTH-1], token);
        printf("%s\n", token);
        token = strtok(NULL, ",");
        if(token != NULL)
            strcat(parameter,",");
    }
}


int DEFINE(){
    namtab = fopen("namtab.txt", "a");  
    deftab = fopen("deftab.txt", "a");  

    fprintf(namtab, "%s\t%d\t", label, line);
    substitute_positional_notation();
    fprintf(deftab, "%s\t%s\t%s\n", label, opcode, parameter);
    line++;
    
    int LEVEL = 1;
    while(LEVEL > 0) {
        GETLINE();
        
        if(label[0] != '.'){
            if(strcmp(opcode, "MACRO") == 0){
                LEVEL++;
            }
            else if(strcmp(opcode, "MEND") == 0){
                fprintf(deftab, "**\tMEND\t**\n");
                line++;
                LEVEL--;
            }
            else {
                for(int i=0; i<LENGTH; i++)
                    if(strcmp(operand, ARGS[i]) == 0)
                        sprintf(parameter, "?%d",i+1);    

                fprintf(deftab, "%s\t%s\t%s\n", label, opcode, parameter);
                line++;
            }
        }
    }
    fprintf(namtab, "%d\n", line);
    fclose(namtab);
    fclose(deftab);
}