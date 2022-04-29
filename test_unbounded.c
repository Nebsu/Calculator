#include "unbounded_int.c"
#include <stdio.h>

int main(){
    unbounded_int a = string2unbounded_int("1234");
    unbounded_int b = string2unbounded_int("-8415");
    unbounded_int c = string2unbounded_int("0");
    unbounded_int d = string2unbounded_int("234");
    unbounded_int e = string2unbounded_int("-415");
    unbounded_int f = string2unbounded_int("-1234");
    
    printf("TEST STRING2UNBOUNDED_INT :\n");
    print_unbounded_int(a);
    print_unbounded_int(b);
    print_unbounded_int(c);
    print_unbounded_int(string2unbounded_int("aegfa"));

    printf("TEST LL2UNBOUNDED_INT :\n");
    print_unbounded_int(ll2unbounded_int(-8415));
    print_unbounded_int(ll2unbounded_int(1234));
    print_unbounded_int(ll2unbounded_int(0));

    printf("TEST UNBOUNDED_INT2STRING :\n");
    printf("%s\n", unbounded_int2string(a));
    printf("%s\n", unbounded_int2string(b));
    printf("%s\n", unbounded_int2string(c));

    printf("TEST UNBOUNDED_INT_CMP_INT :\n");
    printf("%d\n", unbounded_int_cmp_int(a,b));
    printf("%d\n", unbounded_int_cmp_int(b,a));
    printf("%d\n", unbounded_int_cmp_int(a,a));

    printf("TEST UNBOUNDED_INT_CMP_LL :\n");
    printf("%d\n", unbounded_int_cmp_ll(a,-8415));
    printf("%d\n", unbounded_int_cmp_ll(b,1234));
    printf("%d\n", unbounded_int_cmp_ll(a,1234));

    printf("TEST UNBOUNDED_INT_SOMME :\n");
    print_unbounded_int(unbounded_int_somme(a,a));
    print_unbounded_int(unbounded_int_somme(a,b));
    print_unbounded_int(unbounded_int_somme(b,a));
    print_unbounded_int(unbounded_int_somme(b,b));
    print_unbounded_int(unbounded_int_somme(a,e));
    print_unbounded_int(unbounded_int_somme(e,a));
    print_unbounded_int(unbounded_int_somme(d,b));
    print_unbounded_int(unbounded_int_somme(b,d));
    print_unbounded_int(unbounded_int_somme(a,f));
    print_unbounded_int(unbounded_int_somme(f,a));

    printf("TEST UNBOUNDED_INT_DIFFERENCE :\n");
    print_unbounded_int(unbounded_int_difference(a,a));
    print_unbounded_int(unbounded_int_difference(a,b));
    print_unbounded_int(unbounded_int_difference(b,a));
    print_unbounded_int(unbounded_int_difference(b,b));
    print_unbounded_int(unbounded_int_difference(a,e));
    print_unbounded_int(unbounded_int_difference(e,a));
    print_unbounded_int(unbounded_int_difference(d,b));
    print_unbounded_int(unbounded_int_difference(b,d));

    printf("TEST UNBOUNDED_INT_PRODUIT :\n");
    print_unbounded_int(unbounded_int_produit(a,a));
    print_unbounded_int(unbounded_int_produit(a,b));
    print_unbounded_int(unbounded_int_produit(b,a));
    print_unbounded_int(unbounded_int_produit(b,b));
    print_unbounded_int(unbounded_int_produit(a,e));
    print_unbounded_int(unbounded_int_produit(e,a));
    print_unbounded_int(unbounded_int_produit(d,b));
    print_unbounded_int(unbounded_int_produit(b,d));
    print_unbounded_int(unbounded_int_produit(a,f));
    print_unbounded_int(unbounded_int_produit(f,a));
    print_unbounded_int(unbounded_int_produit(c,a));
    print_unbounded_int(unbounded_int_produit(a,c));
    return 0;
}