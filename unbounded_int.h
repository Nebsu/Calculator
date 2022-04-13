#include <stdio.h>
#include <stdlib.h>

typedef struct chiffre {
    struct chiffre *suivant;
    char c;
    struct chiffre *precedent;
} chiffre;

typedef struct unbounded_int {
    char signe; /* soit ’+’ soit ’-’ */
    size_t len; /* longueur de la liste */
    chiffre *premier; /* pointeur vers le premier élément de la liste*/
    chiffre *dernier; /*pointeur vers le dernier élément de la liste*/

} unbounded_int;
