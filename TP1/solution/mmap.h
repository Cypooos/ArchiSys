#include <stdio.h>

#include "linked_list.h"

struct MMap {
  void *memory;
  int size;
  int count;
  struct PageInfo *support;
  struct linkedList *free_list;
  struct linkedList *alloc_list;
  FILE *swap;
};

struct PageInfo {
  int key;
  void *data;
};

struct MMap* mmap_init();
void mmap_clean(struct MMap *mmap);
struct PageInfo* page_alloc(struct MMap *mmap);
struct PageInfo* page_free(struct MMap *mmap);
int check_page_free_list(struct MMap *mmap);
int check_page_alloc(struct MMap *mmap);
int move_to_swap(struct MMap *mmap, struct PageInfo *page);
struct PageInfo* read_from_swap(struct MMap* mmap, struct PageInfo *page);
