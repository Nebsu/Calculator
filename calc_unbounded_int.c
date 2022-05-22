#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "unbounded_int.c"

#define MAX_LENGTH 500

// Structures :

typedef struct Mot Mot;
struct Mot {
    char* str;
    struct Mot *next;
};
typedef Mot* Texte;

typedef struct Variable {
    char *label;
    unbounded_int value;
    struct Variable *next;
} Variable;
typedef Variable* ListeVar;

// Fonctions sur les variables :

// Retourne la Variable si elle est dans la liste, sinon retourne NULL
Variable *contains(Variable *list, char* str) {
    Variable *current = list;
    while (current != NULL) {
        if(strcmp(str, current -> label) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Ajoute la variable ou la modifie s'il existe déja dans la liste :
ListeVar addVar(ListeVar list, char* str, unbounded_int val) {
    Variable* newElem = malloc(sizeof(Variable));
    assert(newElem != NULL);
    newElem->label = str;
    newElem->next = NULL;
    newElem->value = val;
    Variable *clone = contains(list, str);
    if (list == NULL) {
        return newElem;
    } else if (clone != NULL) {
        clone->value = val;
        return list;
    } else {
        Variable* temp = list;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newElem;
        return list;
    }
}

void afficherVar(ListeVar liste) {
    Variable *tmp = liste;
    while(tmp != NULL){
        printf("-%s = %s-\n", tmp->label, unbounded_int2string(tmp->value));
        tmp = tmp->next;
    }
}

Texte ajouterFin(Texte liste, char* valeur){
    Mot* newElem = malloc(sizeof(Mot));
    newElem->str = valeur;
    newElem->next = NULL;
    if(liste == NULL){
        return newElem;
    }else{
        Mot* temp=liste;
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = newElem;
        return liste;
    }
}

void afficherTexte(Texte liste){
    Mot *tmp = liste;
    while(tmp != NULL){
        printf("%s ", tmp->str);
        tmp = tmp->next;
    }
}

int isWord(const char *str) {
    size_t n = strlen(str);
    for (int i=0; i<n; i++) {
        if (isalpha(str[i]) == 0) return 0;
    }
    return 1;
}

// Fonctions qui détecte le type de la ligne (print, affectation ou opération) : 

int isPrint(Texte ligne) {
    Mot *ptr = ligne;
    if (ptr == NULL) return 0;
    char *s1 = ptr->str;
    if (isWord(s1) == 0) return 0;
    if (strcmp(s1, "print") != 0) return 0;
    ptr = ptr->next;

    if (ptr == NULL) return 0;
    char *s2 = ptr->str;
    if (isWord(s2) == 0) return 0;
    return 1;
}

int isAffectation(Texte ligne) {
    Mot *ptr = ligne;
    if (ptr == NULL) return 0;
    char *s1 = ptr->str;
    if (isWord(s1) == 0) return 0;
    ptr = ptr->next;
    
    if (ptr == NULL) return 0;
    char *s2 = ptr->str;
    if (strcmp(s2, "=") != 0) return 0;
    ptr = ptr->next;

    if (ptr == NULL) return 0;
    char *s3 = ptr->str;
    if(is_int(s3) == 0 && isWord(s3) == 0) return 0;
    ptr = ptr->next;

    if (ptr == NULL) return 1;
    return 0;
}

char isOperation(Texte ligne) {
    Mot *ptr = ligne;
    if (ptr == NULL) return 'F';
    char *s1 = ptr->str;
    if (isWord(s1) == 0) return 'F';
    ptr = ptr->next;
    if (ptr == NULL) return 'F';
    char *s2 = ptr->str;
    if (strcmp(s2, "=") != 0) return 'F';
    ptr = ptr->next;
    if (ptr == NULL) return 'F';
    char *s3 = ptr->str;
    // On doit avoir des nombres ou des variables :
    if (is_int(s3) == 0 && isWord(s3) == 0) return 'F';
    ptr = ptr -> next;
    if (ptr == NULL) return 'F';
    char *s4 = ptr->str;
    if (strcmp(s4, "+") != 0 && strcmp(s4, "-") != 0 && strcmp(s4, "*") != 0) return 'F';
    ptr = ptr->next;
    if (ptr == NULL) return 'F';
    char *s5 = ptr->str;
    if (is_int(s5) == 0 && isWord(s5) == 0) return 'F';
    return s4[0];
}

// Fonctions qui effectue les procédures du fichier texte (print, affectation ou opération) :

void printVar(FILE* dest, Texte ligne, Variable *list) {
    Variable *v = contains(list, ligne -> next -> str);
    if(v != NULL){
        char *s = unbounded_int2string(v -> value);
        fprintf(dest, "%s = %s\n" , v -> label, s);
    }else{
        fprintf(dest, "%s = %d\n" , ligne -> next -> str, 0);
    }
}

void affectVar(Texte ligne, Variable *list){
    char *var = ligne -> str;
    // On récupère l'affectation
    while(strcmp(ligne -> str, "=") != 0){
        ligne = ligne -> next;
    }
    ligne = ligne -> next;
    char *val = ligne -> str;
    unbounded_int n;
    // On vérifie si l'affectation est un entier ou une Variable
    // Si c'est un entier on transforme l'entier en u_int
    // Sinon on récupère la Variable et on vérifie si la Variable est définie
    if(is_int(val) == 1){
        n = string2unbounded_int(val);
    }else{
        Variable *v2 = contains(list, val);
        if(v2 != NULL){
            n = v2 -> value;
        }else{
            n = init_result(1);
        }
    }
    // Si la Variable n'est pas définie on l'ajoute dans la liste avec l'affectation
    // Sinon on modifie la valeur de la Variable
    list = addVar(list, var, n);
}

void opVar(Texte ligne, Variable *list, char signe){
    // On récupère l'affectation
    while(strcmp(ligne -> str, "=") != 0){
        ligne = ligne -> next;
    }
    ligne = ligne -> next;
    char *mot1 = ligne -> str;
    // Variable 1 :
    // On vérifie si l'affectation est un entier ou une Variable
    // Si c'est un entier on transforme l'entier en unbounded_int
    // Sinon on récupère la Variable et on vérifie si la Variable est définie
    unbounded_int n1;
    if(is_int(mot1) == 1){
        n1 = string2unbounded_int(mot1);
    }else{
        Variable *v2 = contains(list, mot1);
        if(v2 != NULL){
            n1 = v2 -> value;
        }else{
            n1 = init_result(1);
        }
    }
    ligne = ligne -> next -> next;
    char *mot2 = ligne -> str;
    // Variable 2 :
    unbounded_int n2;
    if(is_int(mot2) == 1){
        n2 = string2unbounded_int(mot2);
    }else{
        Variable *v2 = contains(list, mot2);
        if(v2 != NULL){
            n2 = v2 -> value;
        }else{
            n2 = init_result(1);
        }
    }
    if (signe == '+') {
        unbounded_int somme = unbounded_int_somme(n1, n2);
        printf("somme = ");
        print_unbounded_int(somme);
        list = addVar(list, ligne -> str, somme);
    } else if (signe == '-') {
        unbounded_int diff = unbounded_int_difference(n1,n2);
        printf("différence = ");
        print_unbounded_int(diff);
        list = addVar(list, ligne -> str, diff);
    } else if (signe == '*') {
        unbounded_int prod = unbounded_int_produit(n1,n2);
        printf("produit = ");
        print_unbounded_int(prod);
        list = addVar(list, ligne -> str, prod);
    }
}

void readFile(FILE *input, FILE *output) {
    Texte texte = NULL;
    static ListeVar variables = NULL;
    int i = 0;
    int a = 0, b = 0;
    char *c = malloc(sizeof(char) * MAX_LENGTH);
    // Ajoute toutes les lignes dans le texte
    while((i = fgetc(input))!= EOF) {
        if(a == 0 && i != ' '){
            c[b] = i;
            b++;
        }else{
            if(a == 0) texte = ajouterFin(texte, c);
            if( i == '\n') texte = ajouterFin(texte, "\n");
            char *mot = malloc(sizeof(char)*MAX_LENGTH);
            assert(mot != NULL);
            fscanf(input, "%s", mot);
            texte = ajouterFin(texte, mot);
            b = 1;
            a++;
        }
    }
    Texte ligne;
    // afficherTexte(texte);
    // Récupère les lignes
    while (texte != NULL) {
        while(strcmp(texte -> str, "\n") != 0){
            ligne = ajouterFin(ligne, texte -> str);
            texte = texte -> next;
        }
        // afficherTexte(ligne);
        // Traitement de la ligne :
        if (isPrint(ligne) == 1) {
            printVar(output, ligne, variables);
        } else if (isAffectation(ligne) == 1) {
            affectVar(ligne, variables);
        } else if (isOperation(ligne) != 'F') {
            opVar(ligne, variables, isOperation(ligne));
        } else {
            fprintf(output,"Ligne vide ou invalide\n");
        }
        texte = texte -> next;
        ligne = ligne -> next;
        ligne = NULL;
    }
    // Quand on refait la boucle on arrive sur la ligne 2
    // afficherTexte(ligne);
    // afficherTexte(texte);
}

int main(int argc, char const *argv[]) {
    FILE* file1;
    FILE* file2;
    if (argc == 1) {
        file1 = stdin;
        file2 = stdout;
        readFile(file1, file2);
    } else if (argc == 3) {
        if (strcmp(argv[1], "-i")==0) {
            file1 = fopen(argv[2], "r");
            file2 = stdout;
            readFile(file1, file2);
        } else if (strcmp(argv[1], "-o")) {
            file1 = stdin;
            file2 = fopen(argv[2], "w");
            readFile(file1, file2);
        } else goto error;
    } else if (argc == 5) {
        if (strcmp(argv[1], "-i")!=0 || strcmp(argv[3], "-o")!=0) goto error;
        file1 = fopen(argv[2], "r");
        file2 = fopen(argv[4], "w");
        readFile(file1, file2);
    } else {
        error :
        perror("Argument error");
        return 1;
    }
    fclose(file1);
    fclose(file2);
    return 0;
}
