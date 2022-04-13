#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "unbounded_int.h"

unbounded_int ll2unbounded_int(long long i) {
    // Cas avec une entrée nulle
    if (i==0) {
        chiffre ch = {
            .suivant = NULL,
            .c = '0',
            .precedent = NULL
        };
        unbounded_int zero = {
            .signe =  '+',
            .len = 0,
            .premier = &ch,
            .dernier = &ch
        };
        return zero;
    }
    // On commence par le chiffre des unités :
    chiffre *fin = NULL;
    size_t length = 1;
    int puissance = 10;
    chiffre *ptr = fin->precedent;
    chiffre *prems = fin->precedent;
    chiffre *derch = fin->precedent;
while (i%puissance!=i);
    unbounded_int res = {
        .signe = (i>=0)? '+' : '-',
        .len = length,
        .premier = prems,
        .dernier = derch
    };
    return res;
}

void print_unbounded_int(unbounded_int *u) {
    chiffre *ptr = u->premier;
    while (ptr!=NULL) {
        char ch = ptr->c;
        printf("%c", ch);
        ptr = ptr->suivant;
    }
    printf("\n");
}

int main() {
    long long e0 = 0;
    long long e1 = 1878452;
    long long e2 = -174;
    printf("Entree = %lli\nSortie = ", e0);
    unbounded_int s0 = ll2unbounded_int(e0);
    print_unbounded_int(&s0);
    printf("Entree = %lli\nSortie = ", e1);
    unbounded_int s1 = ll2unbounded_int(e1);
    print_unbounded_int(&s1);
    return 0;
}
