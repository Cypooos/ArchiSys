#include "mmap.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int started = 0;
// start `init()` automatically before main (works only on gcc and clang)
void  __attribute__ ((constructor)) init() {

    // the only malloc of the whole code
    PageInfo_t *alloc_pt = malloc(
        sizeof(PageInfo_t)*ALLOC_SIZE);

    // init allocated
    for(int i = 0;i<ALLOC_SIZE;i++) {
        memcpy(&alloc_pt[i], &empty_page, sizeof(empty_page));
        mem_info.memory[i] = &alloc_pt[i];
    }
    mem_info.current_id = 0;

    // init swap
    mem_info.swap = fopen("/tmp/swap", "wb+");
    for(int i = 0;i<SWAP_SIZE;i++) {
        fwrite(&empty_page, 1, sizeof(PageInfo_t), mem_info.swap);
    }
    fseek(mem_info.swap,0,SEEK_SET);
    started = 1;
}

PageInfo_t *page_alloc() {
    int init_allocated = mem_info.current_id;
    while(mem_info.memory[mem_info.current_id]->id != -1) {
        mem_info.current_id++;
        mem_info.current_id %= ALLOC_SIZE;
        if (mem_info.current_id == init_allocated) {
            //printf("alloc overflow.\n");
            return NULL; // no more free place in `mem_info.allocated`
        }
    }
    memcpy(mem_info.memory[mem_info.current_id], &empty_page, sizeof(empty_page));
    mem_info.memory[mem_info.current_id]->id = mem_info.current_id;
    return mem_info.memory[mem_info.current_id];
}

PageInfo_t *page_free(int page_id) {
    if (page_id>=ALLOC_SIZE || page_id<0 || mem_info.memory[page_id]->id == -1) {
        return NULL;
    }
    mem_info.memory[page_id]->id = -1;
    return mem_info.memory[page_id]; // Why returning it ? it's been freed, it shouldn't be accesed anymore... The specs are weird
}

int check_page_free_list() {
    int count=0;
    for(int i=0;i<ALLOC_SIZE;i++){
        if (mem_info.memory[i]->id == -1)
            count++;
    }
    return count;
}

int check_page_alloc() {
    return ALLOC_SIZE - check_page_free_list();
}

int search_swap_free() { // return an error code: >=0 -> ok and the id, -1 -> not found
    for(int i=0;i<SWAP_SIZE;i++) {
        fseek(mem_info.swap, sizeof(PageInfo_t)*i, SEEK_SET);
        int id;
        fread(&id,sizeof(int), 1, mem_info.swap);
        if (id == -1) {
            fseek(mem_info.swap, sizeof(PageInfo_t)*i, SEEK_SET);
            return i;
        }
    }
    return -1;
}

int move_to_swap(int page_id) { // return the new id within the swap
    if (page_id>=ALLOC_SIZE || page_id<0 || mem_info.memory[page_id]->id == -1) {
        return -1; // out of bounds
    }
    int swap_id = search_swap_free();
    if (swap_id == -1) {
        //printf("swap: overflow.\n");
        return -1;
    };
    mem_info.memory[page_id]->id = swap_id;
    fwrite(mem_info.memory[page_id],sizeof(PageInfo_t),1,mem_info.swap);
    page_free(page_id);
    mem_info.memory[page_id]->id = -1;
    return swap_id;
}

PageInfo_t *read_from_swap(int page_id) { // load a page from the swap 
    if (page_id>=SWAP_SIZE || page_id<0) {
        //printf("read: id=%d out of boud.\n",page_id);
        return NULL; // out of bounds
    }
    PageInfo_t* page = page_alloc();
    int alloc_id = page->id;
    if (!page) {
        //printf("read: alloc overflow.\n");
        return NULL; // no more free space
    };
    fseek(mem_info.swap, sizeof(PageInfo_t)*page_id, SEEK_SET);
    fread(page,sizeof(PageInfo_t),1,mem_info.swap);
    if ( page->id == -1) {
        fseek(mem_info.swap, 0, SEEK_SET);
        page_free(alloc_id);
        return NULL; // reading error
    }
    fseek(mem_info.swap, sizeof(PageInfo_t)*page_id, SEEK_SET);
    int z = -1;
    fwrite(&z, 1,sizeof(int),mem_info.swap);
    page->id = alloc_id;
    return page;
}

void print_mem() {
    printf("\n\n ========== print_mem ==========\n");
    printf("total alloc: %d\n",ALLOC_SIZE);
    printf("total swap : %d\n",SWAP_SIZE);
    printf("free       : %d\n",check_page_free_list());
    printf("used alloc : %d\n",check_page_alloc());
    printf("page size  : %d\n",PAGE_SIZE);
    printf("\n --- RAM  ---\n");
    for(int i=0;i<ALLOC_SIZE;i++) {
        printf("page #%d: id=%d, (int)data[0] = %d\n",i,mem_info.memory[i]->id,((int*)mem_info.memory[i]->data)[0]);
    }
    printf("\n --- SWAP ---\n");
    for(int i=0;i<SWAP_SIZE;i++) {
        fseek(mem_info.swap, sizeof(PageInfo_t)*i, SEEK_SET);
        int id;
        fread(&id,sizeof(int), 1, mem_info.swap);
        int data0;
        fread(&data0,sizeof(int), 1, mem_info.swap);
        printf("page #%d: id=%d, (int)data[0] = %d\n",i,id,data0);
    }
}