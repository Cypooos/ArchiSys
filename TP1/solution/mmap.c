#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "mmap.h"

struct MMap* mmap_init(){
  struct MMap *mmap = malloc(sizeof(struct MMap));
  mmap->size = 4096*100;
  mmap->memory = malloc(mmap->size);
  mmap->count = 0;
  mmap->support = malloc(sizeof(struct PageInfo));
  mmap->support->data = malloc(4096);
  mmap->free_list = createLinkedList();
  // insert 100 pages in the free list
  for (int i = 0; i < 100; i++) {
    struct PageInfo *page = malloc(sizeof(struct PageInfo));
    page->data = mmap->memory + i*4096;
    page->key = i;
    insertFirst(mmap->free_list, i, page);
  }
  mmap->alloc_list = createLinkedList();
  mmap->swap = fopen("/tmp/myswap", "wb+");
  int tmp = -1;
  for (int i = 0; i < 100*sizeof(int); i+=sizeof(int)) {
      fwrite(&tmp, sizeof(int), 1, mmap->swap);
  }
  char tmp2 = '\0';
  for (int i = 0; i < 100*4096; i++) {
      fwrite(&tmp2, 1, 1, mmap->swap);
  }
  return mmap;
}

void mmap_clean(struct MMap *mmap) {
  free(mmap->memory);
  fclose(mmap->swap);
  free(mmap);
  remove("/tmp/myswap");
}

struct PageInfo* page_alloc(struct MMap *mmap) {
  struct PageInfo *page = NULL;
  page = page_free(mmap);
  if(page!=NULL) {
    insertFirst(mmap->alloc_list, page->key, page);
    mmap->count++;
  }
  return page;
}

struct PageInfo* page_free(struct MMap *mmap) {
  struct PageInfo *page = NULL;
  struct node *node = deleteFirst(mmap->free_list);
  if (node != NULL) {
    page = node->data;
  }
  return page;
}

int check_page_free_list(struct MMap *mmap) {
  return length(mmap->free_list);
}

int check_page_alloc(struct MMap *mmap) {
  return length(mmap->alloc_list);
}

int move_to_swap(struct MMap *mmap, struct PageInfo *page) {
  int key = -1;
  fseek(mmap->swap, 0, SEEK_SET);
  if( ferror(mmap->swap) ) {
    return -1;
  }
  for (int i = 0; i < 100*sizeof(int); i+=sizeof(int)) {
    fread(&key, sizeof(int), 1, mmap->swap);
    if( ferror(mmap->swap) ) {
      return -1;
    }
    if (key == -1) {
      fseek(mmap->swap, i, SEEK_SET);
      if( ferror(mmap->swap) ) {
        return -1;
      }
      fwrite(&page->key, sizeof(int), 1, mmap->swap);
      if( ferror(mmap->swap) ) {
        return -1;
      }
      fseek(mmap->swap, 100*sizeof(int) + (i/sizeof(int))*4096, SEEK_SET);
      if( ferror(mmap->swap) ) {
        return -1;
      }
      fwrite(page->data, 4096, 1, mmap->swap);
      if( ferror(mmap->swap) ) {
        return -1;
      }
      return 0;
    }
  }
  return -1;
}

struct PageInfo* read_from_swap(struct MMap* mmap, struct PageInfo *page) {
  int key = -1;
  fseek(mmap->swap, 0, SEEK_SET);
  if( ferror(mmap->swap) ) {
    return NULL;
  }
  for (int i = 0; i < 100*sizeof(int); i+=sizeof(int)) {
    int ret = fread(&key, sizeof(int), 1, mmap->swap);
    if( ferror(mmap->swap) ) {
      return NULL;
    }
    if (key == page->key) {
      fseek(mmap->swap, 100*sizeof(int) + (i/sizeof(int))*4096, SEEK_SET);
      if( ferror(mmap->swap) ) {
        return NULL;
      }
      fread(page->data, 4096, 1, mmap->swap);
      if( ferror(mmap->swap) ) {
        return NULL;
      }
      fseek(mmap->swap, i, SEEK_SET);
      if( ferror(mmap->swap) ) {
        return NULL;
      }
      int tmp = -1;
      fwrite(&tmp, sizeof(int), 1, mmap->swap);
      if( ferror(mmap->swap) ) {
        return NULL;
      }
      return page;
    } else {
      return NULL;
    }
  }
  return NULL;
}

