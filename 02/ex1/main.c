#include <stdlib.h>
#include <stdio.h>
#include "defns.h"  // Pour les prototypes de fonctions implémentées


int main(void) {
    int n;
    printf("Nombre de chaînes de caractères à trier : ");
    scanf("%d", &n);
    char** v = read_strings(n);

    char** uwu = pos_min(v,v+n-1);

    if( v!=NULL ) {
        selsort(v,v+n-1);
        printf( "\nVecteur trié:\n" );
        print_strings(v,n);
        delete_(v,n);
    }
    else
        printf( "Erreur." );

    return 0;
}





