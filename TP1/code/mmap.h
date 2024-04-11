#ifndef _MMAP_H_
#define _MMAP_H_
#include <stdio.h>

#define PAGE_SIZE 4096
#define ALLOC_SIZE 5
#define SWAP_SIZE 5

extern int started;

typedef struct PageInfo {
    int id; // -1 -> freed
    char data[PAGE_SIZE];
} PageInfo_t;


// here, the ids within swap and alloc can be the same.
struct MemInfo {
    int current_id;
    PageInfo_t *memory[ALLOC_SIZE]; // O(1) acces
    FILE *swap;
};

static struct MemInfo mem_info = {}; // global private variable

// useful private declaration for fast initilisalisation
static PageInfo_t empty_page = {.id=-1,.data={0}};


// since the struct MemInfo is a private global variable, no need to take it as an argument
void init();
PageInfo_t *page_alloc();
PageInfo_t *page_free(int page_id); // Why are we returning it if it's been freed ???? Weird specs
int check_page_free_list();
int check_page_alloc();
int move_to_swap(int id_page);   // return the id within the swap
PageInfo_t *read_from_swap(int id_page); // return the page loaded

void print_mem(); // print the state of the memory

#endif