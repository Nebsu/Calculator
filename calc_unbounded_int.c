#include "unbounded_int.c"
#include <stdio.h>
#include <string.h>

typedef struct variables {
    char *name;
    unbounded_int value;
    variables *next;
} variables;

void append(variables *list, char* name, unbounded_int val) {
    variables *current = list;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = (variables *) malloc(sizeof(variables));
    current->next->name = name;
    current->next->value = val;
    current->next->next = NULL;
}

int contains(variables *list, char* name){
    variables *current = list;
    while (current->next != NULL) {
        if(strcmp(name, current -> name) == 0){
            return 1;
        }
        current = current->next;
    }
    return 0;
}

variables *get(variables *list, char* name){
    variables *current = list;
    while (current->next != NULL) {
        if(strcmp(name, current -> name) == 0){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int isAffectation(const char *line) {
    int egal = 0;
    size_t n = strlen(line);
    for (int i=0; i<n; i++) {
        if (line[i] == '(' || line[i] == ')') return 0;
        if (line[i] =='*' || line[i] =='+' || line[i] =='-') return 0;
        if (line[i] =='=') egal += 1;
    }
    // Verification print :
    char *mot = malloc(5 * sizeof(char));
    if (mot == NULL) {perror("malloc"); return 0;} 
    for (int i=0; i<n-5; i++) {
        memmove(mot, line[i], sizeof(mot));
        if (strcmp(mot, "print")==0) return 0;
    }
    // if variable non intialisée return 1 
    if (egal > 1) return 0;
    return 1;
}

int isPrint(const char *line) {
    int print = 0;
    size_t n = strlen(line);
    for (int i=0; i<n; i++) {
        if (line[i] == '(' || line[i] == ')') return 0;
        if (line[i]=='*' || line[i]=='+' || line[i]=='-') return 0;
        if (line[i]=='=') return 0;
    }
    // Verification print :
    char *mot = malloc(5 * sizeof(char));
    if (mot == NULL) {perror("malloc"); return 0;} 
    for (int i=0; i<n-5; i++) {
        memmove(mot, line[i], sizeof(mot));
        if (strcmp(mot, "print")==0) return 1;
    }
    return 0;
}

int isOperation(const char *line) {
    int egal = 0;
    int operation = 0;
    size_t n = strlen(line);
    for (int i=1; i<n-1; i++) {
        if (line[i] == '(' || line[i] == ')') return 0;
        // Egal :
        if (line[i] =='=') egal +=1;
        if (line[i-1] =='=') egal +=1;
        if (line[i+1] =='=') egal +=1;
        // Operation :
        if (line[i]=='*' || line[i]=='+' || line[i]=='-') {
            if (line[i+1] == ' ' && line[i-1] == ' ') operation += 1;
        }
    }
    if (egal != 1 || operation != 1) return 0;
    // Verification print :
    char *mot = malloc(5 * sizeof(char));
    if (mot == NULL) {perror("malloc"); return 0;} 
    for (int i=0; i<n-5; i++) {
        memmove(mot, line[i], sizeof(mot));
        if (strcmp(mot, "print")==0) return 0;
    }
    return 1;
}

void readFile(FILE *file) {
    
}

int main(int argc, char const *argv[]){
    FILE* file1;
    FILE* file2;
    if (argc == 1) {
        file1 = fopen(stdin, "r");
        file2 = fopen(stdout, "w");
    } else if (argc == 3) {
        if (strcmp(argv[1], "-i")==0) {
            file1 = fopen(argv[2], "r");
            file2 = fopen(stdout, "w");
        } else if (strcmp(argv[1], "-o")) {
            file1 = fopen(stdin, "r");
            file2 = fopen(argv[2], "w");
        } else goto error;
    } else if (argc == 5) {
        if (strcmp(argv[1], "-i")!=0 || strcmp(argv[3], "-o")!=0) goto error;
        file1 = fopen(argv[2], "r");
        file2 = fopen(argv[4], "w");
    } else {
        error :
        perror("Argument error");
        return 1;
    }

    return 0;
}
