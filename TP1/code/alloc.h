#ifndef _ALLOC_H_
#define _ALLOC_H_

#include "mmap.h"

#include <stdio.h>

#define VIRT_START 0x10000000 // the virtual memory fake start. The ith block will be faked at VIRT_START + PAGE_SIZE*i

extern int alloc_started;

typedef struct MemoryBlock {
    unsigned short size;      // The offset before the next block
    unsigned short used_size; // How many bytes we use on this offset
    struct MemoryBlock *next;
    char data[]; // the data
} MemoryBlock_t;

typedef enum MemType {
    SwapMem,
    AllocMem,
} MemType_t;

typedef struct BlockInfo
{
    void *virtual_addr; // NULL -> not yet made
    int last_use;
    MemType_t type;
    int page_id;
    MemoryBlock_t* first;
} BlockInfo_t;

typedef struct AllocInfo {
    unsigned int last_use;
    BlockInfo_t blocks[SWAP_SIZE + ALLOC_SIZE]; // the map behind virtual -> physical
} AllocInfo_t;

extern AllocInfo_t alloc_info;

void *my_malloc(unsigned int size);
void my_free(void* pt);
void my_copy(void *mem, void *data, int size);

void *virtual_to_physical(void* addr);

void print_alloc();
void init_alloc();

#endif