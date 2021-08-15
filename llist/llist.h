#ifndef LLIST_H
#define LLIST_H

#include <stdlib.h>
#include <inttypes.h>

typedef struct node{
	struct node* next;
	void* data;
}llNode;

llNode* llNodeInit(void* data, size_t size);

typedef struct{
	llNode* head;
	llNode* last;
	size_t size;
	uint32_t length;
}LinkedList;

// instantiates an empty linked list
LinkedList* llInit(size_t size);

// adds a new node to the linked list
void llAdd(LinkedList* ll, void* data);

// inserts a new node at the given position
// if index > length of linked list, it is inserted at the end
void llInsert(LinkedList* ll, void* data, uint32_t index);

// returns the data at the given index in the linke dlist
// if the index is greater than the linked lists length, then the last elements data is returned
// if no elements exist, NULL is returned
void* llGet(LinkedList* ll, uint32_t index);

void llErase(LinkedList* ll, uint32_t index);

void llFree(LinkedList* ll);

#endif
