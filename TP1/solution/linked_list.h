#include <stdbool.h>

struct node {
   void *data;
   int key;
   struct node *next;
};

struct linkedList {
  struct node *head;
}; 

struct linkedList* createLinkedList();
void insertFirst(struct linkedList *list, int key, void *data);
struct node* deleteFirst(struct linkedList *list);
struct node* find(struct linkedList *list, int key);
struct node* deleteElement(struct linkedList *list, int key);
bool isEmpty(struct linkedList *list);
int length(struct linkedList *list);