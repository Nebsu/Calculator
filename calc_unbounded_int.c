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

int containsPrint(const char *line, int i) {
    char *mot = malloc(5 * sizeof(char));
    if (mot == NULL) {perror("malloc"); return 0;} 
    memmove(mot, line[i], sizeof(mot));
    if (strcmp(mot, "print")==0) return 1;
    return 0;
}

int isSpecialChar(char c) { 
    if (isalnum(c) == 0) return 1;
    else return 0;
}

int isPrint(const char *line) {
    int print = 0;
    size_t n = strlen(line);
    for (int i=0; i<n-5; i++) {
        if (line[i] == ' ') continue;
        if (containsPrint(line, i) == 1) {
            print = i + 5;
            break;
        }
        return 0;
    }
    if (print == 0) return 0;
    for (int i=print; i<n; i++) {
        if (line[i] == ' ') continue;
        if (isalpha(line[i]) >= 1) break;
        return 0;
    }
    return 1;
}

int isAffectation(const char *line) {
    int egal = 0;
    int var = 0;
    size_t n = strlen(line);
    for (int i=0; i<n; i++) {
        // Cas espace :
        if (line[i] == ' ') continue;
        // Verif print :
        if (containsPrint(line, i) == 1) return 0;
        // Cas variable :
        if (isalpha(line[i]) >= 1) {
            if (var == 1) return 0;
            var = 1;
            for (int j=i+1; isalpha(line[j]) >= 1; j++) {i = j;}
            continue;
        }
        // Cas égal :
        if (line[i] == '=') {
            if (var == 0) return 0;
            egal = i + 1;
            break;
        }
        return 0;
    }
    if (egal == 0 && var == 1) return 1;
    if (egal == 0 && var == 0) return 0;
    var = 0;
    int val = 0;
    for (int i=egal; i<n; i++) {
        // Cas espace :
        if (line[i] == ' ') continue;
        // Verif print :
        if (containsPrint(line, i) == 1) return 0;
        // Cas variable :
        if (isalpha(line[i]) == 1) {
            if (val == 1) return 0;
            if (var == 1) return 0;
            var = 1;
            for (int j=i+1; isalpha(line[j]) >= 1; j++) {i = j;}
            continue;
        }
        // Cas valeur :
        if (isdigit(line[i]) >= 1) {
            if (var == 1) return 0;
            if (val == 1) return 0;
            val = 1;
            for (int j=i+1; isdigit(line[j]) >= 1; j++) {i = j;}
            continue;
        }
        return 0;
    }
    return 1;
}

int isOperation(const char *line) {
    int egal = 0;
    int var = 0;
    size_t n = strlen(line);
    for (int i=0; i<n; i++) {
        // Cas espace :
        if (line[i] == ' ') continue;
        // Verif print :
        if (containsPrint(line, i) == 1) return 0;
        // Cas variable :
        if (isalpha(line[i]) == 1) {
            if (var == 1) return 0;
            var = 1;
            for (int j=i+1; isalpha(line[j]) >= 1; j++) {i = j;}
            continue;
        }
        // Cas égal :
        if (line[i] == '=') {
            if (var == 0) return 0;
            egal = i + 1;
            break;
        }
        return 0;
    }
    if (egal == 0 && var == 1) return 1;
    if (egal == 0 && var == 0) return 0;
    var = 0;
    int val = 0;
    int op = 0;
    for (int i=egal; i<n-1; i++) {
        // Cas espace :
        if (line[i] == ' ') continue;
        // Verif print :
        if (containsPrint(line, i) == 1) return 0;
        // Cas variable :
        if (isalpha(line[i]) == 1) {
            if (val == 1) return 0;
            if (var == 1) return 0;
            var = 1;
            for (int j=i+1; isalpha(line[j]) >= 1; j++) {i = j;}
            continue;
        }
        // Cas valeur :
        if (isdigit(line[i]) >= 1) {
            if (var == 1) return 0;
            if (val == 1) return 0;
            val = 1;
            for (int j=i+1; isdigit(line[j]) >= 1; j++) {i = j;}
            continue;
        }
        // Cas opération :
        if (line[i]=='*' || line[i]=='+' || line[i]=='-') {
            if (op == 1) return 0;
            op = 1;

        }
        return 0;
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
