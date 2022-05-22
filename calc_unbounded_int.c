#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "unbounded_int.c"

#define MAX_LENGTH 500
#define MAX_VAR_NUMBER 26

// Structures :

typedef struct Mot Mot;
struct Mot {
    char* str;
    struct Mot *next;
};
typedef Mot* Texte;

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
    if (strcmp(s1, "print") != 0) return 0;
    ptr = ptr->next;

    if (ptr == NULL) return 0;
    char *s2 = ptr->str;
    if (isalpha(s2[0]) == 0) return 0;
    return 1;
}

int isAffectation(Texte ligne) {
    Mot *ptr = ligne;
    if (ptr == NULL) return 0;
    char *s1 = ptr->str;
    if (isalpha(s1[0]) == 0) return 0;
    ptr = ptr->next;
    
    if (ptr == NULL) return 0;
    char *s2 = ptr->str;
    if (strcmp(s2, "=") != 0) return 0;
    ptr = ptr->next;

    if (ptr == NULL) return 0;
    char *s3 = ptr->str;
    if(is_int(s3) == 0 && isalpha(s3[0]) == 0) return 0;
    ptr = ptr->next;

    if (ptr == NULL) return 1;
    return 0;
}

char isOperation(Texte ligne) {
    Mot *ptr = ligne;
    if (ptr == NULL) return 'F';
    char *s1 = ptr->str;
    if (isalpha(s1[0]) == 0) return 'F';
    ptr = ptr->next;
    if (ptr == NULL) return 'F';
    char *s2 = ptr->str;
    if (strcmp(s2, "=") != 0) return 'F';
    ptr = ptr->next;
    if (ptr == NULL) return 'F';
    char *s3 = ptr->str;
    // On doit avoir des nombres ou des variables :
    if (is_int(s3) == 0 && isalpha(s3[0]) == 0) return 'F';
    ptr = ptr -> next;
    if (ptr == NULL) return 'F';
    char *s4 = ptr->str;
    if (strcmp(s4, "+") != 0 && strcmp(s4, "-") != 0 && strcmp(s4, "*") != 0) return 'F';
    ptr = ptr->next;
    if (ptr == NULL) return 'F';
    char *s5 = ptr->str;
    if (is_int(s5) == 0 && isalpha(s5[0]) == 0) return 'F';
    return s4[0];
}


// Fonctions qui effectue les procédures du fichier texte (print, affectation ou opération) :

void printVar(FILE* dest, Texte ligne, unbounded_int *tab) {
    char *s = ligne->next->str;
    char c = s[0];
    fprintf(dest, "%s = %s\n" , s, unbounded_int2string(tab[c-'a']));
}

void affectVar(Texte ligne, unbounded_int *tab){
    char *var = ligne->str;
    // On récupère l'affectation
    Mot *tmp = ligne;
    while(strcmp(tmp -> str, "=") != 0){
        tmp = tmp -> next;
    }
    char *res = tmp -> next -> str;
    unbounded_int n;
    // On vérifie si l'affectation est un entier ou une Variable
    // Si c'est un entier on transforme l'entier en u_int
    // Sinon on récupère la Variable et on vérifie si la Variable est définie
    if(is_int(res) == 1){
        n = string2unbounded_int(res);
    }else{
        char c = res[0];
        n = tab[c-'a'];
    }
}

void opVar(Texte ligne, unbounded_int *tab, char signe){
    char *var = ligne->str;
    // On récupère l'affectation
    Mot *tmp = ligne;
    while(strcmp(tmp -> str, "=") != 0){
        tmp = tmp -> next;
    }
    tmp = tmp -> next;
    char *res1 = tmp -> str;
    // Variable 1 :
    // On vérifie si l'affectation est un entier ou une Variable
    // Si c'est un entier on transforme l'entier en unbounded_int
    // Sinon on récupère la Variable et on vérifie si la Variable est définie
    unbounded_int n1;
    if(is_int(res1) == 1){
        n1 = string2unbounded_int(res1);
    }else{
        char c = res1[0];
        n1 = tab[c-'a'];
    }
    tmp = tmp -> next -> next;
    char *res2 = tmp -> str;
    // Variable 2 :
    unbounded_int n2;
    if(is_int(res2) == 1){
        n2 = string2unbounded_int(res2);
    }else{
        char c = res2[0];
        n2 = tab[c-'a'];
    }
    if(signe == '+'){
        unbounded_int somme = unbounded_int_somme(n1,n2);
        char c = var[0];
        tab[c-'a'] = somme;
    }else if(signe == '-'){
        unbounded_int difference = unbounded_int_difference(n1,n2);
        char c = var[0];
        tab[c-'a'] = difference;
    }else if(signe == '*'){
        unbounded_int produit = unbounded_int_produit(n1,n2);
        char c = var[0];
        tab[c-'a'] = produit;
    }
}

void readFile(FILE *input, FILE *output) {
    Texte texte = NULL;
    unbounded_int *variables = malloc(MAX_VAR_NUMBER * sizeof(unbounded_int));
    assert (variables != NULL);
    for(int i = 0; i < 26; i++){
        variables[i] = init_result(1);
    }
    int i = 0;
    int a = 0, b = 0;
    char *c = malloc(sizeof(char));
    // Ajoute toutes les lignes dans le texte
    while((i = fgetc(input))!= EOF) {
        if(i != ' ' && a != 0){
            if( i == '\n') texte = ajouterFin(texte, "\n");
            char *mot = malloc(sizeof(char));
            assert(mot != NULL);
            fscanf(input, "%s", mot);
            texte = ajouterFin(texte, mot);
        }else{
            c[0] = i;
            c[1] = '\0';
            a = 1;
            texte = ajouterFin(texte, c);
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
        afficherTexte(ligne);
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
