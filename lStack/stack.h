#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <inttypes.h>

typedef struct llStackNode{
	struct llStackNode* next;
	void* data;
}llStackNode;

typedef struct Stack{
	llStackNode* top;
	uint32_t size;
	size_t dataSize;
	void* lastPopCpy;
}Stack;

Stack* stackInit(size_t size);
void* stackPop(Stack* s);
void stackPush(Stack* s, void* data);
void stackFree(Stack* s);
void stackDisplay(Stack* s);

llStackNode* llStackNodeInit(size_t size, void* data);
void llStackNodeFree(llStackNode* n);

#endif
