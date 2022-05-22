#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "unbounded_int.c"

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

ListeVar addVar(ListeVar list, char* str, unbounded_int val) {
    Variable* newElem = malloc(sizeof(Variable));
    assert(newElem != NULL);
    newElem->label = str;
    newElem->next = NULL;
    newElem->value = val;
    if(list == NULL) {
        return newElem;
    } else {
        Variable* temp=list;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newElem;
        return list;
    }
}

// Retourne la Variable si elle est dans la liste, sinon retourne NULL
Variable *contains(Variable *list, char* str) {
    Variable *current = list;
    while (current->next != NULL) {
        if(strcmp(str, current -> label) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void afficherListe(ListeVar liste) {
    Variable *tmp = liste;
    while(tmp != NULL){
        printf("%s", tmp->label);
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

void afficherListe(Texte liste){
    Mot *tmp = liste;
    while(tmp != NULL){
        printf("%s", tmp->str);
        tmp = tmp->next;
    }
}

void abc(char *str, ListeVar t){
    t = addVar(t, str, ll2unbounded_int(1));
}

int isNumber(const char *str) {
    size_t n = strlen(str);
    for (int i=0; i<n; i++) {
        if (isdigit(str[i]) == 0) return 0;
    }
    return 1;
}

int isWord(const char *str) {
    size_t n = strlen(str);
    for (int i=0; i<n; i++) {
        if (isalpha(str[i]) == 0) return 0;
    }
    return 1;
}

// Fonctions qui détecte le type de la ligne (print, affectation ou opération) : 

int isPrint(Texte ligne, Variable *variables) {
    if (ligne == NULL) return 0;
    afficherListe(ligne);
    char *s1 = ligne->str;
    if (isWord(s1) == 0) return 0;
    if (strcmp(s1, "print") != 0) return 0;
    ligne = ligne->next;
    if (ligne == NULL) return 0;
    char *s2 = ligne->str;
    if (contains(variables, s2) == NULL) return 0;
    return 1;
}

int isAffectation(Texte ligne, Variable *variables) {
    if (ligne == NULL) return 0;
    afficherListe(ligne);
    char *s1 = ligne->str;
    if (isWord(s1) == 0) return 0;
    ligne = ligne->next;
    // Cas variable non-initialisée :
    if (ligne == NULL) return 1;
    char *s2 = ligne->str;
    if (strcmp(s2, "=") != 0) return 0;
    ligne = ligne->next;
    if (ligne == NULL) return 0;
    char *s3 = ligne->str;
    if (isNumber(s3) == 1) return 1;
    if (isWord(s3) == 1) {
        if (contains(variables, s3) != NULL) return 1;
    }
    return 0;
}

int isOperation(Texte ligne, Variable *variables) {
    if (ligne == NULL) return 0;
    afficherListe(ligne);
    char *s1 = ligne->str;
    if (isWord(s1) == 0) return 0;
    if (contains(variables, s1) == NULL) return 0;
    ligne = ligne->next;
    if (ligne == NULL) return 0;
    char *s2 = ligne->str;
    if (strcmp(s2, "=") != 0) return 0;
    ligne = ligne->next;
    if (ligne == NULL) return 0;
    char *s3 = ligne->str;
    // On doit avoir des nombres ou des variables :
    if (isNumber(s3) == 0 && isWord(s3) == 0) return 0;
    if (isWord(s3) == 1) {
        if (contains(variables, s3) == NULL) return 0;
    }
    ligne = ligne -> next;
    if (ligne == NULL) return 0;
    char *s4 = ligne->str;
    if (strcmp(s4, "+") != 0 && strcmp(s4, "-") != 0 && strcmp(s4, "*") != 0) return 0;
    ligne = ligne->next;
    if (ligne == NULL) return 0;
    char *s5 = ligne->str;
    if (isNumber(s3) == 0 && isWord(s3) == 0) return 0;
    if (isWord(s3) == 1) {
        if (contains(variables, s3) == NULL) return 0;
    }
    return 1;
}

// Fonctions qui effectue les procédures du fichier texte (print, affectation ou opération) :

void printVar(FILE* dest, Texte ligne, Variable *list) {
    Variable *v = contains(list, ligne -> str);
    if(v != NULL){
        fprintf(dest, "%s = %s\n" , v -> label, unbounded_int2string(v -> value));
    }else{
        fprintf(dest, "%s = %d\n" , ligne -> next -> str, 0);
    }
}

void affectVar(Texte ligne, Variable *list){
    // Verifie si la Variable est déja définie
    Variable *v1 = contains(list, ligne -> str);
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
        Variable *v2 = contains(list, res);
        if(v2 != NULL){
            n = v2 -> value;
        }else{
            n = init_result(1);
        }
    }
    // Si la Variable n'est pas définie on l'ajoute dans la liste avec l'affectation
    // Sinon on modifie la valeur de la Variable
    if(v1 == NULL){
        list = addVar(list, ligne -> str, n);
    }else{
        v1 -> value = n;
    }
}

void opVar(Texte ligne, Variable *list, char signe){
    // Verifie si la Variable est déja définie
    Variable *v1 = contains(list, ligne -> str);
    // On récupère l'affectation
    Mot *tmp = ligne;
    while(strcmp(tmp -> str, "=") != 0){
        tmp = tmp -> next;
    }
    tmp = tmp -> next;
    char *res1 = tmp -> str;
    // Variable 1 :
    // On vérifie si l'affectation est un entier ou une Variable
    // Si c'est un entier on transforme l'entier en u_int
    // Sinon on récupère la Variable et on vérifie si la Variable est définie
    unbounded_int n1;
    if(is_int(res1) == 1){
        n1 = string2unbounded_int(res1);
    }else{
        Variable *v2 = contains(list, res1);
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
        Variable *v2 = contains(list, res2);
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

void readFile(FILE *input, FILE *output) {
    Texte texte = NULL;
    ListeVar variables = malloc(sizeof(ListeVar));
    assert(variables != NULL);
    int i;
    // Ajoute toutes les lignes dans le texte
    while((i = fgetc(input))!= EOF) {
        if( i == '\n') texte = ajouterFin(texte, "\n");
        char *Mot = malloc(sizeof(char));
        assert(Mot != NULL);
        fscanf(input, "%s", Mot);
        texte = ajouterFin(texte, Mot);
    }
    Texte ligne;
    // Récupère les lignes
    while (texte != NULL) {
        while(strcmp(texte -> str, "\n") != 0){
            ligne = NULL;
            ligne = ajouterFin(ligne, texte -> str);
            texte = texte -> next;
        }
        // Traitement de la ligne :
        if (isPrint(ligne, variables) == 1) {

        } else if (isAffectation(ligne, variables) == 1) {

        } else if (isOperation(ligne, variables) == 1) {

        } else {
            fprintf(output, "Ligne vide ou invalide\n");
        }
        texte = texte -> next;
        ligne = ligne -> next;
    }
    // Quand on refait la boucle on arrive sur la ligne 2
    afficherListe(ligne);
    // afficherListe(texte);
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
    readfile(file1, file2);
    return 0;
}
