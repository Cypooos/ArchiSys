// for testing purpose, overwrite the #define for a way lower alloc_size and swap_size

#include "alloc.h"
#include "assert.h"
#include <stdlib.h>
#include <assert.h>

void test_compactness() {
    int* t0 = my_malloc(sizeof(int));      // allocated on the first page (alloc_info.blocks[0])
    int* t1 = my_malloc(1000*sizeof(int)); // can follow on the first page 
    int* t2 = my_malloc(1000*sizeof(int)); // should alloc a new page (alloc_info.blocks[1])
    int* t3 = my_malloc(sizeof(int)); // should be on the first page 
    *(int*)virtual_to_physical(t3) = 42;
    
    assert(alloc_info.blocks[0].type == AllocMem);
    assert(alloc_info.blocks[1].type == AllocMem);
    assert(alloc_info.blocks[0].virtual_addr == (void*)VIRT_START);
    assert(alloc_info.blocks[1].virtual_addr == (void*)VIRT_START + PAGE_SIZE);
    assert(alloc_info.blocks[2].virtual_addr == NULL); // page 2 isn't used yet
    assert(((int*)alloc_info.blocks[0].first->next->next->data)[0] == 42); // t3 is indeed stored on page 1

    // print the memory
    print_alloc();
}

void test_last_unused() {
    int* t0 = my_malloc(1000*sizeof(int));
    int* t1 = my_malloc(1000*sizeof(int)); 
    int* t2 = my_malloc(1000*sizeof(int)); 
    int* t3 = my_malloc(1000*sizeof(int)); 
    int* t4 = my_malloc(1000*sizeof(int)); 
    *(int*)virtual_to_physical(t2) = 33;
    *(int*)virtual_to_physical(t0) = 11;
    *(int*)virtual_to_physical(t1) = 22;
    *(int*)virtual_to_physical(t4) = 55;
    *(int*)virtual_to_physical(t3) = 44;

    // we need a new page... but we don't have place, so the last used will be send to the swap, and that's t2
    // so after this instruction, t2 is on the swap
    int* t6 = my_malloc(1000*sizeof(int));
    *(int*)virtual_to_physical(t6) = 66;
    assert(alloc_info.blocks[2].type == SwapMem);

    print_alloc();
}

void test_free() {
    int* t1 = my_malloc(5*sizeof(int));
    int* t2 = my_malloc(10*sizeof(int));
    int* t3 = my_malloc(5*sizeof(int));
    *(int*)virtual_to_physical(t1) = 11;
    *(int*)virtual_to_physical(t2) = 22;
    *(int*)virtual_to_physical(t3) = 33;

    my_free(t2); // now t1 will have 40+sizeof(MemoryBlock_t) bytes behind him
    assert(alloc_info.blocks[0].first->next->next == NULL); // only two blocks
    assert(alloc_info.blocks[0].first->size == 5*sizeof(int)+10*sizeof(int)+sizeof(MemoryBlock_t)); 

    int* t4 = my_malloc(10*sizeof(int)); // the re-allocation can be fit in the gap of t1, so it should be beetween t1 and t2
    assert(*(int*)virtual_to_physical(t4) == 22); // we test that the freed memory of t2 is still in the new t4

    print_alloc();
}

void test_load_from_swap_on_acces() {

    // fill the ram
    int* t1 = my_malloc(1000*sizeof(int));
    int* t2 = my_malloc(1000*sizeof(int)); 
    int* t3 = my_malloc(1000*sizeof(int)); 
    int* t4 = my_malloc(1000*sizeof(int)); 
    int* t5 = my_malloc(1000*sizeof(int)); 
    *(int*)virtual_to_physical(t1) = 11;
    *(int*)virtual_to_physical(t2) = 22;
    *(int*)virtual_to_physical(t3) = 33;
    *(int*)virtual_to_physical(t4) = 44;
    *(int*)virtual_to_physical(t5) = 55;

    // force most unused page (page 1) to be put into the swap to malloc t6
    int* t6 = my_malloc(1000*sizeof(int)); 
    *(int*)virtual_to_physical(t6) = 66;

    // Will force most unused page (now page 2) to be put into the swap to read t1
    int v = *(int*)virtual_to_physical(t1);
    assert(*(int*)virtual_to_physical(t1) == 11);
    assert(alloc_info.blocks[1].type == SwapMem);
    print_alloc();
}


void test_copy() {
    // create a big table
    int* t1 = my_malloc(500*sizeof(int));
    for(int i=0;i<=500;i++) {
        ((int*)virtual_to_physical(t1))[i] = i*10;
    }

    // and my_copy the table into t2 twice (concat to hitself)
    int* t2 = my_malloc(1000*sizeof(int));
    my_copy(t2,t1,500*sizeof(int));
    my_copy(&t2[500],t1,500*sizeof(int));
    
    // assert that t2 is indeed (t1 || t1)
    for(int i=0;i<1000;i++) {
        assert(((int*)virtual_to_physical(t2))[i] == (i%500)*10);
    }
}


void test_copy_swap() {
    // create a big table
    int* t1 = my_malloc(500*sizeof(int));
    for(int i=0;i<=500;i++) {
        ((int*)virtual_to_physical(t1))[i] = i*10;
    }

    // fill the ram
    int* t2 = my_malloc(1000*sizeof(int)); 
    int* t3 = my_malloc(1000*sizeof(int)); 
    int* t4 = my_malloc(1000*sizeof(int)); 
    int* t5 = my_malloc(1000*sizeof(int)); 

    // now t1 is in the swap!
    int* t6 = my_malloc(1000*sizeof(int));

    my_copy(t6,t1,500*sizeof(int)); // but copy still works because they use the `virtual_to_physical` function
    my_copy(&t6[500],t1,500*sizeof(int));
    
    // assert that t2 is indeed (t1 || t1)
    for(int i=0;i<1000;i++) {
        assert(((int*)virtual_to_physical(t6))[i] == (i%500)*10);
    }
}


int main() {

    // if (!started) {init();} // uncomment if NOT using gcc or clang 
    // if (!alloc_started) {init_alloc();} // uncomment if NOT using gcc or clang 

    // uncomment the test you want to pass
    // since the memory must be truly reset for a test to be valuable, you can't run multiple one after another.
    // test_compactness();
    // test_last_unused();
    // test_free();
    // test_load_from_swap_on_acces();
    // test_copy();
    test_copy_swap();
}