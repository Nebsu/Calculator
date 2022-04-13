#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unbounded_int.h>
#include <unbounded_int.c>

int main() {
    unbounded_int tab[5];
    tab[0] = string2unbounded_int("3610");
    tab[1] = string2unbounded_int("-19928");
    tab[2] = string2unbounded_int("^^@|{#@jfkeéphzfçuzepj");
    tab[3] = string2unbounded_int("-");
    tab[4] = string2unbounded_int("");
    for (int i=0; i<5; i++) {
        print_unbounded_int(tab[i]);
    }
}