#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct chiffre{
    struct chiffre *suivant;
    char c;
    struct chiffre *precedent;
} chiffre;

typedef struct{
    char signe;
    size_t len;
    chiffre *premier;
    chiffre *dernier;
} unbounded_int;

static int is_int(const char *e);
static unbounded_int insert_chiffre(unbounded_int i,const char e);
static unbounded_int insert_chiffre_fin(unbounded_int i,const char e);
static void print_unbounded_int(unbounded_int u);
static int count_digit(long long l);
static int get_digit(long long num, int n);
static unbounded_int unbounded_int_somme_positif(unbounded_int a, unbounded_int b);
static unbounded_int unbounded_int_difference_positif(unbounded_int a, unbounded_int b);

unbounded_int string2unbounded_int(const char *e);
char *unbounded_int2string(unbounded_int i);
int unbounded_int_cmp_int(unbounded_int a, unbounded_int b);
int unbounded_int_cmp_ll(unbounded_int a, long long b);
unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b);
unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b);