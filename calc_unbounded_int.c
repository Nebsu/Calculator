#include "unbounded_int.c"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

typedef struct mot mot;
struct mot{
    char* str;
    struct mot *next;
};
 
typedef mot* Texte;

typedef struct variable {
    char *label;
    unbounded_int value;
    struct variable *next;
} variable;

typedef variable* tableau;

tableau addVar(tableau list, char* str, unbounded_int val){
    variable* newElem = malloc(sizeof(variable));
    newElem->label = str;
    newElem->next = NULL;
    newElem->value = val;
    if(list == NULL){
        return newElem;
    }else{
        variable* temp=list;
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = newElem;
        return list;
    }
}

//Retourne la variable si elle est dans la liste, sinon retourne NULL
variable *contains(variable *list, char* str){
    variable *current = list;
    while (current->next != NULL) {
        if(strcmp(str, current -> label) == 0){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void afficherTab(tableau liste){
    variable *tmp = liste;
    while(tmp != NULL){
        printf("%s", tmp->label);
        tmp = tmp->next;
    }
}

// int isAffectation(const char *line) {
//     int egal = 0;
//     size_t n = strlen(line);
//     for (int i=0; i<n; i++) {
//         if (line[i] == '(' || line[i] == ')') return 0;
//         if (line[i] =='*' || line[i] =='+' || line[i] =='-') return 0;
//         if (line[i] =='=') egal += 1;
//     }
//     // Verification print :
//     char *mot = malloc(5 * sizeof(char));
//     if (mot == NULL) {perror("malloc"); return 0;} 
//     for (int i=0; i<n-5; i++) {
//         memmove(mot, line[i], sizeof(mot));
//         if (strcmp(mot, "print")==0) return 0;
//     }
//     // if variable non intialisée return 1 
//     if (egal > 1) return 0;
//     return 1;
// }

// int isPrint(const char *line) {
//     int print = 0;
//     size_t n = strlen(line);
//     for (int i=0; i<n; i++) {
//         if (line[i] == '(' || line[i] == ')') return 0;
//         if (line[i]=='*' || line[i]=='+' || line[i]=='-') return 0;
//         if (line[i]=='=') return 0;
//     }
//     // Verification print :
//     char *mot = malloc(5 * sizeof(char));
//     if (mot == NULL) {perror("malloc"); return 0;} 
//     for (int i=0; i<n-5; i++) {
//         memmove(mot, line[i], sizeof(mot));
//         if (strcmp(mot, "print")==0) return 1;
//     }
//     return 0;
// }

// int isOperation(const char *line) {
//     int egal = 0;
//     int operation = 0;
//     size_t n = strlen(line);
//     for (int i=1; i<n-1; i++) {
//         if (line[i] == '(' || line[i] == ')') return 0;
//         // Egal :
//         if (line[i] =='=') egal +=1;
//         if (line[i-1] =='=') egal +=1;
//         if (line[i+1] =='=') egal +=1;
//         // Operation :
//         if (line[i]=='*' || line[i]=='+' || line[i]=='-') {
//             if (line[i+1] == ' ' && line[i-1] == ' ') operation += 1;
//         }
//     }
//     if (egal != 1 || operation != 1) return 0;
//     // Verification print :
//     char *mot = malloc(5 * sizeof(char));
//     if (mot == NULL) {perror("malloc"); return 0;} 
//     for (int i=0; i<n-5; i++) {
//         memmove(mot, line[i], sizeof(mot));
//         if (strcmp(mot, "print")==0) return 0;
//     }
//     return 1;
// }

void printVar(FILE* dest, Texte ligne, variable *list){
    variable *v = contains(list, ligne -> str);
    if(v != NULL){
        fprintf(dest, "%s = %s\n" , v -> label, unbounded_int2string(v -> value));
    }else{
        fprintf(dest, "%s = %d\n" , ligne -> next -> str, 0);
    }
}

void affectVar(Texte ligne, variable *list){
    // Verifie si la variable est déja définie
    variable *v1 = contains(list, ligne -> str);
    // On récupère l'affectation
    mot *tmp = ligne;
    while(strcmp(tmp -> str, "=") != 0){
        tmp = tmp -> next;
    }
    char *res = tmp -> next -> str;
    unbounded_int n;
    // On vérifie si l'affectation est un entier ou une variable
    // Si c'est un entier on transforme l'entier en u_int
    // Sinon on récupère la variable et on vérifie si la variable est définie
    if(is_int(res) == 1){
        n = string2unbounded_int(res);
    }else{
        variable *v2 = contains(list, res);
        if(v2 != NULL){
            n = v2 -> value;
        }else{
            n = init_result(1);
        }
    }
    // Si la variable n'est pas définie on l'ajoute dans la liste avec l'affectation
    // Sinon on modifie la valeur de la variable
    if(v1 == NULL){
        list = addVar(list, ligne -> str, n);
    }else{
        v1 -> value = n;
    }
}

void OpVar(Texte ligne, variable *list, char signe){
    // Verifie si la variable est déja définie
    variable *v1 = contains(list, ligne -> str);
    // On récupère l'affectation
    mot *tmp = ligne;
    while(strcmp(tmp -> str, "=") != 0){
        tmp = tmp -> next;
    }
    tmp = tmp -> next;
    char *res1 = tmp -> str;
    // Variable 1 :
    // On vérifie si l'affectation est un entier ou une variable
    // Si c'est un entier on transforme l'entier en u_int
    // Sinon on récupère la variable et on vérifie si la variable est définie
    unbounded_int n1;
    if(is_int(res1) == 1){
        n1 = string2unbounded_int(res1);
    }else{
        variable *v2 = contains(list, res1);
        if(v2 != NULL){
            n1 = v2 -> value;
        }else{
            n1 = init_result(1);
        }
    }
    tmp = tmp -> next -> next;
    char *res2 = tmp -> str;
    // Variable 2 :
    unbounded_int n2;
    if(is_int(res2) == 1){
        n2 = string2unbounded_int(res2);
    }else{
        variable *v2 = contains(list, res2);
        if(v2 != NULL){
            n2 = v2 -> value;
        }else{
            n2 = init_result(1);
        }
    }
    if(signe == '+'){
        if(v1 == NULL){
            list = addVar(list, ligne -> str, unbounded_int_somme(n1,n2));
        }else{
            v1 -> value = unbounded_int_somme(n1,n2);
        }
    }else if(signe == '-'){
        if(v1 == NULL){
            list = addVar(list, ligne -> str, unbounded_int_difference(n1,n2));
        }else{
            v1 -> value = unbounded_int_somme(n1,n2);
        }
    }else if(signe == '*'){
        if(v1 == NULL){
            list = addVar(list, ligne -> str, unbounded_int_produit(n1,n2));
        }else{
            v1 -> value = unbounded_int_somme(n1,n2);
        }

    }

}

void readFile(FILE *file, variable *list){
    Texte liste = NULL;
    FILE *file = fopen("dest.txt", "r");
    int i;
    //Ajoute toutes les lignes dans le liste
    while((i = fgetc(file))!= EOF) {
        if(i == '\n')liste = ajouterFin(liste, "\n");
        char *mot = malloc(sizeof(char));
        fscanf(file, "%s", mot);
        liste = ajouterFin(liste, mot);
    }
    Texte liste2;
    //Recupere une ligne
    while(strcmp(liste -> str, "\n") != 0){
        liste2 = NULL;
        liste2 = ajouterFin(liste2, liste -> str);
        liste = liste -> next;
    }
    liste = liste -> next;
    // Liste2 = Ligne 1
    // Quand on refait la boucle on arrive sur la ligne 2
    afficherListe(liste2);
    while(strcmp(liste -> str, "\n") != 0){
        liste2 = NULL;
        liste2 = ajouterFin(liste2, liste -> str);
        liste = liste -> next;
    }
    liste = liste -> next;
    // afficherListe(liste2);
    // afficherListe(liste);
}

Texte ajouterFin(Texte liste, char* valeur){
    mot* newElem = malloc(sizeof(mot));
    newElem->str = valeur;
    newElem->next = NULL;
    if(liste == NULL){
        return newElem;
    }else{
        mot* temp=liste;
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = newElem;
        return liste;
    }
}

void afficherListe(Texte liste){
    mot *tmp = liste;
    while(tmp != NULL){
        printf("%s", tmp->str);
        tmp = tmp->next;
    }
}

void abc(char *str, tableau t){
    t = addVar(t, str, ll2unbounded_int(1));
}

int main(int argc, char const *argv[]) {



    FILE* file1;
    FILE* file2;
    if (argc == 1) {
        file1 = fopen(stdin, "r");
        file2 = fopen(stdout, "a+");
    } else if (argc == 3) {
        if (strcmp(argv[1], "-i")==0) {
            file1 = fopen(argv[2], "r");
            file2 = fopen(stdout, "a+");
        } else if (strcmp(argv[1], "-o")) {
            file1 = fopen(stdin, "r");
            file2 = fopen(argv[2], "a+");
        } else goto error;
    } else if (argc == 5) {
        if (strcmp(argv[1], "-i")!=0 || strcmp(argv[3], "-o")!=0) goto error;
        file1 = fopen(argv[2], "r");
        file2 = fopen(argv[4], "a+");
    } else {
        error :
        perror("Argument error");
        return 1;
    }

    return 0;
}
