#ifndef _UTILS_H_
#define _UTILS_H_

char **read_strings(int n);
void selsort(char **begin, char **end);
char **pos_min(char **begin, char **end);
void swap(char **a, char **b);
void print_strings(char **t, int n);
void delete_(char **p, int n); // Cann't name a function "delete" as it's a C keyword...

#endif