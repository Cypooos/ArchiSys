#include <stdio.h>

void show(int x, int *y) {
    x++;
    (*y)++;
}

int main() {
    int a=0,b=0;
    printf("a=%d, b=%d (initial)\n",a,b);
    show(a,&b);
    printf("a=%d, b=%d (now, b changed)\n",a,b);
}