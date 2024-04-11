// for testing purpose, overwrite the #define for a way lower alloc_size and swap_size
#define PAGE_SIZE 4096
#define ALLOC_SIZE 5
#define SWAP_SIZE 5
#define _MMAP_H_CONSTANTS

#include "mmap.c"
#include "assert.h"

int test() {
    // on alloc 3 pages
    PageInfo_t* page1 = page_alloc();
    PageInfo_t* page2 = page_alloc();
    PageInfo_t* page3 = page_alloc();
    assert(page1 != NULL && page2 != NULL && page3 != NULL); // test alloc good
    ((int*)page1->data)[0] = 11;
    ((int*)page2->data)[0] = -22;
    ((int*)page3->data)[0] = 33;

    // on en free 1 mais en ralloc 3
    page_free(page1->id);
    PageInfo_t* page4 = page_alloc();
    ((int*)page4->data)[0] = 44;
    PageInfo_t* page5 = page_alloc();
    ((int*)page5->data)[0] = 55;
    PageInfo_t* page6 = page_alloc(); // should re-allocate the freed one
    assert(((int*)page6->data)[0] == 0); // assert que les données soient bien mises à 0.
    ((int*)page6->data)[0] = 66;

    // test swap: move two pages
    int page2_swap_id = move_to_swap(page2->id); // also free the pages
    int page3_swap_id = move_to_swap(page3->id); 
    assert(page2_swap_id != -1 && page3_swap_id != -1);
    PageInfo_t* page7 = page_alloc(); // so we can allocate two ones here
    PageInfo_t* page8 = page_alloc(); 

    // let's free then read from the swap
    page_free(page6->id);
    PageInfo_t* page9 = read_from_swap(page3_swap_id);
    assert(((int*)page9->data)[0] == 33); // data has been conserved
}

int main() {

    //if (!started) {init();} // uncomment if NOT using gcc or clang 
    test();

    print_mem();
    printf("ALL TESTS OK (no assert raised)\n");
}