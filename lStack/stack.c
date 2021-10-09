#include "stack.h"
#include <stdio.h>
#include <string.h>

Stack* stackInit(size_t size){
	Stack* s = malloc(sizeof(Stack));
	s->top = NULL;
	s->lastPopCpy = calloc(1,size);
	s->size = 0;
	s->dataSize = size;
	return s;
}

void* stackPop(Stack* s){
	if (s->top == NULL){
		return NULL;
	}
	llStackNode* n = s->top;
	s->top = s->top->next;
	memcpy(s->lastPopCpy, n->data, s->size);
	llStackNodeFree(n);
	s->size--;
	return s->lastPopCpy;
}

void stackPush(Stack* s, void* data){
	llStackNode* n = llStackNodeInit(s->dataSize, data);
	n->next = s->top;
	s->top = n;
	s->size++;
}

void stackFree(Stack* s){
	while(s->top != NULL){
		llStackNode* n = s->top;
		s->top = s->top->next;
		llStackNodeFree(n);
	}
	free(s);
	s = NULL;
}

void stackDisplay(Stack* s){
	llStackNode* temp = s->top;
	while (temp != NULL){
		printf("[%u]->",*(uint32_t*)temp->data);
		temp = temp->next;
	}
	printf("[NULL]\nSize: %lu\n",s->size);
}

llStackNode* llStackNodeInit(size_t size, void* data){
	llStackNode* n = malloc(sizeof(llStackNode));
	n->next = NULL;
	n->data = malloc(size);
	memcpy(n->data, data, size);
	return n;
}

void llStackNodeFree(llStackNode* n){
	free(n->data);
	n->next = NULL;
	n->data = NULL;
	free(n);
	n = NULL;
}
