// linked list
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "linked_list.h"


struct linkedList* createLinkedList() {
   struct linkedList *list = (struct linkedList*) malloc(sizeof(struct linkedList));
   list->head = NULL;
   return list;
}

//insert link at the first location
void insertFirst(struct linkedList *list, int key, void *data) {
   //create a link
   struct node *link = (struct node*) malloc(sizeof(struct node));
	
   link->key = key;
   link->data = data;
	
   //point it to old first node
   link->next = list->head;
	
   //point first to new first node
   list->head = link;
}

//delete first item
struct node* deleteFirst(struct linkedList *list) {
   if (list->head == NULL) {
      return NULL;
   }

   //save reference to first link
   struct node *tempLink = list->head;
	
   //mark next to first link as first 
   list->head = list->head->next;
	
   //return the deleted link
   return tempLink;
}

//find a link with given key
struct node* find(struct linkedList *list, int key) {

   //start from the first link
   struct node* current = list->head;

   //if list is empty
   if(list->head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->key != key) {
	
      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //go to next link
         current = current->next;
      }
   }      
	
   //if data found, return the current Link
   return current;
}

//delete a link with given key
struct node* deleteElement(struct linkedList *list, int key) {

   //start from the first link
   struct node* current = list->head;
   struct node* previous = NULL;
	
   //if list is empty
   if(list->head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->key != key) {

      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //store reference to current link
         previous = current;
         //move to next link
         current = current->next;
      }
   }

   //found a match, update the link
   if(current == list->head) {
      //change first to point to next link
      list->head = list->head->next;
   } else {
      //bypass the current link
      previous->next = current->next;
   }    
	
   return current;
}

//is list empty
bool isEmpty(struct linkedList *list) {
   return list->head == NULL;
}

int length(struct linkedList *list) {
   int length = 0;
   struct node *current;
	
   for(current = list->head; current != NULL; current = current->next) {
      length++;
   }
	
   return length;
}