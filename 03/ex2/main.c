#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* 
    With a buffer of 8192 bytes (1024*4) : 12 seconds (approx.)
    With a buffer of 1024 bytes          : 13 seconds (approx.)
    With a buffer of   16 bytes          : 25 seconds (approx.)
*/

int main(int argc, char *argv[]) {
    FILE *f_in, *f_out;
    char* file_in = NULL;
    char* file_out = NULL;
    int count = 1;
    int buff_size = 1024;
    for(int i=1;i<argc;i++) {
        if (!strcmp(argv[i],"-s")) {
            if (i+1<argc) {
                file_in = argv[i+1];
                i++;
            } else {
                printf("Missing file name for argument -s\n");
                return 2;
            }
        } else if (!strcmp(argv[i],"-d")) {
            if (i+1<argc) {
                file_out = argv[i+1];
                i++;
            } else {
                printf("Missing file name for argument -d\n");
                return 2;
            }
        } else if (!strcmp(argv[i],"-n")) {
            if (i+1<argc) {
                char* nil;
                count = strtol(argv[i+1], &nil, 10);
                if (errno == ERANGE) {
                    printf("Conversion to int failed for parameter -n\n");
                    return 2;
                }
                i++;
            } else {
                printf("Missing number for argument -n\n");
                return 2;
            }
        } else if (!strcmp(argv[i],"-b")) {
            if (i+1<argc) {
                char* nil;
                buff_size = strtol(argv[i+1], &nil, 10);
                if (errno == ERANGE || buff_size <=0) {
                    printf("Conversion to positive int failed for parameter -b\n");
                    return 2;
                }
                i++;
            } else {
                printf("Missing number for argument -b\n");
                return 2;
            }
        }
    }
    if (!file_in) {
        printf("Missing required -s file argument\n");
        return 2;
    }
    if (!file_out) {
        printf("Missing required -d file argument\n");
        return 2;
    }
    f_in = fopen(file_in,"r");
    if (!f_in) {
        printf("Unknow file `%s`\n",file_in);
        return 2;
    }
    f_out = fopen(file_out,"w");
    if (!f_out) {
        printf("Unknow file `%s`\n",file_in);
        return 2;
    }
    char buffer[buff_size]; int pos=0;
    for(int i=0;i<count;i++) {
        rewind(f_in);
        size_t si;
        do {
            si = fread(buffer, sizeof(char), buff_size, f_in);
            fwrite(buffer, sizeof(char), si, f_out);
        } while(buff_size == si);
    }
    fclose(f_in);
    fclose(f_out);
    return 0;
}

