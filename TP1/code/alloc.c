#include "alloc.h"
#include <stdio.h>

AllocInfo_t alloc_info = {.blocks={0}, .last_use = 0};


void move_to_swap_last_block() { // move to the swap the most unused block (the one who's use date the most)
    int least_used_id = 0;
    int least_used_last_used = 9999999;
    for(int i=0;i<ALLOC_SIZE+SWAP_SIZE;i++) {
        if ( alloc_info.blocks[i].type == AllocMem && alloc_info.blocks[i].virtual_addr != NULL && alloc_info.blocks[i].last_use < least_used_last_used) {
            least_used_id = i;
            least_used_last_used = alloc_info.blocks[i].last_use;
        }
    }
    int new_id = move_to_swap(alloc_info.blocks[least_used_id].page_id); // move the page to the swap
    alloc_info.blocks[least_used_id].type = SwapMem; // make it as beeing stored in the swap
    alloc_info.blocks[least_used_id].page_id = new_id;
}

// will reload a page into the ram if it was in the swap
void move_from_swap_block(int i) {
    if (alloc_info.blocks[i].type == AllocMem) {
        return;
    }
    PageInfo_t *page = read_from_swap(alloc_info.blocks[i].page_id);
    alloc_info.blocks[i].type = AllocMem;
    alloc_info.blocks[i].page_id = page->id;
    alloc_info.blocks[i].last_use = alloc_info.last_use++;
    alloc_info.blocks[i].first = (MemoryBlock_t*)page->data;
}

void init_new_page(int i) {
    // TODO: if can't allocate, here push the least used page to swap
    PageInfo_t *page = page_alloc();
    if (page == NULL) { // if there is no free place on the ram
        move_to_swap_last_block(); // move the last used block to the swap
        page = page_alloc(); // now there's a free one !
    }
    MemoryBlock_t* block = (MemoryBlock_t*)page->data;

    block->size = PAGE_SIZE;
    block->used_size = 0;
    block->next = NULL;

    alloc_info.blocks[i].virtual_addr = (void*) VIRT_START + i*PAGE_SIZE;
    alloc_info.blocks[i].last_use = alloc_info.last_use++;
    alloc_info.blocks[i].type = AllocMem;
    alloc_info.blocks[i].page_id = page->id;
    alloc_info.blocks[i].first = block;
}



int alloc_started = 0;
void  __attribute__ ((constructor)) init_alloc() {
    if (alloc_started) {return;}
    for(int i=0;i<ALLOC_SIZE+SWAP_SIZE;i++) {
        alloc_info.blocks[i].virtual_addr = NULL;
    }
    alloc_started = 1;
}

void *my_malloc(unsigned int size) {
    for(int i=0;i<ALLOC_SIZE+SWAP_SIZE;i++) {
        if (alloc_info.blocks[i].virtual_addr == NULL) {
            init_new_page(i);
        }
        if (alloc_info.blocks[i].type == SwapMem) {continue;}; // will only malloc on a currently allocated block
        MemoryBlock_t *block = alloc_info.blocks[i].first;
        int d = 0;

        while (block != NULL) {
            // we're the first block on a new page
            if (block->used_size == 0 && block->size >= size+sizeof(MemoryBlock_t)) {
                block->used_size = size;
                alloc_info.blocks[i].last_use = alloc_info.last_use++;
                return (void*)(VIRT_START + PAGE_SIZE*i + (void*)&block->data-(void*)alloc_info.blocks[i].first);
            }
            // there's a bit of free place behind this block and the next !
            if (block->size >= size+sizeof(MemoryBlock_t) + block->used_size) {
                // insert new block
                MemoryBlock_t* new = (MemoryBlock_t*)&block->data[block->used_size];
                new->next = block->next;
                block->next = new;

                // set his size accordingly
                new->used_size = size;
                new->size = block->size - block->used_size - sizeof(MemoryBlock_t);
                // set the old block size 
                block->size = block->used_size;
                alloc_info.blocks[i].last_use = alloc_info.last_use++;
                return (void*)(VIRT_START + PAGE_SIZE*i + (void*)&new->data-(void*)alloc_info.blocks[i].first);
            }
            d++;
            block = block->next;
        }

    }
    printf("my_malloc: no more space.\n");
    return NULL; // out of space on the computer ! so sad.
}

void print_alloc_block(int page_id) {
    if (alloc_info.blocks[page_id].virtual_addr == NULL) {
        printf("page #%d: (unused)\n",page_id);
    } else if (alloc_info.blocks[page_id].type == SwapMem) {
        printf("page #%d: (swap)\n",page_id);
    } else {
        printf("page #%d: ",page_id);
        MemoryBlock_t *block = alloc_info.blocks[page_id].first;
        while (block != NULL) {
            printf("[%d:%d](%d)->",block->used_size, block->size,((int*)block->data)[0]);
            block = block->next;
        }
        printf("NULL\n");
    }
}

void print_alloc() {
    // For each block `[a:b](c)`
    //  - a is the number of malloc data
    //  - b the size of the block (there can be freed data beetween two blocks),
    //  - c the first 4 bytes of the data as a int.
    for(int i=0;i<SWAP_SIZE + ALLOC_SIZE;i++) {
        print_alloc_block(i);
    }
}

void my_free(void* addr) {
    unsigned long page_id = ((unsigned long)addr - VIRT_START)/PAGE_SIZE;
    if (page_id >= SWAP_SIZE+ALLOC_SIZE) {
        printf("my_free: wrong adress in. (block_id)\n");
        return;
    }
    void* real = virtual_to_physical(addr);
    MemoryBlock_t *block = alloc_info.blocks[page_id].first;
    while (block != NULL) {
        if ((void*)block->next+sizeof(MemoryBlock_t) == real) {
            block->size += block->next->size + sizeof(MemoryBlock_t); // eat the data of the next one
            block->next = block->next->next;
            return;
        } else if ((void*)block+sizeof(MemoryBlock_t) == real) { // if we are the first one, we can make the one before eat this one
            block->used_size = 0;
            return;
        } else if ((void*)block>real) { // we skipped it
            printf("my_free: wrong adress in. (skip)\n");
            return;
        }
        block = block->next;
    }
    printf("my_free: wrong adress in. (not found)\n");
}

void my_copy(void *mem /* destination */, void *data /* source */, int size) {
    char* mem_real = virtual_to_physical(mem);
    char* data_real = virtual_to_physical(data);
    for(int i=0;i<size;i++) {
        mem_real[i] = data_real[i];
    }

}

void *virtual_to_physical(void* addr) {
    unsigned long page_id = ((unsigned long)addr - VIRT_START)/PAGE_SIZE;
    if (page_id >= SWAP_SIZE+ALLOC_SIZE) {
        printf("virtual_to_physical: wrong adress in.\n");
        return NULL;
    }
    unsigned long block_offset = ((unsigned long)addr - VIRT_START)%PAGE_SIZE;

    if (alloc_info.blocks[page_id].type == SwapMem) { // reloading into the ram the page if it's been put in the swap
        alloc_info.blocks[page_id].last_use = alloc_info.last_use++; // we used it !
        move_to_swap_last_block();
        move_from_swap_block(page_id);
        return (void*)alloc_info.blocks[page_id].first + block_offset;
    }
    if (alloc_info.blocks[page_id].virtual_addr == NULL) {
        return NULL; // not a valid addr (page not mmap yet)
    }
    return (void*)alloc_info.blocks[page_id].first + block_offset;
}