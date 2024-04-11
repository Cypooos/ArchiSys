#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/*
L'exercice n'indiquait pas lex arguments en plus tel que "fichier d'entrée" et "fichier de sortie"...
J'ai choisi "aucun argument" pour le ficheir d'entrée et `-o XXX` pour le fichier de sortie.
*/

typedef enum vig_mode { unspecified=0, encrypt, decrypt } vig_mode_t ;

void vignere(char *buffer,char *key,vig_mode_t mode) {
    if (mode==encrypt) {
        for(int i=0;i<strlen(buffer);i++) {
            if(buffer[i] >= 'a' && buffer[i] <= 'z') {
                buffer[i] = ((buffer[i] - 'a' + key[i] - 'a' + 26*2) % 26) + 'a';
            } else if (buffer[i]>='A' && buffer[i] <= 'Z') {
                buffer[i] = ((buffer[i] - 'A' + key[i] - 'A' + 26*2) % 26) + 'A';
            };
        }
    } else if (mode==decrypt) {
        for(int i=0;i<strlen(buffer);i++) {
            if(buffer[i] >= 'a' && buffer[i] <= 'z') {
                buffer[i] = ((buffer[i] - key[i] + 26*2) % 26) + 'a';
            } else if (buffer[i]>='A' && buffer[i] <= 'Z') {
                buffer[i] = ((buffer[i] - key[i] + 26*2) % 26) + 'A';
            };
        }
    }
}

int main(int argc, char *argv[]) {
    FILE *f_in, *f_out;
    char* file_in = NULL;
    char* file_out = NULL;
    char* key = "\1";
    vig_mode_t mode = unspecified;
    for(int i=1;i<argc;i++) {
        if (!strcmp(argv[i],"-e")) {
            mode = encrypt;
        } else if (!strcmp(argv[i],"-d")) {
            mode = decrypt;
        } else if (!strcmp(argv[i],"-o")) {
            if (i+1<argc) {
                file_out = argv[i+1];
                i++;
            } else {
                printf("Missing file name for argument -o\n");
                return 2;
            }
        }else if (!strcmp(argv[i],"-k")) {
            if (i+1<argc) {
                char* nil;
                key = argv[i+1];
                i++;
            } else {
                printf("Missing string for argument -k\n");
                return 2;
            }
        } else {
            file_in = argv[i];
        }
    }
    if (!mode) {
        printf("Missing required mode argument (-e or -d)\n");
        return 2;
    }
    if (!file_in) {
        printf("Missing required file in argument\n");
        return 2;
    }
    if (!file_out) {
        printf("Missing required file out argument\n");
        return 2;
    }
    f_in = fopen(file_in,"r");
    if (!f_in) {
        printf("Unknow file `%s`\n",file_in);
        return 2;
    }
    f_out = fopen(file_out,"w");
    if (!f_out) {
        printf("Unknow file `%s`\n",file_out);
        return 2;
    }
    size_t size = strlen(key);
    char buffer[size]; int pos=0;
    size_t si;
    do {
        si = fread(buffer, sizeof(char), size, f_in);
        vignere(buffer,key,mode);
        fwrite(buffer, sizeof(char), si, f_out);
    } while(size == si);
    fclose(f_in);
    fclose(f_out);
    return 0;
}

