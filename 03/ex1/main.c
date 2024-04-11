#include <stdio.h>
#include <stdlib.h>

enum vari_ {neutral=0, positive=1, negative=2};
typedef char vari_t;

typedef struct row_offer {
    char mois, jour, annee;
    char clot_int, clot_dec;
    char open_int, open_dec;
    vari_t type;
    union {
        struct inner_a  {
            unsigned char maxi_int, maxi_dec;
            unsigned char mini_int, mini_dec;
        } neutral;
        struct inner_b  {
            unsigned char vari_int, vari_dec;
            unsigned char maxi_int, maxi_dec;
            unsigned char mini_int, mini_dec;
        } change;
    } content;
} offer_t;

int read_offer(FILE *f_in, offer_t *ele) {
    int r = fread(ele, sizeof(char), 8, f_in);
    if (r!= 8)
        return 0;
    switch(ele->type) {
        case neutral:
            r = fread((char*)ele+8, sizeof(char), 4, f_in);
            if (r!=4)
                return 0;
            break;
        default:
            r = fread((char*)ele+8, sizeof(char), 6, f_in);
            if (r!=6)
                return 0;
            break;
    }
    return 1;
}

void print_offer(offer_t *arg) {
    switch (arg->type) {
        case neutral:
            printf("%2d/%2d/%2d:  %2d.%2d %2d.%2d   =SAME==   %2d.%2d %2d.%2d\n",
                arg->mois, arg->jour, arg->annee,
                arg->clot_int, arg->clot_dec,
                arg->open_int, arg->open_dec,
                arg->content.neutral.maxi_int, arg->content.neutral.maxi_dec,
                arg->content.neutral.mini_int, arg->content.neutral.mini_dec
            );
            break;
        case positive:
            printf("%2d/%2d/%2d:  %2d.%2d %2d.%2d   +%2d.%2d%%   %2d.%2d %2d.%2d\n",
                arg->mois, arg->jour, arg->annee,
                arg->clot_int, arg->clot_dec,
                arg->open_int, arg->open_dec,
                arg->content.change.vari_int, arg->content.change.vari_dec,
                arg->content.change.maxi_int, arg->content.change.maxi_dec,
                arg->content.change.mini_int, arg->content.change.mini_dec
            );
            break;
        case negative:
            printf("%2d/%2d/%2d:  %2d.%2d %2d.%2d   -%2d.%2d%%   %2d.%2d %2d.%2d\n",
                arg->mois, arg->jour, arg->annee,
                arg->clot_int, arg->clot_dec,
                arg->open_int, arg->open_dec,
                arg->content.change.vari_int, arg->content.change.vari_dec,
                arg->content.change.maxi_int, arg->content.change.maxi_dec,
                arg->content.change.mini_int, arg->content.change.mini_dec
            );
            break;
    }
    return;
}

int main(int argc, char *argv[]) {
    FILE *f_in;
    if (argc!= 2) {
        printf("syntax: ./main <file>\n");
        return 2;
    }
    f_in = fopen(argv[1],"r");
    if (!f_in) {
        printf("Unknow file `%s`\n",argv[1]);
        return 2;
    }
    char clot_int, clot_dec;
    fread(&clot_int, sizeof(char), 1, f_in);
    fread(&clot_dec, sizeof(char), 1, f_in);
    size_t r = 1;
    offer_t element = {};
    printf("      %2d.%2d\n",clot_int,clot_dec);
    do {
        r = read_offer(f_in,&element);
        if (r)
            print_offer(&element);
    } while (r);
}