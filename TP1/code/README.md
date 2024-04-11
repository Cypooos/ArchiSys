# Run

To run test, uncomment the test you want to run in `test.c` then execute `make run`.
You cannot uncomment multiple tests as they each require a full reseted allocated space.
For testing purpose, the number of pages in the ram and in the swap are both 5.
You can change that number in `mmap.h` (CF. the `ALLOC_SIZE` and `SWAP_SIZE` constants).
Because some tests are testing the habillity of the malloc/free to automatically loading / pushing to the swap the most unused page, changing thoses constant might break some tests (they test that some page are indeed push to the swap but with a bigger `ALLOC_SIZE` that may not be necessary).

**WARNING:** If you do not compile on CClang or GCC, the inits function won't automatically launched on started.
In that case, uncomment the first two comments of `main` in `test.c`

# Philosphy

Since we are trying to re-implement the memory management + an allocator, this whole project have **only one single malloc**, found in the `init` function in the `mmap.c` file.

# Functions

I implemented :
 - All four required functions (`my_malloc`, `my_free`, `my_copy`, `virtual_to_physicial`)
 - Re-utilization of freed blocks in the most compact manner possible.
 - A replacement beetween the alloc space and the swap of the most unused page if we need more pages
 - An automatic loading of pages that adresses are in the swap on using `virtual_to_physicial`
 - A `print_alloc` function to show the state of the memory. See it's definition in `alloc.c` for the legend.

The replacement policy is to use the pages that hasn't been used for the longest time. See the `last_used` field in the data structures.

# Data structure

## mmap.h

Because linked list are a bad idea for the pages, as they arn't in O(1) (but we know how much of them we have at startup), we use a table.
Here's the definition in `mmap.h`
```c
typedef struct PageInfo {
    int id;
    char data[PAGE_SIZE];
} PageInfo_t;

struct MemInfo {
    int current_id;
    PageInfo_t *memory[ALLOC_SIZE];
    FILE *swap;
};

static struct MemInfo mem_info = {};
```

And so, `mem_info.memory[i]` will be the (i+1)th ALLOCATED page of the computer.
If it's not initilized, then it's id will be -1.
A simmilar structure is used for the swap, but it's store within a file.
Not that "id" once initilised is the location of the page in the `mem_info.memory`, so that we have an O(1) acces when reading/writing: it's `mem_info.memory[id]`.
Because of this, id arn't unique beetween swap and the memory. This isn't an issue.

## alloc.h

The allocator is a table of `BlockInfo_t`, where a `BlockInfo_t` give more informations about a page of malloc'ed values: are they in the swap, loaded etc...
A page is allocated if `block.virtual_addr != NULL`.
If loaded, `block.first` is a pointor to the first element of a linked list of mallocs values within this page (`MemoryBlock_t`).
Each list element of this list contain a `size` and `used_size`, respectivly describing how much data before the next element and the malloc size of the element.

In case of a free, the block before (if it exist) will eat the unused data, gaining `used_size` but no `data`.
