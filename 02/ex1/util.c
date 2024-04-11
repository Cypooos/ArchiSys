#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "defns.h"

char **read_strings(int n) {
    while(getchar() != '\n');

    char **table = malloc(sizeof(char*)*n);
    if (!table) {
        printf("Malloc error\n");
        exit(1);
    }
    for (int i = 0;i<n;i++) 
        table[i] = NULL;
    
    size_t uwu = 0;
    int pos = 0;
    while (pos<n) {
        printf("Chaine.%d :",pos+1);
        fflush(NULL);
        if (getline(&table[pos++],&uwu,stdin) == -1) { // entry didn't suceed, retry
            pos--;
        }
    }

    return table;
}

void selsort(char **begin, char **end) {
    char** current = begin;
    do {
        char** mini = pos_min(current,end);
        swap(mini,current);
        current++;
    } while (current < end);
}

char **pos_min(char **begin, char **end) {
    char** mini = begin;
    char** current = begin;
    while(current++ < end)
        if(strcmp(*current, *mini)<0)
            swap(current, mini);
    return mini;
}

void swap(char **a, char **b) { 
    // OMG!!! IS THAT A SWAPPPP OwO !!! I ONLY SWAP POINTERRRS and not VALUESSSS because that way it's in O(1) UwU !!! Nyaaaaa
    char* temp = *a;
    *a = *b;
    *b = temp;
}

void print_strings(char **t, int n) {
    for(int i=0;i<n;i++) 
        printf("Line %d: %s",i,t[i]);
}

// Can't name a function "delete" as it's a C keyword...
void delete_(char **p, int n) {
    for(int i=0;i<n;i++) 
        free(p[i]);
    free(p);
};