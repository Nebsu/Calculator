#include <stdio.h>
#include <math.h>

#include "unbounded_int.h"

static int is_int(const char *e) {
    size_t len = strlen(e);
    if (len == 0){
        return 0;
    }
    int a = 0;
    if (e[a]=='-') a = 1;
    while(a < len){
        if(e[a] > 47 && e[a] < 58){
            a++;
            continue;
        }
        return 0;
    }
    return 1;
}

static unbounded_int insert_chiffre(unbounded_int nb, const char e) {
    chiffre *new_chiffre = malloc(sizeof(chiffre*));
    if(new_chiffre != NULL){
        new_chiffre -> c = e;
        new_chiffre -> suivant = NULL;
        if(nb.premier == NULL){
            new_chiffre -> precedent = NULL;
            nb.premier = new_chiffre;
            nb.dernier = new_chiffre;
        }else{
            nb.dernier -> suivant = new_chiffre;
            new_chiffre -> precedent = nb.dernier;
            nb.dernier = new_chiffre;
        }
        nb.len++;
    }
    return nb;
}

static unbounded_int insert_chiffre_fin (unbounded_int nb, const char e){
  chiffre *new_chiffre = malloc(sizeof(chiffre*));
  if(new_chiffre != NULL){
        new_chiffre -> c = e;
        new_chiffre -> precedent = NULL;
        if(nb.dernier == NULL){
            new_chiffre -> suivant = NULL;
            nb.premier = new_chiffre;
            nb.dernier = new_chiffre;
        }else{
            nb.premier -> precedent = new_chiffre;
            new_chiffre -> suivant = nb.premier;
            nb.premier = new_chiffre;
        }
        nb.len++;
    }
    return nb;
}

static void print_unbounded_int(unbounded_int u) {
    printf("%c", u.signe);
    while (u.premier!= NULL) {
        printf("%c", u.premier -> c);
        u.premier = u.premier -> suivant;
    }
    printf("\n");
}

unbounded_int string2unbounded_int(const char *e){
    unbounded_int res;
    size_t slen = strlen(e);
    if (is_int(e)==0) {
        res.signe = '*';
        res.len = 0;
        res.premier = NULL;
        res.dernier = NULL;
    } else {
        res.signe = (e[0]=='-')? '-' : '+';
        res.len = 0;
        res.premier = NULL;
        res.dernier = NULL;
        int a = (e[0]=='-')? 1 : 0;
        for(int i = a; i < slen; i++){
            res = insert_chiffre(res, e[i]);
        }
    }
    return res;
}

char *unbounded_int2string(unbounded_int i){
    char* res = malloc(sizeof(char)*i.len+1);
    int mem = 1;
    res[0] = i.signe;
    while (i.premier!= NULL) {
        res[mem] = i.premier -> c;
        i.premier = i.premier -> suivant;
        mem++;
    }
    return res;
}

int unbounded_int_cmp_int(unbounded_int a, unbounded_int b){
    // Comparaison des signes
    if (a.signe == '-' && b.signe == '+'){
        return -1;
    }else if (a.signe == '+' && b.signe == '-'){
        return 1;
    }else{
        //Cas chaine plus longue
        if (a.len > b.len){
            return 1;
        }else if (a.len < b.len){
            return -1;
        // Cas ou les des chaines sont de la même longueur
        }else{ 
            while (a.premier!= NULL) {
                if(a.premier -> c > b.premier -> c){
                    return 1;
                }else if (a.premier -> c < b.premier -> c){
                    return -1;
                }
                a.premier = a.premier -> suivant;
                b.premier = b.premier -> suivant;
            }
        }
    }
    return 0;
}

static int count_digit(long long l){
    int count = 0;
    do {
        l /= 10;
        ++count;
    } while (l != 0);
    return count;
}

static int get_digit(long long num, int n){
    int r;
    r = num / pow(10, n);
    r = r % 10;
    return r;
}

int unbounded_int_cmp_ll(unbounded_int a, long long b){
    // Comparaison des signes
    int blen = count_digit(b);
    int mem = blen - 1;
    int digit = get_digit(b, mem);
    if (a.signe == '-' && b > 0){
        return -1;
    }else if (a.signe == '+' && b < 0){
        return 1;
    }else{
        //Cas chaine plus longue
        if (a.len > blen){
            return 1;
        }else if (a.len < blen){
            return -1;
        // Cas ou les des chaines sont de la même longueur
        }else{ 
            while (a.premier != NULL) {
                if(a.premier -> c - 48 > digit){
                    return 1;
                }else if (a.premier -> c - 48 < digit){
                    return -1;
                }
                a.premier = a.premier -> suivant;
                digit = get_digit(b, --mem);
            }
        }
    }
    return 0;
}

static unbounded_int unbounded_int_somme_positif(unbounded_int a, unbounded_int b){
    unbounded_int res = {res.signe = '+', res.len = 0, res.premier = NULL, res.dernier = NULL};
    unbounded_int max = b; unbounded_int min = a;
    int somme, reste, equals = 0;
    if(a.len > b.len){ 
        max = a;
        min = b;
    }else if(a.len == b.len){
        equals = 1;
    }
    while (min.dernier != NULL){
        somme = (min.dernier -> c - '0' + max.dernier -> c - '0' + reste)%10;
        reste = (min.dernier -> c - '0' + max.dernier -> c - '0' + reste)/10;
        res = insert_chiffre_fin(res, somme + '0');
        min.dernier = min.dernier -> precedent;
        max.dernier = max.dernier -> precedent;
    }
    if(equals == 1){
        res = insert_chiffre_fin(res, reste + '0');
    }
    while(max.dernier != NULL){
        somme = (max.dernier -> c - '0' + reste)%10;
        reste = (max.dernier -> c - '0' + reste)/10;
        res = insert_chiffre_fin(res, somme + '0');
        max.dernier = max.dernier -> precedent;
    }
    return res;
}

static unbounded_int unbounded_int_difference_positif(unbounded_int a, unbounded_int b){
    unbounded_int res = {res.signe = '+', res.len = 0, res.premier = NULL, res.dernier = NULL};
    int difference = 0; int reste = 0;
    while (b.dernier != NULL){
        if((a.dernier -> c - '0') - (b.dernier -> c - '0') + reste >= 0){
            difference = (a.dernier -> c - '0') - (b.dernier -> c - '0') + reste;
            reste = 0;
        }else{
            difference = (a.dernier -> c - '0') - (b.dernier -> c - '0') + reste + 10;
            reste = -1;
        }
        res = insert_chiffre_fin(res, difference + '0');
        b.dernier = b.dernier -> precedent;
        a.dernier = a.dernier -> precedent;
    }
    while(a.dernier != NULL){
        difference = a.dernier -> c - '0' + reste;
        reste = 0;
        res = insert_chiffre_fin(res, difference + '0');
        a.dernier = a.dernier -> precedent;
    }
    return res;
}

unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b){
    unbounded_int res;
    if(a.signe == '+' && b.signe == '+'){
        res = unbounded_int_somme_positif(a, b);
    }else if (a.signe == '-' && b.signe == '-'){
        res = unbounded_int_somme_positif(a, b);
        res.signe = '-';
    }else if (a.signe == '+' && b.signe == '-'){
        if(b.len > a.len){
            res = unbounded_int_difference_positif(b, a);
            res.signe = '-';
        }else{
            res = unbounded_int_difference_positif(a, b);
        }
    }else if (a.signe == '-' && b.signe == '+'){
        if(a.len > b.len){
            res = unbounded_int_difference_positif(a, b);
            res.signe = '-';
        }else{
            res = unbounded_int_difference_positif(b, a);
        }
    }
    return res;
}

unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b){
    unbounded_int res;
    if(a.signe == '+' && b.signe == '+'){
        if(unbounded_int_cmp_int(a, b) == -1){
            res = unbounded_int_difference_positif(b, a);
            res.signe = '-';
        }else{
            res = unbounded_int_difference_positif(a, b);
        }
    }else if (a.signe == '-' && b.signe == '-'){
        res = unbounded_int_somme_positif(b,a);
        res.signe = '-';
    }else if (a.signe == '+' && b.signe == '-'){
        res = unbounded_int_somme_positif(a, b);
    }else if (a.signe == '-' && b.signe == '+'){
        res = unbounded_int_somme_positif(b, a);
        res.signe = '-';
    }
    return res;
}

int main() { 
    const char* e = "-5411";
    const char* f = "5553";
    // long long l = 543;
    // printf("%d",is_int(e));
    // print_unbounded_int(string2unbounded_int(e));
    // printf("%s", unbounded_int2string(string2unbounded_int(e)));
    // printf("%d", unbounded_int_cmp_int(string2unbounded_int(f), string2unbounded_int(e)));
    // printf("%d", unbounded_int_cmp_ll(string2unbounded_int(e), l));
    // print_unbounded_int(unbounded_int_somme_positif(string2unbounded_int(e), string2unbounded_int(f)));
    // print_unbounded_int(unbounded_int_difference_positif(string2unbounded_int(e), string2unbounded_int(f)));
    // print_unbounded_int(unbounded_int_somme(string2unbounded_int(e), string2unbounded_int(f)));
    print_unbounded_int(unbounded_int_difference(string2unbounded_int(e), string2unbounded_int(f)));
    return 0;
}