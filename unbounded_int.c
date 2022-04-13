#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unbounded_int.h>

int is_int(const char *e) {
    if (e=="" || e=="-") return 0;
    for (size_t i=0; i<strlen(e); i++) {
        if (i==0 && e[i]=='-') continue;
        if (e[i]!='0' || e[i]!='1' || e[i]!='2' || e[i]!='3' || e[i]!='4' || 
            e[i]!='5' || e[i]!='6' || e[i]!='7' || e[i]!='8' || e[i]!='9')
            return 0;
    }
    return 1;
}

unbounded_int string2unbounded_int(const char *e) {
    unbounded_int res;
    if (is_int(e)==0) {
        res.signe = '*';
        res.len = 0;
        res.premier = NULL;
        res.dernier = NULL;
    } else {
        res.signe = (e[0]=='-')? '-' : '+';
        res.len = (e[0]=='-')? strlen(e)-1 : strlen(e);
        size_t a = (e[0]=='-')? 1 : 0;
        for (size_t i=a; i<res.len; i++) {
            res.premier->c = e[i];
        }
        for (size_t i=res.len-1; i>=a; i--) {
            res.dernier->c = e[i];
        }
    }
    return res;
}

void print_unbounded_int(unbounded_int u) {
    while (u.premier<=u.dernier) {
        char chiffre = u.premier->c;
        printf("%c", chiffre);
        u.premier++;
    }
    printf("\n");
}