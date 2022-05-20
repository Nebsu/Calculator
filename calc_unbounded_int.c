#include "unbounded_int.c"
#include <stdio.h>

typedef struct variables{
    char *name;
    unbounded_int value;
    variables *next;
}variables;

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
        if(strcmp(name, current -> name) == 1){
            return 1;
        }
        current = current->next;
    }
    return 0;
}

variables *get(variables *list, char* name){
    variables *current = list;
    while (current->next != NULL) {
        if(strcmp(name, current -> name) == 1){
            return current;
        }
        current = current->next;
    }
    return NULL;
}



int main(int argc, char const *argv[]){
    FILE* file = fopen("source.txt", "r");
    FILE* file2 = fopen("dest.txt", "w");
    unbounded_int values[26];
    int i, equals = 0, var;
    for(size_t i = 0; i < 26; i++){
        values[i] = init_result(0);
    }
    while((i = fgetc(file))!=EOF){
        // if(i == 32)continue;
        if(i == '\n'){
            //reset les mem
            equals = 0;
        }else if(i == '='){
            equals = 1;
        }else if(i >= 'a' && i <= 'z'){
            var = i;
        }else if(i >= '0' && i <= '9'){
            values[var - 'a'] = insert_chiffre_fin(values[var - 'a'], i);
        }
    }
    fclose(file);
    printf("%s", unbounded_int2string(values[var - 'a']));
    fprintf(file2, "%c %c %s", var, '=', unbounded_int2string(values[var - 'a']));
    fclose(file2);
    return 0;
}
